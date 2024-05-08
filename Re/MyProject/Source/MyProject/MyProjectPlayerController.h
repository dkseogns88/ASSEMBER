// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h" 
#include "\Program Files\Epic Games\UE_5.3\Engine\Source\Runtime\Engine\Classes\Sound\SoundBase.h"
#include "Blueprint/UserWidget.h"
#include "HealthBarWidgets.h"
#include "AmmoWidget.h"
#include "MyProjectPlayerController.generated.h"

class UInputMappingContext;

//ĳ���ͺ������� 
struct FCharacterChangeInfo
{
	FString CharacterName; // ĳ���� �̸�
	UClass* CharacterClass; // ĳ������ Ŭ���� ����
    UClass* CharacterParentClass;
};
/**
 *
 */
UCLASS()
class MYPROJECT_API AMyProjectPlayerController : public APlayerController
{

	GENERATED_BODY()


public:



	
	// ĳ���� ���� �Լ�
	UFUNCTION(BlueprintCallable, Category = "Character")
	
	void RequestServerForCharacterChange(FString CharacterName);

	void RequestServerForAimingChange(bool bIsAiming);

	void FireWeapon();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> AimUIClass;
	
	UPROPERTY()
	UUserWidget* AimUIInstance;
	
	UFUNCTION()
	void ReloadWeapon();

	bool bIsUIActive = false;
	void AttemptToFireWeapon();
	void ToggleCharacterSelectUI();
private:
	// UI가 현재 활성화되어 있는지 여부를 나타내는 변수
	

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* InputMappingContext;


	
	virtual void SetupInputComponent() override;

	void OnAimPressed();

	void OnAimReleased();	
	
	//UI ���� �ν��Ͻ��� ����
	UPROPERTY()
	UUserWidget* CharacterSelectWidgetInstance;
	
	

	// UI ���� Ŭ���� Ÿ��
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> CharacterSelectWidgetClass;

	void SetHealth(float NewHealth);


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
	USoundBase* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
	USoundBase* ReloadSound;

	
protected:

	AMyProjectPlayerController();

	
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UHealthBarWidgets> HealthBarWidgetClass;

	// Health bar widget instance
	UPROPERTY()
	UHealthBarWidgets* HealthBarWidgets;
	float PlayerHealth = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UAmmoWidget> AmmoWidgetClass;

	UPROPERTY()
	UAmmoWidget* AmmoWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int32 CurrentAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int32 MaxAmmo;

	// End Actor interface
};
