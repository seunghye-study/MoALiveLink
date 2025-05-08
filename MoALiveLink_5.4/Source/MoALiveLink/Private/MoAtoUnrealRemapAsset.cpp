// Fill out your copyright notice in the Description page of Project Settings.


#include "MoAtoUnrealRemapAsset.h"


FVector UMoAtoUnrealRemapAsset::ConvertRootPosition(FVector LLPosition) const
{
    return 100 * FVector(
        -LLPosition.X,
        LLPosition.Z,
        LLPosition.Y
    );

}

FQuat UMoAtoUnrealRemapAsset::ConvertRootRotation(FQuat LLRotation) const
{
    return FQuat(
        -LLRotation.Y,
        LLRotation.X,
        LLRotation.Z,
        LLRotation.W
    );
}

FQuat UMoAtoUnrealRemapAsset::ConvertBoneRotation(FQuat LLRotation) const
{
    return FQuat(
        -LLRotation.Y,
        LLRotation.Z,
        LLRotation.X,
        LLRotation.W
    );
}
