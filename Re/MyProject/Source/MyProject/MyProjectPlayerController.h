// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h" 
#include "\Program Files\Epic Games\UE_5.3\Engine\Source\Runtime\Engine\Classes\Sound\SoundBase.h"
#include "Blueprint/UserWidget.h"
#include "HealthBarWidgets.h"
#include "LevelUpWidget.h"
#include "AmmoWidget.h"
#include "NPC.h"
#include "SkillManager.h"
#include "IPAddressWidget.h"
#include "Character/CharacterStatue.h"
#include "MyProjectPlayerController.generated.h"

class UInputMappingContext;
class UUserWidget;
class UHealthBarWidgets;
class UAmmoWidget;
class UEnemyInfoWidget;

//ĳ���ͺ������� 
struct FCharacterChangeInfo
{
	FString CharacterName; // ĳ���� �̸�
	UClass* CharacterClass; // ĳ������ Ŭ���� ����
    UClass* CharacterParentClass;
};

UCLASS()
class MYPROJECT_API AMyProjectPlayerController : public APlayerController
{

	GENERATED_BODY()


public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float PlayerHealth;

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
	void UpdateStats(float NewHealth, float NewMovementSpeed, float NewAttackPower);

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void ShowLevelUpUI();
	
	// ĳ���� ���� �Լ�
	UFUNCTION(BlueprintCallable, Category = "Character")
	


	void RequestServerForAimingChange(bool bIsAiming);
	void RequestServerForRollingChange(bool bIsRolling);

	void Tick(float DeltaTime);

	void RemoveEnemyInfo();

	void FireWeapon();

	void ShowEnemyInfo(ANPC* Enemy);
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> AimUIClass;
	
	UPROPERTY()
	UUserWidget* AimUIInstance;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UEnemyInfoWidget> EnemyInfoWidgetClass;

	UPROPERTY()
	UEnemyInfoWidget* CurrentEnemyInfoWidget;

	UFUNCTION()
	void ReloadWeapon();

	bool bIsUIActive = false;
	void AttemptToFireWeapon();
	

	
	UFUNCTION(BlueprintCallable, Category = "LevelUp")
	void HandleLevelUpOption(int OptionIndex);

	void ToggleIPAddressWidget();

	void SetNearbyStatue(ACharacterStatue* Statue);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UIPAddressWidget> IPAddressWidgetClass;

private:
	
	void ShowEnemyInfo_Internal(FString EnemyName, float Health);

	// Level Up UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> LevelUpWidgetClass;

	UPROPERTY()
	ULevelUpWidget* LevelUpWidgetInstance;

	// Function to update stats based on selected option
	void UpdateStatsBasedOnOption(int OptionIndex);

	//IPAdress Widget
	UPROPERTY()
	UIPAddressWidget* IPAddressWidget;

	bool bIsIPWidgetVisible;

	void Interact();
	ACharacterStatue* NearbyStatue;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* InputMappingContext;


	
	virtual void SetupInputComponent() override;

	void OnAimPressed();

	void OnAimReleased();	

	void OnRollPressed();

	void MoveForward(float Value);
	void MoveRight(float Value);

	
	//UI ���� �ν��Ͻ��� ����
	UPROPERTY()
	UUserWidget* CharacterSelectWidgetInstance;
	
	

	// UI ���� Ŭ���� Ÿ��
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> CharacterSelectWidgetClass;

	


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
	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UAmmoWidget> AmmoWidgetClass;

	UPROPERTY()
	UAmmoWidget* AmmoWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int32 CurrentAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int32 MaxAmmo;


private:
		void UseSkill();
		void PerformSkill(FName SkillName);

		UPROPERTY(EditDefaultsOnly, Category = "Skill")
		USkillManager* SkillManager;

		// 스킬 사용 횟수를 저장하는 변수
		TMap<FName, int32> SkillUsageCount;

		void LogSkillUsage(FName SkillName);

		float ForwardInput;
		float RightInput;
		
	
};
