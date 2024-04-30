// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h" 
#include "Blueprint/UserWidget.h"
#include "HealthBarWidgets.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> AimUIClass;
	
	UPROPERTY()
	UUserWidget* AimUIInstance;
	

	bool bIsUIActive = false;
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
	
protected:


	

	AMyProjectPlayerController();

	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UHealthBarWidgets> HealthBarWidgetClass;

	// Health bar widget instance
	UPROPERTY()
	UHealthBarWidgets* HealthBarWidgets;
	float PlayerHealth = 100.0f;

	// End Actor interface
};
