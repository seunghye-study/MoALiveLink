// Fill out your copyright notice in the Description page of Project Settings.


#include "MoALiveLinkSource.h"
#include "ILiveLinkClient.h"
#include "Roles/LiveLinkAnimationTypes.h"
#include "Roles/LiveLinkAnimationRole.h"
#include "Misc/ByteSwap.h"

#include "Common/UdpSocketBuilder.h"
#include <Core/Private/Modules/SimpleParse.h>

#define LOCTEXT_NAMESPACE "MoALiveLinkSource"
#define RECV_BUFFER_SIZE 1024 * 1024

void ParseBlobDataToFloats(const uint8* DataPtr, int32 BlobSize, TArray<float>& OutFloatArray);


MoALiveLinkSource::MoALiveLinkSource(FIPv4Endpoint Endpoint)
	:Socket(nullptr)
	, Stopping(false)
	, Thread(false)
	, WaitTime(FTimespan::FromMilliseconds(100))
{

	UE_LOG(LogTemp, Log, TEXT("MoALiveLinkSource created with Endpoint: %s"), *Endpoint.ToString());
	GlobalTransform.SetNumZeroed(51);
	// defaults
	DeviceEndpoint = Endpoint;

	SourceStatus = LOCTEXT("SourceStatus_DeviceNotFound", "Device Not Found");
	SourceType = LOCTEXT("MoALiveLinkSourceType", "MoA LiveLink");
	SourceMachineName = LOCTEXT("MoALiveLinkSourceMachineName", "localhost");

	// ��� IP���� �����͸� �����ϵ��� ���� ����
	Socket = FUdpSocketBuilder(TEXT("MoASocket"))
		.AsNonBlocking()
		.AsReusable()
		.BoundToAddress(FIPv4Address::Any) // ��� IP���� ����
		.BoundToPort(DeviceEndpoint.Port) // ��Ʈ ����
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
						// data�� ������ ���� �迭 ������
						TSharedPtr<TArray<uint8>, ESPMode::ThreadSafe> ReceivedData = MakeShareable(new TArray<uint8>());
						// ���ŵ� ������ ����
						// ������ �迭�� ũ�⸦ read��ŭ �����ϰ� �ʱ�ȭ �����Ͽ� ����ȭ
						ReceivedData->SetNumUninitialized(Read);
						memcpy(ReceivedData->GetData(), RecvBuffer.GetData(), Read);
						// �����忡�� ������ó�� (�Ľ�, ���̺긵ũ ���˺�ȯ �� Ǫ��)
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
	FName SubjectName ="MoAMotionCapture";
	//UE_LOG(LogTemp, Warning, TEXT("Received OSC data"));
	const uint8* DataPtr = ReceivedData->GetData();
	int32 DataSize = ReceivedData->Num();
	int32 Offset = 0;

	/* Address ����, /MOA/Stream/Motion/Unreal */
	FString AddressPattern = ParseString(DataPtr, Offset, DataSize);
	if (AddressPattern.IsEmpty() || AddressPattern != "/MOA/Steam/Motion/Unreal")
	{
		//UE_LOG(LogTemp, Warning, TEXT("Received empty OSC data"));
		return;
	}

	/* TypeTag ���� */
	FString TypeTags = ParseString(DataPtr, Offset, DataSize);
	if (TypeTags.IsEmpty() || TypeTags[0] != ',')
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid OSC Type Tags"));
		return;
	}

	/* bone data ���� */
	TArray<float> BoneDatas;
	for(int32 i = 0; i < TypeTags.Len(); i++)
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
			Offset += BlobSize; // BLOB ũ�⸸ŭ Offset �̵�
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

	const int32 NumBones = 50; // bone 50�� 

	/* static data init */
	bool bIsEncounter = !EncounterdSubjects.Contains(SubjectName);
	if (bIsEncounter)
	{
		UE_LOG(LogTemp, Error, TEXT("Static Data Frame Push"));
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
		FQuat BoneRotation(BoneDatas[boneOffset+3], BoneDatas[boneOffset + 4], BoneDatas[boneOffset + 5], BoneDatas[boneOffset + 6]);

		FTransform LocalTransform(BoneRotation, BonePosition, FVector(1.0f));
		FrameData.Transforms[i] = LocalTransform;
		boneOffset += 7;
	}

	FrameData.Transforms.Add(FTransform(FQuat::Identity, FVector::ZeroVector, FVector(1.0f)));

	/* Push Subject Data */
	Client->PushSubjectFrameData_AnyThread({ SourceGuid, SubjectName }, MoveTemp(FrameDataStruct));
}

void MoALiveLinkSource::InitStaticData(int32 NumBones, FName SubjectName)
{
	FLiveLinkStaticDataStruct StaticDataStruct = FLiveLinkStaticDataStruct(FLiveLinkSkeletonStaticData::StaticStruct());
	FLiveLinkSkeletonStaticData& StaticData = *StaticDataStruct.Cast<FLiveLinkSkeletonStaticData>();

	StaticData.BoneNames.SetNumUninitialized(NumBones + 1); //51��
	StaticData.BoneParents.SetNumUninitialized(NumBones + 1); // root bone ����

	for (int id = 0; id < NumBones; id++) // pelvis~hand_r
	{
		FString BoneName = SkeletalBoneNames[id];
		StaticData.BoneNames[id] = FName(BoneName);
		StaticData.BoneParents[id] = -1;
	}
	StaticData.BoneNames[50] = FName("root");
	StaticData.BoneParents[50] = -1;

	//StaticData.BoneParents[0] = 50;   // pelvis -> root
	//StaticData.BoneParents[1] = 0;   // thigh_l -> pelvis
	//StaticData.BoneParents[2] = 1;   // calf_l -> thigh
	//StaticData.BoneParents[3] = 2;   // foot_l -> calf

	//StaticData.BoneParents[4] = 0;  // thigh_r -> pelvis
	//StaticData.BoneParents[5] = 4;  // calf_r -> thigh
	//StaticData.BoneParents[6] = 5;  // foot -> calf

	//StaticData.BoneParents[7] = 0; // spine 01 -> pelvis
	//StaticData.BoneParents[8] = 7;  // spine 02 -> spine 01
	//StaticData.BoneParents[9] = 8; // spine 03 -> spine 02 

	//StaticData.BoneParents[10] = 9; // clavicle -> spine 03
	//StaticData.BoneParents[11] = 10; // upperarm l -> clavicle
	//StaticData.BoneParents[12] = 11; // lowerarm l -> upperarm
	//StaticData.BoneParents[13] = 12;  // hand l-> lowerarm

	//StaticData.BoneParents[14] = 9; // neck -> spine03
	//StaticData.BoneParents[15] = 14;  // head -> neck

	//StaticData.BoneParents[16] = 9; // clavicle r ->  spine03
	//StaticData.BoneParents[17] = 16; // rightupper -> clavicle 
	//StaticData.BoneParents[18] = 17; // right lowerarm -> right upper
	//StaticData.BoneParents[19] = 18; // hand r

	//StaticData.BoneParents[20] = 13;
	//StaticData.BoneParents[21] = 20;
	//StaticData.BoneParents[22] = 21;
	//StaticData.BoneParents[23] = 13;
	//StaticData.BoneParents[24] = 23;
	//StaticData.BoneParents[25] = 24;
	//StaticData.BoneParents[26] = 13;
	//StaticData.BoneParents[27] = 26;
	//StaticData.BoneParents[28] = 27;
	//StaticData.BoneParents[29] = 13;
	//StaticData.BoneParents[30] = 29;
	//StaticData.BoneParents[31] = 30;
	//StaticData.BoneParents[32] = 13;
	//StaticData.BoneParents[33] = 32;
	//StaticData.BoneParents[34] = 33;

	//StaticData.BoneParents[35] = 18;
	//StaticData.BoneParents[36] = 34;
	//StaticData.BoneParents[37] = 35;
	//StaticData.BoneParents[38] = 18;
	//StaticData.BoneParents[39] = 38;
	//StaticData.BoneParents[40] = 39;
	//StaticData.BoneParents[41] = 19;
	//StaticData.BoneParents[42] = 41;
	//StaticData.BoneParents[43] = 42;
	//StaticData.BoneParents[44] = 19;
	//StaticData.BoneParents[45] = 44;
	//StaticData.BoneParents[46] = 45;
	//StaticData.BoneParents[47] = 19;
	//StaticData.BoneParents[48] = 47;
	//StaticData.BoneParents[49] = 48;

	//BoneParents = StaticData.BoneParents;

	/* Push Subject Data(Static) */
	Client->PushSubjectStaticData_AnyThread({ SourceGuid, SubjectName }, ULiveLinkAnimationRole::StaticClass(), MoveTemp(StaticDataStruct));
}

// OSC ������ �и�
FString MoALiveLinkSource::ParseString(const uint8* DataPtr, int32& offset, int32 DataSize)
{
	// ������ �Ѿ�� �� ���ڿ� ����
	if (offset >= DataSize)
	{
		return FString();
	}

	// ���ڿ� ������ġ
	const char* StringStart = reinterpret_cast<const char*>(DataPtr + offset);

	// null terminated ���ڿ� -> FString
	FString result = FString(ANSI_TO_TCHAR(StringStart));

	// ���ڿ� ���̿� �е� ���
	int32 StringLength = result.Len() + 1; // null-terminated ����
	int32 PaddedLength = (StringLength + 3) & ~3; // 4����Ʈ ������ �е�

	// Offset�� �е��� ���̸�ŭ ����
	offset += PaddedLength;

	return result;
}

/* ����Ʈ �Ľ� �� ����� ��ȯ */
void ParseBlobDataToFloats(const uint8* DataPtr, int32 BlobSize, TArray<float>& OutFloatArray)
{
	// BLOB ũ�� ��ȿ�� Ȯ��
	if (BlobSize % sizeof(float) != 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid Blob Size: %d. Must be a multiple of 4."), BlobSize);
		return;
	}

	// float ���� ���
	const int32 NumFloats = BlobSize / sizeof(float);
	OutFloatArray.SetNumUninitialized(NumFloats);

	for (int32 i = 0; i < NumFloats; ++i)
	{
		float Value = *reinterpret_cast<const float*>(DataPtr + i * sizeof(float));
		OutFloatArray[i] = Value;
	}
}


#undef LOCTEXT_NAMESPACE