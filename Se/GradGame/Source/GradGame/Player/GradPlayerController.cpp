// Fill out your copyright notice in the Description page of Project Settings.


#include "GradPlayerController.h"
#include "GradGame/Camera/GradPlayerCameraManager.h"
#include "EnhancedInputSubsystems.h"


AGradPlayerController::AGradPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PlayerCameraManagerClass = AGradPlayerCameraManager::StaticClass();
}