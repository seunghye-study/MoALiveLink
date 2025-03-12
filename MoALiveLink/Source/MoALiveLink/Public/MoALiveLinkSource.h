//======================================================================================================
// Company Name:       MotionTechnologies.inc
// Copyright Year:     2025
// File:               MoALiveLinkSource.h
//======================================================================================================


#pragma once

#include "CoreMinimal.h"
#include "LiveLinkInterface/Public/ILiveLinkSource.h"
#include "HAL/Runnable.h"
#include "HAL/ThreadSafeBool.h"
#include "HAL/RunnableThread.h"
#include "Async/Async.h"

#include "IMessageContext.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"
#include "Sockets.h"
#include "SocketSubsystem.h"

class FRunnableThread;
class FSocket;
class ILiveLinkClient;
class ISocketSubsystem;

class MOALIVELINK_API MoALiveLinkSource : public ILiveLinkSource, public FRunnable
{
public:
	MoALiveLinkSource(FIPv4Endpoint Endpoint);
	virtual ~MoALiveLinkSource();

	/* ILiveLinkSource Interface */
	virtual void ReceiveClient(ILiveLinkClient* InClient, FGuid InSourceGuid) override;
	virtual bool IsSourceStillValid() const override;
	virtual bool RequestSourceShutdown() override;
	virtual FText GetSourceType() const override { return SourceType; };
	virtual FText GetSourceMachineName() const override { return SourceMachineName; }
	virtual FText GetSourceStatus() const override { return SourceStatus; }

	/* thread interface */
	virtual bool Init() override { return true; } // thread init
	virtual uint32 Run() override; // thread main loop
	void Start(); // thread start
	virtual void Stop() override; // thread stop
	virtual void Exit() override {} // memory clean

	/* Process OSC Message */
	void ProcessReceivedData(TSharedPtr<TArray<uint8>, ESPMode::ThreadSafe> ReceivedData);
	void InitStaticData(int32 NumBones, FName SubjectName);
	FString ParseString(const uint8* DataPtr, int32& offset, int32 DataSize);


private:
	ILiveLinkClient* Client;

	// Our identifier in LiveLink
	FGuid SourceGuid;

	FMessageAddress ConnectionAddress;

	FIPv4Endpoint DeviceEndpoint;

	FText SourceType;
	FText SourceMachineName;
	FText SourceStatus;

	// socket
	FSocket* Socket;

	// subsystem associated by socket
	ISocketSubsystem* SocketSubsystem;

	/* thread */
	FThreadSafeBool Stopping;
	FRunnableThread* Thread; // thread to run socket operations on

	// Name of socket thread
	FString ThreadName;

	// Time to wait between attempted receives
	FTimespan WaitTime;

	// list of subjects , we've already encountered
	TSet<FName> EncounterdSubjects;

	// socket data buffer
	TArray<uint8> RecvBuffer;

	TArray<int32> BoneParents; // 본 부모 배열

	TArray<FString> SkeletalBoneNames = { 
		"Hips",
		"LeftHip",
		"LeftKnee",
		"LeftAnkle",
		"RightHip",
		"RightKnee",
		"RightAnkle",
		"Chest",
		"Chest1",
		"Chest2",
		"LeftCollar",   
		"LeftShoulder", 
		"LeftElbow", 
		"LeftWrist", 
		"Neck",
		"Neck1",     
		"RightCollar",  
		"RightShoulder",
		"RightElbow",   
		"RightWrist",   

		// Left hand fingers
		"LeftFinger0",  
		"LeftFinger01", 
		"LeftFinger02", 
		"LeftFinger1",  
		"LeftFinger11", 
		"LeftFinger12", 
		"LeftFinger2",  
		"LeftFinger21", 
		"LeftFinger22", 
		"LeftFinger3",  
		"LeftFinger31", 
		"LeftFinger32", 
		"LeftFinger4",  
		"LeftFinger41", 
		"LeftFinger42", 

		// Right hand fingers
		"RightFinger0",  
		"RightFinger01", 
		"RightFinger02", 
		"RightFinger1",  
		"RightFinger11", 
		"RightFinger12", 
		"RightFinger2",  
		"RightFinger21", 
		"RightFinger22", 
		"RightFinger3",  
		"RightFinger31", 
		"RightFinger32", 
		"RightFinger4",  
		"RightFinger41", 
		"RightFinger42"
	};
};
