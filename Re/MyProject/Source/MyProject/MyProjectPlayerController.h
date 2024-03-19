// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h" 
#include "Blueprint/UserWidget.h"
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
	void ChangeCharacter(TSubclassOf<APawn> NewCharacterClass);
    
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* InputMappingContext;

	
	virtual void SetupInputComponent() override;
	
	// UI ǥ�� �� ����� �Լ�
	void ToggleCharacterSelectUI();
	

	
	//UI ���� �ν��Ͻ��� ����
	UPROPERTY()
	UUserWidget* CharacterSelectWidgetInstance;
	
	

	// UI ���� Ŭ���� Ÿ��
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> CharacterSelectWidgetClass;

	
	// Begin Actor interface
protected:

	void NotifyServerOfCharacterChange(const FCharacterChangeInfo& ChangeInfo);

	virtual void BeginPlay() override;

	// End Actor interface
};
