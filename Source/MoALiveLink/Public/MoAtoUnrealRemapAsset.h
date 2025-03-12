// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MoARemapAssetBase.h"
#include "MoAtoUnrealRemapAsset.generated.h"

UCLASS()
class MOALIVELINK_API UMoAtoUnrealRemapAsset : public UMoARemapAssetBase
{
	GENERATED_BODY()
protected:
    virtual FVector ConvertRootPosition(FVector LLPosition) const override;

    virtual FQuat ConvertRootRotation(FQuat LLRotation) const override;

    virtual FQuat ConvertBoneRotation(FQuat LLRotation) const override;

    virtual FName GetTargetRootName() const override { return "pelvis"; }
	
};
