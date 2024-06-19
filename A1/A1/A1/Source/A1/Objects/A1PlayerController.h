// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "../Widgets/EnemyInfoWidget.h"
#include "../Widgets/BaseWidget.h"
#include "../Widgets/HealthBarWidget.h"
#include "../Widgets/LevelUpWidget.h"
#include "../Widgets/AmmoWidget.h"
#include "../Widgets/IPAddressWidget.h"
#include "../Characters/Monster.h"
#include "../Widgets/HealthBarWidget.h"
#include "../Widgets/PlayerStatWidget.h"
#include "../SKill.h"
#include "../BombSkill.h"
#include "A1PlayerController.generated.h"

class UInputMappingContext;
class UBaseWidget;
class UHealthBarWidgets;
class UAmmoWidget;
class UEnemyInfoWidget;
/**
 *
 */
UCLASS()
class A1_API AA1PlayerController : public APlayerController
{
	GENERATED_BODY()
	

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float PlayerHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float PlayerMaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float AttackPower;

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetHealth(float NewHealth);

	// Function to initialize stats
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void InitializeStats(float InitialHealth, float InitialMovementSpeed, float InitialAttackPower);

	// Function to update stats
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void UpdateStats(float NewHealth, float NewMovementSpeed, float NewAttackPower,float NewMaxHealth);

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void ShowLevelUpUI();

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void ApplyDamage(float DamageAmount);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	TSubclassOf<ASKill> SkillClass; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ABombSkill> BombSkillClass;

	UPROPERTY()
	ABombSkill* CurrentBombSkill;

	UFUNCTION(BlueprintCallable, Category = "Character")

	//void RequestServerForAimingChange(bool bIsAiming);
	//void RequestServerForRollingChange(bool bIsRolling);

	void Tick(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void RemoveEnemyInfo();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowEnemyInfo(AMonster* Enemy);



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UEnemyInfoWidget> EnemyInfoWidgetClass;

	UPROPERTY()
	UEnemyInfoWidget* CurrentEnemyInfoWidget;

	UFUNCTION()
	void ReloadWeapon();

	bool bIsUIActive = false;
	
	FTimerHandle FireCooldownTimerHandle;


	UFUNCTION(BlueprintCallable, Category = "LevelUp")
	void HandleLevelUpOption(int OptionIndex);

	void ToggleIPAddressWidget();

	//void SetNearbyStatue(ACharacterStatue* Statue);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UBaseWidget> IPAddressWidgetClass;

private:

	void ShowEnemyInfo_Internal(FString EnemyName, float Health);

	// Level Up UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UBaseWidget> LevelUpWidgetClass;

	UPROPERTY()
	ULevelUpWidget* LevelUpWidgetInstance;

	// Function to update stats based on selected option
	void UpdateStatsBasedOnOption(int OptionIndex);

	//IPAdress Widget
	UPROPERTY()
	UIPAddressWidget* IPAddressWidget;

	bool bIsIPWidgetVisible;

	void Interact();
	//ACharacterStatue* NearbyStatue;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UCrosshairWidget> CrosshairWidgetClass;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UPlayerStatWidget> PlayerStatWidgetClass;

	UPROPERTY()
	class UCrosshairWidget* CrosshairWidgetInstance;

	UPROPERTY()
	UPlayerStatWidget* PlayerStatWidget;

	void TogglePlayerStatWidget();

	void UseSkill();

	UFUNCTION()
	void OnSkillEnd();
	
	FVector GetCamCenLoc(FVector& CameraLocation, FRotator& CameraRotation);

	UFUNCTION(BlueprintCallable, Category = "Skills")
	void UseBombSkill();

	UFUNCTION(BlueprintCallable, Category = "Skills")
	void ThrowBomb();

public:
	AA1PlayerController();
	void AimingChange(bool bIsAiming);
	void FireWeapon();
	void TryFireWeapon();
	void AimPressed();
	void AimReleased();
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* InputMappingContext;

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	
	

	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UBaseWidget> HealthBarWidgetClass;

	// Health bar widget instance
	UPROPERTY()
	UHealthBarWidget* HealthBarWidgets;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UBaseWidget> AmmoWidgetClass;

	UPROPERTY()
	UAmmoWidget* AmmoWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int32 CurrentAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int32 MaxAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
	USoundBase* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
	USoundBase* ReloadSound;


	
private:
	




	


	
};
