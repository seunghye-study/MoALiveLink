// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"
#include "MoAOSCReceiver.generated.h"


UCLASS()
class LIVELINK2_API AMoAOSCReceiver : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMoAOSCReceiver();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void ProcessReceivedData(TSharedPtr<TArray<uint8>> ReceivedData);
	void PushToActor();
	FString ParseString(const uint8* DataPtr, int32& offset, int32 DataSize);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	FText SocketAddress;
	FIPv4Endpoint DeviceEndpoint;
	FSocket* Socket;
	ISocketSubsystem* SocketSubsystem;

	TArray<uint8> RecvBuffer;
	FTimespan WaitTime;
};
