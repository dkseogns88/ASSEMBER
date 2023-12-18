// Copyright Epic Games, Inc. All Rights Reserved.

#include "LevelUpDemoGameMode.h"
#include "LevelUpDemoCharacter.h"
#include "LevelUpGameInstance.h"
#include "LevelUpDemoPlayerController.h"
#include "UObject/ConstructorHelpers.h"

ALevelUpDemoGameMode::ALevelUpDemoGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// PlayerController 클래스 설정
	PlayerControllerClass = ALevelUpDemoPlayerController::StaticClass();
}
