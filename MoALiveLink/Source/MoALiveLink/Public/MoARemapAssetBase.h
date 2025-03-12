//======================================================================================================
// Company Name:       MotionTechnologies.inc
// Copyright Year:     2025
// File:               MoARemapAssetBase.h
//======================================================================================================

#pragma once

#include "CoreMinimal.h"
#include "LiveLinkAnimationCore/Public/LiveLinkRemapAsset.h"
#include "MoARemapAssetBase.generated.h"

/**
 * 
 */
UCLASS()
class MOALIVELINK_API UMoARemapAssetBase : public ULiveLinkRemapAsset
{
	GENERATED_BODY()

public:

    virtual void BuildPoseFromAnimationData(float DeltaTime, const FLiveLinkSkeletonStaticData* InSkeletonData,
        const FLiveLinkAnimationFrameData* InFrameData,
        FCompactPose& OutPose) override;

protected:
    // Override these in child classes
    virtual FVector ConvertRootPosition(FVector LLPosition) const { return LLPosition; };
    virtual FQuat ConvertRootRotation(FQuat LLRotation) const { return LLRotation; };
    virtual FQuat ConvertBoneRotation(FQuat LLRotation) const { return LLRotation; };
    virtual FVector ConvertBonePosition(FVector LLPosition) const { return LLPosition; }
    virtual FName GetTargetRootName() const { return "root"; }

    TMap<FName, FName> BoneNameMap;

	
};
