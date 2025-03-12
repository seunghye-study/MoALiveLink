// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "MoALiveLink/Public/MoALiveLinkSourceFactory.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeMoALiveLinkSourceFactory() {}
// Cross Module References
	LIVELINKINTERFACE_API UClass* Z_Construct_UClass_ULiveLinkSourceFactory();
	MOALIVELINK_API UClass* Z_Construct_UClass_UMoALiveLinkSourceFactory();
	MOALIVELINK_API UClass* Z_Construct_UClass_UMoALiveLinkSourceFactory_NoRegister();
	UPackage* Z_Construct_UPackage__Script_MoALiveLink();
// End Cross Module References
	void UMoALiveLinkSourceFactory::StaticRegisterNativesUMoALiveLinkSourceFactory()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UMoALiveLinkSourceFactory);
	UClass* Z_Construct_UClass_UMoALiveLinkSourceFactory_NoRegister()
	{
		return UMoALiveLinkSourceFactory::StaticClass();
	}
	struct Z_Construct_UClass_UMoALiveLinkSourceFactory_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UMoALiveLinkSourceFactory_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_ULiveLinkSourceFactory,
		(UObject* (*)())Z_Construct_UPackage__Script_MoALiveLink,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UMoALiveLinkSourceFactory_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "MoALiveLinkSourceFactory.h" },
		{ "ModuleRelativePath", "Public/MoALiveLinkSourceFactory.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UMoALiveLinkSourceFactory_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UMoALiveLinkSourceFactory>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UMoALiveLinkSourceFactory_Statics::ClassParams = {
		&UMoALiveLinkSourceFactory::StaticClass,
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
		METADATA_PARAMS(Z_Construct_UClass_UMoALiveLinkSourceFactory_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UMoALiveLinkSourceFactory_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UMoALiveLinkSourceFactory()
	{
		if (!Z_Registration_Info_UClass_UMoALiveLinkSourceFactory.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UMoALiveLinkSourceFactory.OuterSingleton, Z_Construct_UClass_UMoALiveLinkSourceFactory_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UMoALiveLinkSourceFactory.OuterSingleton;
	}
	template<> MOALIVELINK_API UClass* StaticClass<UMoALiveLinkSourceFactory>()
	{
		return UMoALiveLinkSourceFactory::StaticClass();
	}
	UMoALiveLinkSourceFactory::UMoALiveLinkSourceFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UMoALiveLinkSourceFactory);
	UMoALiveLinkSourceFactory::~UMoALiveLinkSourceFactory() {}
	struct Z_CompiledInDeferFile_FID_Users_motion1_Desktop_MoALiveLink_MoALiveLink_HostProject_Plugins_MoALiveLink_Source_MoALiveLink_Public_MoALiveLinkSourceFactory_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_motion1_Desktop_MoALiveLink_MoALiveLink_HostProject_Plugins_MoALiveLink_Source_MoALiveLink_Public_MoALiveLinkSourceFactory_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UMoALiveLinkSourceFactory, UMoALiveLinkSourceFactory::StaticClass, TEXT("UMoALiveLinkSourceFactory"), &Z_Registration_Info_UClass_UMoALiveLinkSourceFactory, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UMoALiveLinkSourceFactory), 1477199598U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_motion1_Desktop_MoALiveLink_MoALiveLink_HostProject_Plugins_MoALiveLink_Source_MoALiveLink_Public_MoALiveLinkSourceFactory_h_3402896701(TEXT("/Script/MoALiveLink"),
		Z_CompiledInDeferFile_FID_Users_motion1_Desktop_MoALiveLink_MoALiveLink_HostProject_Plugins_MoALiveLink_Source_MoALiveLink_Public_MoALiveLinkSourceFactory_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_motion1_Desktop_MoALiveLink_MoALiveLink_HostProject_Plugins_MoALiveLink_Source_MoALiveLink_Public_MoALiveLinkSourceFactory_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
