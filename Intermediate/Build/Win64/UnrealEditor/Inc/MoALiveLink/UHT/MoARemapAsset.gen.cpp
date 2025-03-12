// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "MoALiveLink/Public/MoARemapAsset.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeMoARemapAsset() {}
// Cross Module References
	MOALIVELINK_API UClass* Z_Construct_UClass_UMoARemapAsset();
	MOALIVELINK_API UClass* Z_Construct_UClass_UMoARemapAsset_NoRegister();
	MOALIVELINK_API UClass* Z_Construct_UClass_UMoARemapAssetBase();
	UPackage* Z_Construct_UPackage__Script_MoALiveLink();
// End Cross Module References
	void UMoARemapAsset::StaticRegisterNativesUMoARemapAsset()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UMoARemapAsset);
	UClass* Z_Construct_UClass_UMoARemapAsset_NoRegister()
	{
		return UMoARemapAsset::StaticClass();
	}
	struct Z_Construct_UClass_UMoARemapAsset_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UMoARemapAsset_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UMoARemapAssetBase,
		(UObject* (*)())Z_Construct_UPackage__Script_MoALiveLink,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UMoARemapAsset_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * \n */" },
		{ "IncludePath", "MoARemapAsset.h" },
		{ "ModuleRelativePath", "Public/MoARemapAsset.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UMoARemapAsset_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UMoARemapAsset>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UMoARemapAsset_Statics::ClassParams = {
		&UMoARemapAsset::StaticClass,
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
		METADATA_PARAMS(Z_Construct_UClass_UMoARemapAsset_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UMoARemapAsset_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UMoARemapAsset()
	{
		if (!Z_Registration_Info_UClass_UMoARemapAsset.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UMoARemapAsset.OuterSingleton, Z_Construct_UClass_UMoARemapAsset_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UMoARemapAsset.OuterSingleton;
	}
	template<> MOALIVELINK_API UClass* StaticClass<UMoARemapAsset>()
	{
		return UMoARemapAsset::StaticClass();
	}
	UMoARemapAsset::UMoARemapAsset(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UMoARemapAsset);
	UMoARemapAsset::~UMoARemapAsset() {}
	struct Z_CompiledInDeferFile_FID_Users_motion1_Desktop_MoALiveLink_MoALiveLink_HostProject_Plugins_MoALiveLink_Source_MoALiveLink_Public_MoARemapAsset_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_motion1_Desktop_MoALiveLink_MoALiveLink_HostProject_Plugins_MoALiveLink_Source_MoALiveLink_Public_MoARemapAsset_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UMoARemapAsset, UMoARemapAsset::StaticClass, TEXT("UMoARemapAsset"), &Z_Registration_Info_UClass_UMoARemapAsset, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UMoARemapAsset), 2548697260U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_motion1_Desktop_MoALiveLink_MoALiveLink_HostProject_Plugins_MoALiveLink_Source_MoALiveLink_Public_MoARemapAsset_h_361013756(TEXT("/Script/MoALiveLink"),
		Z_CompiledInDeferFile_FID_Users_motion1_Desktop_MoALiveLink_MoALiveLink_HostProject_Plugins_MoALiveLink_Source_MoALiveLink_Public_MoARemapAsset_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_motion1_Desktop_MoALiveLink_MoALiveLink_HostProject_Plugins_MoALiveLink_Source_MoALiveLink_Public_MoARemapAsset_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
