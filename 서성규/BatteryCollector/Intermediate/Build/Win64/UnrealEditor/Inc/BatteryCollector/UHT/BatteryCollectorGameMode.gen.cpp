// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "BatteryCollector/BatteryCollectorGameMode.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeBatteryCollectorGameMode() {}
// Cross Module References
	BATTERYCOLLECTOR_API UClass* Z_Construct_UClass_ABatteryCollectorGameMode();
	BATTERYCOLLECTOR_API UClass* Z_Construct_UClass_ABatteryCollectorGameMode_NoRegister();
	BATTERYCOLLECTOR_API UEnum* Z_Construct_UEnum_BatteryCollector_EBatteryPlayState();
	COREUOBJECT_API UClass* Z_Construct_UClass_UClass();
	ENGINE_API UClass* Z_Construct_UClass_AGameModeBase();
	UMG_API UClass* Z_Construct_UClass_UUserWidget_NoRegister();
	UPackage* Z_Construct_UPackage__Script_BatteryCollector();
// End Cross Module References
	static FEnumRegistrationInfo Z_Registration_Info_UEnum_EBatteryPlayState;
	static UEnum* EBatteryPlayState_StaticEnum()
	{
		if (!Z_Registration_Info_UEnum_EBatteryPlayState.OuterSingleton)
		{
			Z_Registration_Info_UEnum_EBatteryPlayState.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_BatteryCollector_EBatteryPlayState, (UObject*)Z_Construct_UPackage__Script_BatteryCollector(), TEXT("EBatteryPlayState"));
		}
		return Z_Registration_Info_UEnum_EBatteryPlayState.OuterSingleton;
	}
	template<> BATTERYCOLLECTOR_API UEnum* StaticEnum<EBatteryPlayState>()
	{
		return EBatteryPlayState_StaticEnum();
	}
	struct Z_Construct_UEnum_BatteryCollector_EBatteryPlayState_Statics
	{
		static const UECodeGen_Private::FEnumeratorParam Enumerators[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[];
#endif
		static const UECodeGen_Private::FEnumParams EnumParams;
	};
	const UECodeGen_Private::FEnumeratorParam Z_Construct_UEnum_BatteryCollector_EBatteryPlayState_Statics::Enumerators[] = {
		{ "EBatteryPlayState::EPlaying", (int64)EBatteryPlayState::EPlaying },
		{ "EBatteryPlayState::EGameOver", (int64)EBatteryPlayState::EGameOver },
		{ "EBatteryPlayState::EWon", (int64)EBatteryPlayState::EWon },
		{ "EBatteryPlayState::EUnknown", (int64)EBatteryPlayState::EUnknown },
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UEnum_BatteryCollector_EBatteryPlayState_Statics::Enum_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "EGameOver.Name", "EBatteryPlayState::EGameOver" },
		{ "EPlaying.Name", "EBatteryPlayState::EPlaying" },
		{ "EUnknown.Name", "EBatteryPlayState::EUnknown" },
		{ "EWon.Name", "EBatteryPlayState::EWon" },
		{ "ModuleRelativePath", "BatteryCollectorGameMode.h" },
	};
#endif
	const UECodeGen_Private::FEnumParams Z_Construct_UEnum_BatteryCollector_EBatteryPlayState_Statics::EnumParams = {
		(UObject*(*)())Z_Construct_UPackage__Script_BatteryCollector,
		nullptr,
		"EBatteryPlayState",
		"EBatteryPlayState",
		Z_Construct_UEnum_BatteryCollector_EBatteryPlayState_Statics::Enumerators,
		UE_ARRAY_COUNT(Z_Construct_UEnum_BatteryCollector_EBatteryPlayState_Statics::Enumerators),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EEnumFlags::None,
		(uint8)UEnum::ECppForm::EnumClass,
		METADATA_PARAMS(Z_Construct_UEnum_BatteryCollector_EBatteryPlayState_Statics::Enum_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UEnum_BatteryCollector_EBatteryPlayState_Statics::Enum_MetaDataParams))
	};
	UEnum* Z_Construct_UEnum_BatteryCollector_EBatteryPlayState()
	{
		if (!Z_Registration_Info_UEnum_EBatteryPlayState.InnerSingleton)
		{
			UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_EBatteryPlayState.InnerSingleton, Z_Construct_UEnum_BatteryCollector_EBatteryPlayState_Statics::EnumParams);
		}
		return Z_Registration_Info_UEnum_EBatteryPlayState.InnerSingleton;
	}
	DEFINE_FUNCTION(ABatteryCollectorGameMode::execGetCurrentState)
	{
		P_FINISH;
		P_NATIVE_BEGIN;
		*(EBatteryPlayState*)Z_Param__Result=P_THIS->GetCurrentState();
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(ABatteryCollectorGameMode::execGetPowerToWin)
	{
		P_FINISH;
		P_NATIVE_BEGIN;
		*(float*)Z_Param__Result=P_THIS->GetPowerToWin();
		P_NATIVE_END;
	}
	void ABatteryCollectorGameMode::StaticRegisterNativesABatteryCollectorGameMode()
	{
		UClass* Class = ABatteryCollectorGameMode::StaticClass();
		static const FNameNativePtrPair Funcs[] = {
			{ "GetCurrentState", &ABatteryCollectorGameMode::execGetCurrentState },
			{ "GetPowerToWin", &ABatteryCollectorGameMode::execGetPowerToWin },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
	}
	struct Z_Construct_UFunction_ABatteryCollectorGameMode_GetCurrentState_Statics
	{
		struct BatteryCollectorGameMode_eventGetCurrentState_Parms
		{
			EBatteryPlayState ReturnValue;
		};
		static const UECodeGen_Private::FBytePropertyParams NewProp_ReturnValue_Underlying;
		static const UECodeGen_Private::FEnumPropertyParams NewProp_ReturnValue;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_ABatteryCollectorGameMode_GetCurrentState_Statics::NewProp_ReturnValue_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, 0, nullptr, METADATA_PARAMS(nullptr, 0) };
	const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_ABatteryCollectorGameMode_GetCurrentState_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(BatteryCollectorGameMode_eventGetCurrentState_Parms, ReturnValue), Z_Construct_UEnum_BatteryCollector_EBatteryPlayState, METADATA_PARAMS(nullptr, 0) }; // 576694884
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ABatteryCollectorGameMode_GetCurrentState_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ABatteryCollectorGameMode_GetCurrentState_Statics::NewProp_ReturnValue_Underlying,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ABatteryCollectorGameMode_GetCurrentState_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_ABatteryCollectorGameMode_GetCurrentState_Statics::Function_MetaDataParams[] = {
		{ "Category", "Power" },
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd \xef\xbf\xbd\xc3\xb7\xef\xbf\xbd\xef\xbf\xbd\xcc\xbb\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd \xef\xbf\xbd\xef\xbf\xbd\xc8\xaf\n" },
		{ "ModuleRelativePath", "BatteryCollectorGameMode.h" },
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd \xef\xbf\xbd\xc3\xb7\xef\xbf\xbd\xef\xbf\xbd\xcc\xbb\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd \xef\xbf\xbd\xef\xbf\xbd\xc8\xaf" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ABatteryCollectorGameMode_GetCurrentState_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_ABatteryCollectorGameMode, nullptr, "GetCurrentState", nullptr, nullptr, sizeof(Z_Construct_UFunction_ABatteryCollectorGameMode_GetCurrentState_Statics::BatteryCollectorGameMode_eventGetCurrentState_Parms), Z_Construct_UFunction_ABatteryCollectorGameMode_GetCurrentState_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ABatteryCollectorGameMode_GetCurrentState_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_ABatteryCollectorGameMode_GetCurrentState_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_ABatteryCollectorGameMode_GetCurrentState_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_ABatteryCollectorGameMode_GetCurrentState()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ABatteryCollectorGameMode_GetCurrentState_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_ABatteryCollectorGameMode_GetPowerToWin_Statics
	{
		struct BatteryCollectorGameMode_eventGetPowerToWin_Parms
		{
			float ReturnValue;
		};
		static const UECodeGen_Private::FFloatPropertyParams NewProp_ReturnValue;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_ABatteryCollectorGameMode_GetPowerToWin_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(BatteryCollectorGameMode_eventGetPowerToWin_Parms, ReturnValue), METADATA_PARAMS(nullptr, 0) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_ABatteryCollectorGameMode_GetPowerToWin_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_ABatteryCollectorGameMode_GetPowerToWin_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_ABatteryCollectorGameMode_GetPowerToWin_Statics::Function_MetaDataParams[] = {
		{ "Category", "Power" },
		{ "Comment", "// powertowin getter \xef\xbf\xbd\xd4\xbc\xef\xbf\xbd\n" },
		{ "ModuleRelativePath", "BatteryCollectorGameMode.h" },
		{ "ToolTip", "powertowin getter \xef\xbf\xbd\xd4\xbc\xef\xbf\xbd" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_ABatteryCollectorGameMode_GetPowerToWin_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_ABatteryCollectorGameMode, nullptr, "GetPowerToWin", nullptr, nullptr, sizeof(Z_Construct_UFunction_ABatteryCollectorGameMode_GetPowerToWin_Statics::BatteryCollectorGameMode_eventGetPowerToWin_Parms), Z_Construct_UFunction_ABatteryCollectorGameMode_GetPowerToWin_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_ABatteryCollectorGameMode_GetPowerToWin_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x54020401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_ABatteryCollectorGameMode_GetPowerToWin_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_ABatteryCollectorGameMode_GetPowerToWin_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_ABatteryCollectorGameMode_GetPowerToWin()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_ABatteryCollectorGameMode_GetPowerToWin_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(ABatteryCollectorGameMode);
	UClass* Z_Construct_UClass_ABatteryCollectorGameMode_NoRegister()
	{
		return ABatteryCollectorGameMode::StaticClass();
	}
	struct Z_Construct_UClass_ABatteryCollectorGameMode_Statics
	{
		static UObject* (*const DependentSingletons[])();
		static const FClassFunctionLinkInfo FuncInfo[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_DecayRate_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_DecayRate;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_PowerToWin_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_PowerToWin;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_HUDWidgetClass_MetaData[];
#endif
		static const UECodeGen_Private::FClassPropertyParams NewProp_HUDWidgetClass;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_CurrentWidget_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_CurrentWidget;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_ABatteryCollectorGameMode_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AGameModeBase,
		(UObject* (*)())Z_Construct_UPackage__Script_BatteryCollector,
	};
	const FClassFunctionLinkInfo Z_Construct_UClass_ABatteryCollectorGameMode_Statics::FuncInfo[] = {
		{ &Z_Construct_UFunction_ABatteryCollectorGameMode_GetCurrentState, "GetCurrentState" }, // 793674392
		{ &Z_Construct_UFunction_ABatteryCollectorGameMode_GetPowerToWin, "GetPowerToWin" }, // 410226754
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ABatteryCollectorGameMode_Statics::Class_MetaDataParams[] = {
		{ "HideCategories", "Info Rendering MovementReplication Replication Actor Input Movement Collision Rendering HLOD WorldPartition DataLayers Transformation" },
		{ "IncludePath", "BatteryCollectorGameMode.h" },
		{ "ModuleRelativePath", "BatteryCollectorGameMode.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
#endif
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ABatteryCollectorGameMode_Statics::NewProp_DecayRate_MetaData[] = {
		{ "BlueprintProtect", "true" },
		{ "Category", "Power" },
		{ "Comment", "// \xc4\xb3\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xcd\xb0\xef\xbf\xbd \xef\xbf\xbd\xc4\xbf\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
		{ "ModuleRelativePath", "BatteryCollectorGameMode.h" },
		{ "ToolTip", "\xc4\xb3\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xcd\xb0\xef\xbf\xbd \xef\xbf\xbd\xc4\xbf\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_ABatteryCollectorGameMode_Statics::NewProp_DecayRate = { "DecayRate", nullptr, (EPropertyFlags)0x0020080000010005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(ABatteryCollectorGameMode, DecayRate), METADATA_PARAMS(Z_Construct_UClass_ABatteryCollectorGameMode_Statics::NewProp_DecayRate_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ABatteryCollectorGameMode_Statics::NewProp_DecayRate_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ABatteryCollectorGameMode_Statics::NewProp_PowerToWin_MetaData[] = {
		{ "BlueprintProtect", "true" },
		{ "Category", "Power" },
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xd3\xbf\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd \xef\xbf\xbd\xcc\xb1\xef\xbf\xbd\xef\xbf\xbd \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd \xef\xbf\xbd\xc4\xbf\xef\xbf\xbd\n" },
		{ "ModuleRelativePath", "BatteryCollectorGameMode.h" },
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xd3\xbf\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd \xef\xbf\xbd\xcc\xb1\xef\xbf\xbd\xef\xbf\xbd \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd \xef\xbf\xbd\xc4\xbf\xef\xbf\xbd" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_ABatteryCollectorGameMode_Statics::NewProp_PowerToWin = { "PowerToWin", nullptr, (EPropertyFlags)0x0020080000010005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(ABatteryCollectorGameMode, PowerToWin), METADATA_PARAMS(Z_Construct_UClass_ABatteryCollectorGameMode_Statics::NewProp_PowerToWin_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ABatteryCollectorGameMode_Statics::NewProp_PowerToWin_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ABatteryCollectorGameMode_Statics::NewProp_HUDWidgetClass_MetaData[] = {
		{ "BlueprintProtect", "true" },
		{ "Category", "Power" },
		{ "Comment", "// HUD \xc8\xad\xef\xbf\xbd\xe9\xbf\xa1\xef\xbf\xbd\xef\xbf\xbd \xef\xbf\xbd\xef\xbf\xbd \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd \xc5\xac\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n" },
		{ "ModuleRelativePath", "BatteryCollectorGameMode.h" },
		{ "ToolTip", "HUD \xc8\xad\xef\xbf\xbd\xe9\xbf\xa1\xef\xbf\xbd\xef\xbf\xbd \xef\xbf\xbd\xef\xbf\xbd \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd \xc5\xac\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
	};
#endif
	const UECodeGen_Private::FClassPropertyParams Z_Construct_UClass_ABatteryCollectorGameMode_Statics::NewProp_HUDWidgetClass = { "HUDWidgetClass", nullptr, (EPropertyFlags)0x0024080000010005, UECodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(ABatteryCollectorGameMode, HUDWidgetClass), Z_Construct_UClass_UClass, Z_Construct_UClass_UUserWidget_NoRegister, METADATA_PARAMS(Z_Construct_UClass_ABatteryCollectorGameMode_Statics::NewProp_HUDWidgetClass_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ABatteryCollectorGameMode_Statics::NewProp_HUDWidgetClass_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ABatteryCollectorGameMode_Statics::NewProp_CurrentWidget_MetaData[] = {
		{ "Comment", "// HUD \xef\xbf\xbd\xef\xbf\xbd\xc3\xbc\n" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "BatteryCollectorGameMode.h" },
		{ "ToolTip", "HUD \xef\xbf\xbd\xef\xbf\xbd\xc3\xbc" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ABatteryCollectorGameMode_Statics::NewProp_CurrentWidget = { "CurrentWidget", nullptr, (EPropertyFlags)0x0020080000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(ABatteryCollectorGameMode, CurrentWidget), Z_Construct_UClass_UUserWidget_NoRegister, METADATA_PARAMS(Z_Construct_UClass_ABatteryCollectorGameMode_Statics::NewProp_CurrentWidget_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_ABatteryCollectorGameMode_Statics::NewProp_CurrentWidget_MetaData)) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_ABatteryCollectorGameMode_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ABatteryCollectorGameMode_Statics::NewProp_DecayRate,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ABatteryCollectorGameMode_Statics::NewProp_PowerToWin,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ABatteryCollectorGameMode_Statics::NewProp_HUDWidgetClass,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ABatteryCollectorGameMode_Statics::NewProp_CurrentWidget,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_ABatteryCollectorGameMode_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ABatteryCollectorGameMode>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_ABatteryCollectorGameMode_Statics::ClassParams = {
		&ABatteryCollectorGameMode::StaticClass,
		"Game",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		FuncInfo,
		Z_Construct_UClass_ABatteryCollectorGameMode_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		UE_ARRAY_COUNT(FuncInfo),
		UE_ARRAY_COUNT(Z_Construct_UClass_ABatteryCollectorGameMode_Statics::PropPointers),
		0,
		0x008802ACu,
		METADATA_PARAMS(Z_Construct_UClass_ABatteryCollectorGameMode_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_ABatteryCollectorGameMode_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_ABatteryCollectorGameMode()
	{
		if (!Z_Registration_Info_UClass_ABatteryCollectorGameMode.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_ABatteryCollectorGameMode.OuterSingleton, Z_Construct_UClass_ABatteryCollectorGameMode_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_ABatteryCollectorGameMode.OuterSingleton;
	}
	template<> BATTERYCOLLECTOR_API UClass* StaticClass<ABatteryCollectorGameMode>()
	{
		return ABatteryCollectorGameMode::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(ABatteryCollectorGameMode);
	ABatteryCollectorGameMode::~ABatteryCollectorGameMode() {}
	struct Z_CompiledInDeferFile_FID_Users_SeongGyu_Desktop_BatteryCollector_Source_BatteryCollector_BatteryCollectorGameMode_h_Statics
	{
		static const FEnumRegisterCompiledInInfo EnumInfo[];
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FEnumRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_SeongGyu_Desktop_BatteryCollector_Source_BatteryCollector_BatteryCollectorGameMode_h_Statics::EnumInfo[] = {
		{ EBatteryPlayState_StaticEnum, TEXT("EBatteryPlayState"), &Z_Registration_Info_UEnum_EBatteryPlayState, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 576694884U) },
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_SeongGyu_Desktop_BatteryCollector_Source_BatteryCollector_BatteryCollectorGameMode_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_ABatteryCollectorGameMode, ABatteryCollectorGameMode::StaticClass, TEXT("ABatteryCollectorGameMode"), &Z_Registration_Info_UClass_ABatteryCollectorGameMode, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(ABatteryCollectorGameMode), 1833420165U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_SeongGyu_Desktop_BatteryCollector_Source_BatteryCollector_BatteryCollectorGameMode_h_4099288592(TEXT("/Script/BatteryCollector"),
		Z_CompiledInDeferFile_FID_Users_SeongGyu_Desktop_BatteryCollector_Source_BatteryCollector_BatteryCollectorGameMode_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_SeongGyu_Desktop_BatteryCollector_Source_BatteryCollector_BatteryCollectorGameMode_h_Statics::ClassInfo),
		nullptr, 0,
		Z_CompiledInDeferFile_FID_Users_SeongGyu_Desktop_BatteryCollector_Source_BatteryCollector_BatteryCollectorGameMode_h_Statics::EnumInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_SeongGyu_Desktop_BatteryCollector_Source_BatteryCollector_BatteryCollectorGameMode_h_Statics::EnumInfo));
PRAGMA_ENABLE_DEPRECATION_WARNINGS
