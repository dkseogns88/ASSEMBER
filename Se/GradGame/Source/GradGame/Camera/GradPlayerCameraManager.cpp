// Fill out your copyright notice in the Description page of Project Settings.

#include "GradPlayerCameraManager.h"

AGradPlayerCameraManager::AGradPlayerCameraManager(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	DefaultFOV = GRAD_CAMERA_DEFAULT_FOV;
	ViewPitchMin = GRAD_CAMERA_DEFAULT_PITCH_MIN;
	ViewPitchMax = GRAD_CAMERA_DEFAULT_PITCH_MAX;
}
