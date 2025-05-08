// Fill out your copyright notice in the Description page of Project Settings.


#include "MoARemapAsset.h"

FVector UMoARemapAsset::ConvertRootPosition(FVector LLPosition) const
{
    return 100 * FVector(
        -LLPosition.X,
        LLPosition.Z,
        LLPosition.Y
    );

}

FQuat UMoARemapAsset::ConvertRootRotation(FQuat LLRotation) const
{
    return FQuat(
        -LLRotation.X,
        -LLRotation.Y,
        LLRotation.Z,
        LLRotation.W
    );
}

FQuat UMoARemapAsset::ConvertBoneRotation(FQuat LLRotation) const
{
    return FQuat(
        -LLRotation.X,
        -LLRotation.Y,
        LLRotation.Z,
        LLRotation.W
    );
}
