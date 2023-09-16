// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "BatteryCollector/BatteryPickup.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeBatteryPickup() {}
// Cross Module References
	BATTERYCOLLECTOR_API UClass* Z_Construct_UClass_ABatteryPickup();
	BATTERYCOLLECTOR_API UClass* Z_Construct_UClass_ABatteryPickup_NoRegister();
	BATTERYCOLLECTOR_API UClass* Z_Construct_UClass_APickup();
	UPackage* Z_Construct_UPackage__Script_BatteryCollector();
// End Cross Module References
	void ABatteryPickup::StaticRegisterNativesABatteryPickup()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(ABatteryPickup);
	UClass* Z_Construct_UClass_ABatteryPickup_NoRegister()
	{
		return ABatteryPickup::StaticClass();
	}
	struct Z_Construct_UClass_ABatteryPickup_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_BatteryPower_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_BatteryPower;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_ABatteryPickup_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_APickup,
		(UObject* (*)())Z_Construct_UPackage__Script_BatteryCollector,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ABatteryPickup_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * \n */" },
		{ "IncludePath", "BatteryPickup.h" },
		{ "ModuleRelativePath", "BatteryPickup.h" },
	};
#endif
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ABatteryPickup_Statics::NewProp_BatteryPower_MetaData[] = {
		{ "BlueprintProtected", "true" },
		{ "Category", "Power" },
		{ "Comment", "// \xc4\xb3\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xcd\xbf\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd \xef\xbf\xbd\xef\xbf\xbd power \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
		{ "ModuleRelativePath", "BatteryPickup.h" },
		{ "ToolTip", "\xc4\xb3\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xcd\xbf\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd \xef\xbf\xbd\xef\xbf\xbd power \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_ABatteryPickup_Statics::NewProp_BatteryPower = { "BatteryPower", nullptr, (EPropertyFlags)0x0020080000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(ABatteryPickup, BatteryPower), METADATA_PARAMS(Z_Construct_UClass_ABatteryPickup_Statics::NewProp_BatteryPower_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ABatteryPickup_Statics::NewProp_BatteryPower_MetaData)) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_ABatteryPickup_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ABatteryPickup_Statics::NewProp_BatteryPower,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_ABatteryPickup_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ABatteryPickup>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_ABatteryPickup_Statics::ClassParams = {
		&ABatteryPickup::StaticClass,
		"Engine",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_ABatteryPickup_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_ABatteryPickup_Statics::PropPointers),
		0,
		0x009000A4u,
		METADATA_PARAMS(Z_Construct_UClass_ABatteryPickup_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_ABatteryPickup_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_ABatteryPickup()
	{
		if (!Z_Registration_Info_UClass_ABatteryPickup.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_ABatteryPickup.OuterSingleton, Z_Construct_UClass_ABatteryPickup_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_ABatteryPickup.OuterSingleton;
	}
	template<> BATTERYCOLLECTOR_API UClass* StaticClass<ABatteryPickup>()
	{
		return ABatteryPickup::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(ABatteryPickup);
	ABatteryPickup::~ABatteryPickup() {}
	struct Z_CompiledInDeferFile_FID_Users_SeongGyu_Desktop_BatteryCollector_Source_BatteryCollector_BatteryPickup_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_SeongGyu_Desktop_BatteryCollector_Source_BatteryCollector_BatteryPickup_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_ABatteryPickup, ABatteryPickup::StaticClass, TEXT("ABatteryPickup"), &Z_Registration_Info_UClass_ABatteryPickup, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(ABatteryPickup), 2636496310U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_SeongGyu_Desktop_BatteryCollector_Source_BatteryCollector_BatteryPickup_h_2966500309(TEXT("/Script/BatteryCollector"),
		Z_CompiledInDeferFile_FID_Users_SeongGyu_Desktop_BatteryCollector_Source_BatteryCollector_BatteryPickup_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_SeongGyu_Desktop_BatteryCollector_Source_BatteryCollector_BatteryPickup_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
