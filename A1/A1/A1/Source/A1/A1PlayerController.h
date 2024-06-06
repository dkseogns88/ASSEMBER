// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "A1PlayerController.generated.h"

class UInputMappingContext;

/**
 *
 */
UCLASS()
class A1_API AA1PlayerController : public APlayerController
{
	GENERATED_BODY()
	

public:
	AA1PlayerController();
	void AimingChange(bool bIsAiming);
	void FireWeapon();
	void TryFireWeapon();
	void AimPressed();
	void AimReleased();
protected:

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* InputMappingContext;

	
private:
	
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UCrosshairWidget> CrosshairWidgetClass;

	
	UPROPERTY()
	class UCrosshairWidget* CrosshairWidgetInstance;

	


	
};
