// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeBatteryCollector_init() {}
	static FPackageRegistrationInfo Z_Registration_Info_UPackage__Script_BatteryCollector;
	FORCENOINLINE UPackage* Z_Construct_UPackage__Script_BatteryCollector()
	{
		if (!Z_Registration_Info_UPackage__Script_BatteryCollector.OuterSingleton)
		{
			static const UECodeGen_Private::FPackageParams PackageParams = {
				"/Script/BatteryCollector",
				nullptr,
				0,
				PKG_CompiledIn | 0x00000000,
				0x80BA0DB4,
				0xFB8BA040,
				METADATA_PARAMS(nullptr, 0)
			};
			UECodeGen_Private::ConstructUPackage(Z_Registration_Info_UPackage__Script_BatteryCollector.OuterSingleton, PackageParams);
		}
		return Z_Registration_Info_UPackage__Script_BatteryCollector.OuterSingleton;
	}
	static FRegisterCompiledInInfo Z_CompiledInDeferPackage_UPackage__Script_BatteryCollector(Z_Construct_UPackage__Script_BatteryCollector, TEXT("/Script/BatteryCollector"), Z_Registration_Info_UPackage__Script_BatteryCollector, CONSTRUCT_RELOAD_VERSION_INFO(FPackageReloadVersionInfo, 0x80BA0DB4, 0xFB8BA040));
PRAGMA_ENABLE_DEPRECATION_WARNINGS
