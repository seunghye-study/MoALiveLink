// Fill out your copyright notice in the Description page of Project Settings.


#include "MoAOSCReceiver.h"
#include "Common/UdpSocketBuilder.h"

#define RECV_BUFFER_SIZE 1024 * 1024

// Sets default values
AMoAOSCReceiver::AMoAOSCReceiver()
	:Socket(nullptr),
	SocketSubsystem(nullptr)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMoAOSCReceiver::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Log, TEXT("MoALiveLinkSource created with Endpoint: 0.0.0.0:8000"));

	// 모든 IP에서 데이터를 수신하도록 소켓 설정
	Socket = FUdpSocketBuilder(TEXT("MoASocket"))
		.AsNonBlocking()
		.AsReusable()
		.BoundToAddress(FIPv4Address::Any) // 모든 IP에서 수신
		.BoundToPort(8000) // 포트 지정
		.WithReceiveBufferSize(RECV_BUFFER_SIZE);

	RecvBuffer.SetNumUninitialized(RECV_BUFFER_SIZE);

	
	if ((Socket != nullptr) && (Socket->GetSocketType() == SOCKTYPE_Datagram))
	{
		
	}
}

void AMoAOSCReceiver::ProcessReceivedData(TSharedPtr<TArray<uint8>> ReceivedData)
{
	const uint8* DataPtr = ReceivedData->GetData();
	int32 DataSize = ReceivedData->Num();
	int32 Offset = 0;

	FString AddressPattern = ParseString(DataPtr, Offset, DataSize);
	if (AddressPattern.IsEmpty() || AddressPattern != "/MOA/Stream/Motion/Unreal/World")
	{
		return;
	}


	FString TypeTags = ParseString(DataPtr, Offset, DataSize);
	if (TypeTags.IsEmpty() || TypeTags[0] != ',')
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid OSC Type Tags"));
		return;
	}
}

void AMoAOSCReceiver::PushToActor()
{
}

FString AMoAOSCReceiver::ParseString(const uint8* DataPtr, int32& offset, int32 DataSize)
{
	if (offset >= DataSize)
	{
		return FString();
	}

	const char* StringStart = reinterpret_cast<const char*>(DataPtr + offset);

	FString result = FString(ANSI_TO_TCHAR(StringStart));

	int32 StringLength = result.Len() + 1; // null-terminated 포함
	int32 PaddedLength = (StringLength + 3) & ~3; // 4바이트 단위로 패딩

	offset += PaddedLength;

	return result;
}


// Called every frame
void AMoAOSCReceiver::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

