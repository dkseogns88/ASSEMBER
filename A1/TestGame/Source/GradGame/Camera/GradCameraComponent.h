// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GradCameraMode.h"
#include "Camera/CameraComponent.h"
#include "GradCameraComponent.generated.h"

class UGradCameraModeStack;

/** template forward declaration */
template <class TClass> class TSubclassOf;

/** (return type, delegate_name) */
DECLARE_DELEGATE_RetVal(TSubclassOf<UGradCameraMode>, FGradCameraModeDelegate);

UCLASS()
class GRADGAME_API UGradCameraComponent : public UCameraComponent
{
	GENERATED_BODY()
public:
	UGradCameraComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	static UGradCameraComponent* FindCameraComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UGradCameraComponent>() : nullptr); }

	/**
	* member methods
	*/
	AActor* GetTargetActor() const { return GetOwner(); }
	void UpdateCameraModes();

	/**
	 * CameraComponent interface
	 */
	virtual void OnRegister() final;
	virtual void GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView) final;

	/**
	 * member variables
	 */
	/** 카메라의 blending 기능을 지원하는 stack */
	UPROPERTY()
	TObjectPtr<UGradCameraModeStack> CameraModeStack;

	/** 현재 CameraMode를 가져오는 Delegate */
	FGradCameraModeDelegate DetermineCameraModeDelegate;
};
