// Fill out your copyright notice in the Description page of Project Settings.

#include "MoALiveLinkSource.h"
#include "ILiveLinkClient.h"
#include "Roles/LiveLinkAnimationTypes.h"
#include "Roles/LiveLinkAnimationRole.h"
#include "Misc/ByteSwap.h"

#include "Common/UdpSocketBuilder.h"

#define LOCTEXT_NAMESPACE "MoALiveLinkSource"
#define RECV_BUFFER_SIZE 1024 * 1024

void ParseBlobDataToFloats(const uint8* DataPtr, int32 BlobSize, TArray<float>& OutFloatArray);


MoALiveLinkSource::MoALiveLinkSource(FIPv4Endpoint Endpoint)
	:Socket(nullptr)
	, Stopping(false)
	, Thread(nullptr)
	, WaitTime(FTimespan::FromMilliseconds(100))
{
	DeviceEndpoint = Endpoint;

	SourceStatus = LOCTEXT("SourceStatus_DeviceNotFound", "Device Not Found");
	SourceType = LOCTEXT("MoALiveLinkSourceType", "MoA LiveLink");
	SourceMachineName = LOCTEXT("MoALiveLinkSourceMachineName", "localhost");

	Socket = FUdpSocketBuilder(TEXT("MoASocket"))
		.AsNonBlocking()
		.AsReusable()
		.BoundToAddress(FIPv4Address::Any)
		.BoundToPort(DeviceEndpoint.Port)
		.WithReceiveBufferSize(RECV_BUFFER_SIZE);

	RecvBuffer.SetNumUninitialized(RECV_BUFFER_SIZE);
	if ((Socket != nullptr) && (Socket->GetSocketType() == SOCKTYPE_Datagram))
	{
		SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
		Start();
		SourceStatus = LOCTEXT("SourceStatus_Receiving", "Receiving");
	}
}

MoALiveLinkSource::~MoALiveLinkSource()
{
	Stop();
	if (Thread != nullptr)
	{
		Thread->WaitForCompletion();
		delete Thread;
		Thread = nullptr;
	}
	if (Socket != nullptr)
	{
		Socket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Socket);
	}
}

void MoALiveLinkSource::ReceiveClient(ILiveLinkClient* InClient, FGuid InSourceGuid)
{
	Client = InClient;
	SourceGuid = InSourceGuid;
}

bool MoALiveLinkSource::IsSourceStillValid() const
{
	bool bIsSourceValid = !Stopping && Thread != nullptr && Socket != nullptr;
	return bIsSourceValid;
}

bool MoALiveLinkSource::RequestSourceShutdown()
{
	Stop();
	return true;
}

uint32 MoALiveLinkSource::Run()
{
	TSharedRef<FInternetAddr> Sender = SocketSubsystem->CreateInternetAddr();

	while (!Stopping)
	{
		if (Socket->Wait(ESocketWaitConditions::WaitForRead, WaitTime))
		{
			uint32 Size;

			while (Socket->HasPendingData(Size))
			{
				int32 Read = 0;
				/* Socket Data Receive */
				if (Socket->RecvFrom(RecvBuffer.GetData(), RecvBuffer.Num(), Read, *Sender))
				{
					if (Read > 0)
					{
						TSharedPtr<TArray<uint8>, ESPMode::ThreadSafe> ReceivedData = MakeShareable(new TArray<uint8>());
						ReceivedData->SetNumUninitialized(Read);
						memcpy(ReceivedData->GetData(), RecvBuffer.GetData(), Read);
						AsyncTask(ENamedThreads::GameThread, [this, ReceivedData]() {ProcessReceivedData(ReceivedData); });
					}
				}
			}
		}
	}

	return 0;
}

void MoALiveLinkSource::Start()
{
	ThreadName = "MoA OSC Receiver";
	ThreadName.AppendInt(FAsyncThreadIndex::GetNext());
	Thread = FRunnableThread::Create(this, *ThreadName, 128 * 1024, TPri_AboveNormal, FPlatformAffinity::GetPoolThreadMask());
}

void MoALiveLinkSource::Stop()
{
	Stopping = true;
}

void MoALiveLinkSource::ProcessReceivedData(TSharedPtr<TArray<uint8>, ESPMode::ThreadSafe> ReceivedData)
{
	FName SubjectName = "MoAMotionCapture";
	const uint8* DataPtr = ReceivedData->GetData();
	int32 DataSize = ReceivedData->Num();
	int32 Offset = 0;

	/* Address, /MOA/Stream/Motion/Unreal */
	FString AddressPattern = ParseString(DataPtr, Offset, DataSize);
	if (AddressPattern.IsEmpty() || AddressPattern != "/MOA/Stream/Motion/Unreal/Local")
	{
		return;
	}

	/* TypeTag */
	FString TypeTags = ParseString(DataPtr, Offset, DataSize);
	if (TypeTags.IsEmpty() || TypeTags[0] != ',')
	{
		return;
	}

	/* bone data */
	TArray<float> BoneDatas;
	for (int32 i = 0; i < TypeTags.Len(); i++)
	{
		if (TypeTags[i] == 'i')
		{
			if (Offset + sizeof(int32) > DataSize)
			{
				UE_LOG(LogTemp, Warning, TEXT("Data out of bounds for int32"));
				break;
			}
			int32 IntValue = *reinterpret_cast<const int32*>(DataPtr + Offset);
			Offset += sizeof(int32);
		}
		else if (TypeTags[i] == 'b')
		{
			if (Offset + sizeof(float) > DataSize)
			{
				UE_LOG(LogTemp, Error, TEXT("Offset exceeds data size for float. Offset: %d, DataSize: %d"), Offset, DataSize);
				return;
			}
			int32 BlobSize = *reinterpret_cast<const int32*>(DataPtr + Offset);
			Offset += sizeof(int32);

			BlobSize = ByteSwap(BlobSize);
			ParseBlobDataToFloats(DataPtr + Offset, BlobSize, BoneDatas);
			Offset += BlobSize;
		}
		else
		{
			continue;
		}
	}

	if (BoneDatas.Num() != 350)
	{
		UE_LOG(LogTemp, Error, TEXT("Insufficient Bone Data received : %d"), BoneDatas.Num());
		return;
	}

	const int32 NumBones = 50;

	/* static data init */
	bool bIsEncounter = !EncounterdSubjects.Contains(SubjectName);
	if (bIsEncounter)
	{
		InitStaticData(NumBones, SubjectName);
		EncounterdSubjects.Add(SubjectName);
	}

	FLiveLinkFrameDataStruct FrameDataStruct = FLiveLinkFrameDataStruct(FLiveLinkAnimationFrameData::StaticStruct());
	FLiveLinkAnimationFrameData& FrameData = *FrameDataStruct.Cast<FLiveLinkAnimationFrameData>();

	FrameData.Transforms.SetNumZeroed(NumBones);

	int32 boneOffset = 0;
	for (int32 i = 0; i < NumBones; i++)
	{
		FVector BonePosition(BoneDatas[boneOffset], BoneDatas[boneOffset + 1], BoneDatas[boneOffset + 2]);
		FQuat BoneRotation(BoneDatas[boneOffset + 3], BoneDatas[boneOffset + 4], BoneDatas[boneOffset + 5], BoneDatas[boneOffset + 6]);

		FTransform LocalTransform(BoneRotation, BonePosition, FVector(1.0f));
		FrameData.Transforms[i] = LocalTransform;
		boneOffset += 7;
	}

	/* Push Subject Data */
	Client->PushSubjectFrameData_AnyThread({ SourceGuid, SubjectName }, MoveTemp(FrameDataStruct));
}

void MoALiveLinkSource::InitStaticData(int32 NumBones, FName SubjectName)
{
	FLiveLinkStaticDataStruct StaticDataStruct = FLiveLinkStaticDataStruct(FLiveLinkSkeletonStaticData::StaticStruct());
	FLiveLinkSkeletonStaticData& StaticData = *StaticDataStruct.Cast<FLiveLinkSkeletonStaticData>();

	StaticData.BoneNames.SetNumUninitialized(NumBones); //51°³
	StaticData.BoneParents.SetNumUninitialized(NumBones); // root bone Æ÷ÇÔ

	for (int id = 0; id < NumBones; id++) // pelvis~hand_r
	{
		FString BoneName = SkeletalBoneNames[id];
		StaticData.BoneNames[id] = FName(BoneName);
		StaticData.BoneParents[id] = -1;
	}

	StaticData.BoneParents[7] = 0;    // spine_01 -> pelvis
	StaticData.BoneParents[8] = 7;    // spine_02 -> spine_01
	StaticData.BoneParents[9] = 8;    // spine_03 -> spine_02
	StaticData.BoneParents[14] = 9;   // neck_01 -> spine_03
	StaticData.BoneParents[15] = 14;  // head -> neck_01

	StaticData.BoneParents[1] = 0;    // thigh_l -> pelvis
	StaticData.BoneParents[2] = 1;    // calf_l -> thigh_l
	StaticData.BoneParents[3] = 2;    // foot_l -> calf_l

	StaticData.BoneParents[4] = 0;    // thigh_r -> pelvis
	StaticData.BoneParents[5] = 4;    // calf_r -> thigh_r
	StaticData.BoneParents[6] = 5;    // foot_r -> calf_r

	StaticData.BoneParents[10] = 9;   // clavicle_l -> spine_03
	StaticData.BoneParents[11] = 10;  // upperarm_l -> clavicle_l
	StaticData.BoneParents[12] = 11;  // lowerarm_l -> upperarm_l
	StaticData.BoneParents[13] = 12;  // hand_l -> lowerarm_l

	StaticData.BoneParents[16] = 9;   // clavicle_r -> spine_03
	StaticData.BoneParents[17] = 16;  // upperarm_r -> clavicle_r
	StaticData.BoneParents[18] = 17;  // lowerarm_r -> upperarm_r
	StaticData.BoneParents[19] = 18;  // hand_r -> lowerarm_r

	StaticData.BoneParents[20] = 13;  // thumb_01_l -> hand_l
	StaticData.BoneParents[21] = 20;  // thumb_02_l -> thumb_01_l
	StaticData.BoneParents[22] = 21;  // thumb_03_l -> thumb_02_l

	StaticData.BoneParents[23] = 13;  // index_01_l -> hand_l
	StaticData.BoneParents[24] = 23;  // index_02_l -> index_01_l
	StaticData.BoneParents[25] = 24;  // index_03_l -> index_02_l

	StaticData.BoneParents[26] = 13;  // middle_01_l -> hand_l
	StaticData.BoneParents[27] = 26;  // middle_02_l -> middle_01_l
	StaticData.BoneParents[28] = 27;  // middle_03_l -> middle_02_l

	StaticData.BoneParents[29] = 13;  // ring_01_l -> hand_l
	StaticData.BoneParents[30] = 29;  // ring_02_l -> ring_01_l
	StaticData.BoneParents[31] = 30;  // ring_03_l -> ring_02_l

	StaticData.BoneParents[32] = 13;  // pinky_01_l -> hand_l
	StaticData.BoneParents[33] = 32;  // pinky_02_l -> pinky_01_l
	StaticData.BoneParents[34] = 33;  // pinky_03_l -> pinky_02_l

	StaticData.BoneParents[35] = 19;  // thumb_01_r -> hand_rthis
	StaticData.BoneParents[36] = 35;  // thumb_02_r -> thumb_01_r
	StaticData.BoneParents[37] = 36;  // thumb_03_r -> thumb_02_r

	StaticData.BoneParents[38] = 19;  // index_01_r -> hand_r
	StaticData.BoneParents[39] = 38;  // index_02_r -> index_01_r
	StaticData.BoneParents[40] = 39;  // index_03_r -> index_02_r

	StaticData.BoneParents[41] = 19;  // middle_01_r -> hand_r
	StaticData.BoneParents[42] = 41;  // middle_02_r -> middle_01_r
	StaticData.BoneParents[43] = 42;  // middle_03_r -> middle_02_r

	StaticData.BoneParents[44] = 19;  // ring_01_r -> hand_r
	StaticData.BoneParents[45] = 44;  // ring_02_r -> ring_01_r
	StaticData.BoneParents[46] = 45;  // ring_03_r -> ring_02_r

	StaticData.BoneParents[47] = 19;  // pinky_01_r -> hand_r
	StaticData.BoneParents[48] = 47;  // pinky_02_r -> pinky_01_r
	StaticData.BoneParents[49] = 48;  // pinky_03_r -> pinky_02_r

	BoneParents = StaticData.BoneParents;

	/* Push Subject Data(Static) */
	Client->PushSubjectStaticData_AnyThread({ SourceGuid, SubjectName }, ULiveLinkAnimationRole::StaticClass(), MoveTemp(StaticDataStruct));
}

FString MoALiveLinkSource::ParseString(const uint8* DataPtr, int32& offset, int32 DataSize)
{
	if (offset >= DataSize)
	{
		return FString();
	}

	const char* StringStart = reinterpret_cast<const char*>(DataPtr + offset);

	FString result = FString(ANSI_TO_TCHAR(StringStart));

	int32 StringLength = result.Len() + 1;
	int32 PaddedLength = (StringLength + 3) & ~3;

	offset += PaddedLength;

	return result;
}

/* Byte Parsing, Endian */
void ParseBlobDataToFloats(const uint8* DataPtr, int32 BlobSize, TArray<float>& OutFloatArray)
{
	if (BlobSize % sizeof(float) != 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid Blob Size: %d. Must be a multiple of 4."), BlobSize);
		return;
	}

	const int32 NumFloats = BlobSize / sizeof(float);
	OutFloatArray.SetNumUninitialized(NumFloats);

	for (int32 i = 0; i < NumFloats; ++i)
	{
		float Value = *reinterpret_cast<const float*>(DataPtr + i * sizeof(float));
		OutFloatArray[i] = Value;
	}
}

#undef LOCTEXT_NAMESPACE