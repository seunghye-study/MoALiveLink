//======================================================================================================
// Company Name:       MotionTechnologies.inc
// Copyright Year:     2025
// File:               MoARemapAsset.h
//======================================================================================================

#pragma once

#include "CoreMinimal.h"
#include "MoARemapAssetBase.h"
#include "MoARemapAsset.generated.h"

/**
 * 
 */
UCLASS()
class MOALIVELINK_API UMoARemapAsset : public UMoARemapAssetBase
{
	GENERATED_BODY()
	
protected:
    virtual FVector ConvertRootPosition(FVector LLPosition) const override;

    virtual FQuat ConvertRootRotation(FQuat LLRotation) const override;

    virtual FQuat ConvertBoneRotation(FQuat LLRotation) const override;

    virtual FName GetTargetRootName() const override { return "Hips"; }
};
