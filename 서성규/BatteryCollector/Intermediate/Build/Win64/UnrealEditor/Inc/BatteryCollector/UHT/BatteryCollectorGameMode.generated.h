// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "BatteryCollectorGameMode.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
enum class EBatteryPlayState : uint8;
#ifdef BATTERYCOLLECTOR_BatteryCollectorGameMode_generated_h
#error "BatteryCollectorGameMode.generated.h already included, missing '#pragma once' in BatteryCollectorGameMode.h"
#endif
#define BATTERYCOLLECTOR_BatteryCollectorGameMode_generated_h

#define FID_Users_SeongGyu_Desktop_BatteryCollector_Source_BatteryCollector_BatteryCollectorGameMode_h_21_SPARSE_DATA
#define FID_Users_SeongGyu_Desktop_BatteryCollector_Source_BatteryCollector_BatteryCollectorGameMode_h_21_RPC_WRAPPERS \
 \
	DECLARE_FUNCTION(execGetCurrentState); \
	DECLARE_FUNCTION(execGetPowerToWin);


#define FID_Users_SeongGyu_Desktop_BatteryCollector_Source_BatteryCollector_BatteryCollectorGameMode_h_21_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execGetCurrentState); \
	DECLARE_FUNCTION(execGetPowerToWin);


#define FID_Users_SeongGyu_Desktop_BatteryCollector_Source_BatteryCollector_BatteryCollectorGameMode_h_21_ACCESSORS
#define FID_Users_SeongGyu_Desktop_BatteryCollector_Source_BatteryCollector_BatteryCollectorGameMode_h_21_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesABatteryCollectorGameMode(); \
	friend struct Z_Construct_UClass_ABatteryCollectorGameMode_Statics; \
public: \
	DECLARE_CLASS(ABatteryCollectorGameMode, AGameModeBase, COMPILED_IN_FLAGS(0 | CLASS_Transient | CLASS_Config), CASTCLASS_None, TEXT("/Script/BatteryCollector"), BATTERYCOLLECTOR_API) \
	DECLARE_SERIALIZER(ABatteryCollectorGameMode)


#define FID_Users_SeongGyu_Desktop_BatteryCollector_Source_BatteryCollector_BatteryCollectorGameMode_h_21_INCLASS \
private: \
	static void StaticRegisterNativesABatteryCollectorGameMode(); \
	friend struct Z_Construct_UClass_ABatteryCollectorGameMode_Statics; \
public: \
	DECLARE_CLASS(ABatteryCollectorGameMode, AGameModeBase, COMPILED_IN_FLAGS(0 | CLASS_Transient | CLASS_Config), CASTCLASS_None, TEXT("/Script/BatteryCollector"), BATTERYCOLLECTOR_API) \
	DECLARE_SERIALIZER(ABatteryCollectorGameMode)


#define FID_Users_SeongGyu_Desktop_BatteryCollector_Source_BatteryCollector_BatteryCollectorGameMode_h_21_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	BATTERYCOLLECTOR_API ABatteryCollectorGameMode(const FObjectInitializer& ObjectInitializer); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(ABatteryCollectorGameMode) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(BATTERYCOLLECTOR_API, ABatteryCollectorGameMode); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(ABatteryCollectorGameMode); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	BATTERYCOLLECTOR_API ABatteryCollectorGameMode(ABatteryCollectorGameMode&&); \
	BATTERYCOLLECTOR_API ABatteryCollectorGameMode(const ABatteryCollectorGameMode&); \
public: \
	BATTERYCOLLECTOR_API virtual ~ABatteryCollectorGameMode();


#define FID_Users_SeongGyu_Desktop_BatteryCollector_Source_BatteryCollector_BatteryCollectorGameMode_h_21_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	BATTERYCOLLECTOR_API ABatteryCollectorGameMode(ABatteryCollectorGameMode&&); \
	BATTERYCOLLECTOR_API ABatteryCollectorGameMode(const ABatteryCollectorGameMode&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(BATTERYCOLLECTOR_API, ABatteryCollectorGameMode); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(ABatteryCollectorGameMode); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(ABatteryCollectorGameMode) \
	BATTERYCOLLECTOR_API virtual ~ABatteryCollectorGameMode();


#define FID_Users_SeongGyu_Desktop_BatteryCollector_Source_BatteryCollector_BatteryCollectorGameMode_h_18_PROLOG
#define FID_Users_SeongGyu_Desktop_BatteryCollector_Source_BatteryCollector_BatteryCollectorGameMode_h_21_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Users_SeongGyu_Desktop_BatteryCollector_Source_BatteryCollector_BatteryCollectorGameMode_h_21_SPARSE_DATA \
	FID_Users_SeongGyu_Desktop_BatteryCollector_Source_BatteryCollector_BatteryCollectorGameMode_h_21_RPC_WRAPPERS \
	FID_Users_SeongGyu_Desktop_BatteryCollector_Source_BatteryCollector_BatteryCollectorGameMode_h_21_ACCESSORS \
	FID_Users_SeongGyu_Desktop_BatteryCollector_Source_BatteryCollector_BatteryCollectorGameMode_h_21_INCLASS \
	FID_Users_SeongGyu_Desktop_BatteryCollector_Source_BatteryCollector_BatteryCollectorGameMode_h_21_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define FID_Users_SeongGyu_Desktop_BatteryCollector_Source_BatteryCollector_BatteryCollectorGameMode_h_21_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Users_SeongGyu_Desktop_BatteryCollector_Source_BatteryCollector_BatteryCollectorGameMode_h_21_SPARSE_DATA \
	FID_Users_SeongGyu_Desktop_BatteryCollector_Source_BatteryCollector_BatteryCollectorGameMode_h_21_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_Users_SeongGyu_Desktop_BatteryCollector_Source_BatteryCollector_BatteryCollectorGameMode_h_21_ACCESSORS \
	FID_Users_SeongGyu_Desktop_BatteryCollector_Source_BatteryCollector_BatteryCollectorGameMode_h_21_INCLASS_NO_PURE_DECLS \
	FID_Users_SeongGyu_Desktop_BatteryCollector_Source_BatteryCollector_BatteryCollectorGameMode_h_21_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> BATTERYCOLLECTOR_API UClass* StaticClass<class ABatteryCollectorGameMode>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Users_SeongGyu_Desktop_BatteryCollector_Source_BatteryCollector_BatteryCollectorGameMode_h


#define FOREACH_ENUM_EBATTERYPLAYSTATE(op) \
	op(EBatteryPlayState::EPlaying) \
	op(EBatteryPlayState::EGameOver) \
	op(EBatteryPlayState::EWon) \
	op(EBatteryPlayState::EUnknown) 

enum class EBatteryPlayState : uint8;
template<> struct TIsUEnumClass<EBatteryPlayState> { enum { Value = true }; };
template<> BATTERYCOLLECTOR_API UEnum* StaticEnum<EBatteryPlayState>();

PRAGMA_ENABLE_DEPRECATION_WARNINGS
