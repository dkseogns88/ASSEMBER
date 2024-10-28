// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GradCameraMode.h"
#include "GradCameraMode_ThirdPerson.generated.h"

class UCurveVector;

UCLASS(Abstract , Blueprintable)
class GRADGAME_API UGradCameraMode_ThirdPerson : public UGradCameraMode
{
	GENERATED_BODY()
public:
	UGradCameraMode_ThirdPerson(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	/**
	* UGradCameraMode's interface
	*/
	virtual void UpdateView(float DeltaTime) override;

	/**
	 * member variables
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Third Person")
	TObjectPtr<const UCurveVector> TargetOffsetCurve;
};
