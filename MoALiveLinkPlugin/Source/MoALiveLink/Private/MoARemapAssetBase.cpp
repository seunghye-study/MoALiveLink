//======================================================================================================
// Company Name:       MotionTechnologies.inc
// Copyright Year:     2025
// File:               MoARemapAssetBase.cpp
//======================================================================================================


#include "MoARemapAssetBase.h"
#include "Roles/LiveLinkAnimationTypes.h"
#include "BonePose.h"

void UMoARemapAssetBase::BuildPoseFromAnimationData(float DeltaTime, const FLiveLinkSkeletonStaticData* InSkeletonData, const FLiveLinkAnimationFrameData* InFrameData, FCompactPose& OutPose)
{
    const TArray<FName>& SourceBoneNames = InSkeletonData->BoneNames;

    TArray<FName, TMemStackAllocator<>> TransformedBoneNames;
    TransformedBoneNames.Reserve(SourceBoneNames.Num());

    // Find remapped bone names and cache them for fast subsequent retrieval.
    for (const FName& SrcBoneName : SourceBoneNames)
    {
        FName* TargetBoneName = BoneNameMap.Find(SrcBoneName);
        if (TargetBoneName == nullptr)
        {
            /* User will create a blueprint child class and implement this function using a switch statement. */
            FName NewName = GetRemappedBoneName(SrcBoneName);
            TransformedBoneNames.Add(NewName);
            BoneNameMap.Add(SrcBoneName, NewName);
        }
        else
        {
            TransformedBoneNames.Add(*TargetBoneName);
        }
    }

    // Iterate over remapped bone names, find the index of that bone on the skeleton, and apply the Live Link pose data.
    for (int32 i = 0; i < TransformedBoneNames.Num(); i++)
    {
        FName BoneName = TransformedBoneNames[i];
        
        FTransform BoneTransform = InFrameData->Transforms[i];
        const int32 MeshIndex = OutPose.GetBoneContainer().GetPoseBoneIndexForBoneName(BoneName);
        if (MeshIndex != INDEX_NONE)
        {
            FCompactPoseBoneIndex CPIndex = OutPose.GetBoneContainer().MakeCompactPoseIndex(
                FMeshPoseBoneIndex(MeshIndex));
            if (CPIndex != INDEX_NONE)
            {
                FQuat ConvertedLiveLinkRotation;
                // Only use position + rotation data for root. For all other bones, set rotation only.
                if (BoneName == GetTargetRootName())
                {
                    OutPose[CPIndex].SetLocation(ConvertRootPosition(BoneTransform.GetTranslation()));
                    ConvertedLiveLinkRotation = ConvertRootRotation(BoneTransform.GetRotation());
                }
                else
                {
                    ConvertedLiveLinkRotation = ConvertBoneRotation(BoneTransform.GetRotation());
                }
                // Retrieves the default reference pose for the skeleton. Live Link data contains relative transforms from the default pose.
                auto RefBoneTransform = OutPose.GetRefPose(CPIndex);

                OutPose[CPIndex].SetRotation(RefBoneTransform.GetRotation() * ConvertedLiveLinkRotation);
            }
        }
    }


}
