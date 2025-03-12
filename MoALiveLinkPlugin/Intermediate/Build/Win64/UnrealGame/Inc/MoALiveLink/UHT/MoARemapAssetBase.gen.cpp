// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "MoALiveLink/Public/MoARemapAssetBase.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeMoARemapAssetBase() {}
// Cross Module References
	LIVELINKANIMATIONCORE_API UClass* Z_Construct_UClass_ULiveLinkRemapAsset();
	MOALIVELINK_API UClass* Z_Construct_UClass_UMoARemapAssetBase();
	MOALIVELINK_API UClass* Z_Construct_UClass_UMoARemapAssetBase_NoRegister();
	UPackage* Z_Construct_UPackage__Script_MoALiveLink();
// End Cross Module References
	void UMoARemapAssetBase::StaticRegisterNativesUMoARemapAssetBase()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UMoARemapAssetBase);
	UClass* Z_Construct_UClass_UMoARemapAssetBase_NoRegister()
	{
		return UMoARemapAssetBase::StaticClass();
	}
	struct Z_Construct_UClass_UMoARemapAssetBase_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UMoARemapAssetBase_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_ULiveLinkRemapAsset,
		(UObject* (*)())Z_Construct_UPackage__Script_MoALiveLink,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UMoARemapAssetBase_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * \n */" },
		{ "IncludePath", "MoARemapAssetBase.h" },
		{ "ModuleRelativePath", "Public/MoARemapAssetBase.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UMoARemapAssetBase_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UMoARemapAssetBase>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UMoARemapAssetBase_Statics::ClassParams = {
		&UMoARemapAssetBase::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x001000A0u,
		METADATA_PARAMS(Z_Construct_UClass_UMoARemapAssetBase_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UMoARemapAssetBase_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UMoARemapAssetBase()
	{
		if (!Z_Registration_Info_UClass_UMoARemapAssetBase.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UMoARemapAssetBase.OuterSingleton, Z_Construct_UClass_UMoARemapAssetBase_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UMoARemapAssetBase.OuterSingleton;
	}
	template<> MOALIVELINK_API UClass* StaticClass<UMoARemapAssetBase>()
	{
		return UMoARemapAssetBase::StaticClass();
	}
	UMoARemapAssetBase::UMoARemapAssetBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UMoARemapAssetBase);
	UMoARemapAssetBase::~UMoARemapAssetBase() {}
	struct Z_CompiledInDeferFile_FID_Users_motion1_Desktop_MoALiveLink_MoALiveLink_HostProject_Plugins_MoALiveLink_Source_MoALiveLink_Public_MoARemapAssetBase_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_motion1_Desktop_MoALiveLink_MoALiveLink_HostProject_Plugins_MoALiveLink_Source_MoALiveLink_Public_MoARemapAssetBase_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UMoARemapAssetBase, UMoARemapAssetBase::StaticClass, TEXT("UMoARemapAssetBase"), &Z_Registration_Info_UClass_UMoARemapAssetBase, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UMoARemapAssetBase), 4117383531U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_motion1_Desktop_MoALiveLink_MoALiveLink_HostProject_Plugins_MoALiveLink_Source_MoALiveLink_Public_MoARemapAssetBase_h_1809002424(TEXT("/Script/MoALiveLink"),
		Z_CompiledInDeferFile_FID_Users_motion1_Desktop_MoALiveLink_MoALiveLink_HostProject_Plugins_MoALiveLink_Source_MoALiveLink_Public_MoARemapAssetBase_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_motion1_Desktop_MoALiveLink_MoALiveLink_HostProject_Plugins_MoALiveLink_Source_MoALiveLink_Public_MoARemapAssetBase_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
