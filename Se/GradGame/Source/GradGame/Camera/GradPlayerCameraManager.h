// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "GradPlayerCameraManager.generated.h"

/**
 * Controller에 바인딩된 CameraManager
 */
#define GRAD_CAMERA_DEFAULT_FOV (80.0f)
#define GRAD_CAMERA_DEFAULT_PITCH_MIN (-89.0f)
#define GRAD_CAMERA_DEFAULT_PITCH_MAX (89.0f)

/**
 * 
 */ 
UCLASS()
class GRADGAME_API AGradPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
public:
	AGradPlayerCameraManager(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
