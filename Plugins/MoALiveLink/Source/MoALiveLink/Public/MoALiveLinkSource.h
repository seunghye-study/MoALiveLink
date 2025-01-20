// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LiveLinkInterface/Public/ILiveLinkSource.h"
#include "HAL/Runnable.h"
#include "HAL/ThreadSafeBool.h"
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

	/* 기타 */
	TArray<int32> BoneParents; // 본 부모 배열
	TArray<FTransform> GlobalTransform;
	TArray<FString> SkeletalBoneNames = { // 50개 + 손가락 본 이름 추가
	"pelvis",       // hip
	"thigh_l",      // LeftUpperLeg
	"calf_l",       // LeftLowerLeg
	"foot_l",       // leftFoot
	"thigh_r",      // RightUpperLeg
	"calf_r",       // RightLowerLeg
	"foot_r",       // RightFoot
	"spine_01",     // Spine
	"spine_02",     // Chest
	"spine_03",     // UpperChest
	"clavicle_l",   // LeftShoulder
	"upperarm_l",   // LeftUpperArm
	"lowerarm_l",   // LeftLowerArm
	"hand_l",       // LeftHand
	"neck_01",      // Neck
	"head",         // Head
	"clavicle_r",   // RightShoulder
	"upperarm_r",   // RightUpperArm
	"lowerarm_r",   // RightLowerArm
	"hand_r",       // RightHand

	// Left hand fingers
	"thumb_01_l",   // LeftThumbProximal
	"thumb_02_l",   // LeftThumbIntermediate
	"thumb_03_l",   // LeftThumbDistal
	"index_01_l",   // LeftIndexProximal
	"index_02_l",   // LeftIndexIntermediate
	"index_03_l",   // LeftIndexDistal
	"middle_01_l",  // LeftMiddleProximal
	"middle_02_l",  // LeftMiddleIntermediate
	"middle_03_l",  // LeftMiddleDistal
	"ring_01_l",    // LeftRingProximal
	"ring_02_l",    // LeftRingIntermediate
	"ring_03_l",    // LeftRingDistal
	"pinky_01_l",   // LeftLittleProximal
	"pinky_02_l",   // LeftLittleIntermediate
	"pinky_03_l",   // LeftLittleDistal

	// Right hand fingers
	"thumb_01_r",   // RightThumbProximal
	"thumb_02_r",   // RightThumbIntermediate
	"thumb_03_r",   // RightThumbDistal
	"index_01_r",   // RightIndexProximal
	"index_02_r",   // RightIndexIntermediate
	"index_03_r",   // RightIndexDistal
	"middle_01_r",  // RightMiddleProximal
	"middle_02_r",  // RightMiddleIntermediate
	"ring_01_r",    // RightRingProximal
	"ring_02_r",    // RightRingIntermediate
	"ring_03_r",    // RightRingDistal
	"pinky_01_r",   // RightLittleProximal
	"pinky_02_r",   // RightLittleIntermediate
	"pinky_03_r",    // RightLittleDistal
	"middle_03_r",  // RightMiddleDistal
	"root"
	};


	TArray<FString> SkeletalBoneNamesUnity = { 
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
		"Neck1",     
		"Head",         
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
