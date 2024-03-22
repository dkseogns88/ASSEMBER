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
	

	void RequestServerForCharacterChange(FString CharacterName);

	// �����κ��� ĳ���� ���� ���� ������ �޾��� �� ȣ��Ǵ� �Լ�
	void OnServerCharacterChangeResponse(bool bIsChangeApproved, FString CharacterName, FString AdditionalInfo);

    
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* InputMappingContext;

	
	virtual void SetupInputComponent() override;
	
	// UI ǥ�� �� ����� �Լ�
	void ToggleCharacterSelectUI();

	

	

	// �����κ��� ĳ���� ���濡 ���� ������ �޾��� �� ó��
	
	void ExecuteCharacterChange(FString CharacterName);

	TSubclassOf<APawn> FindCharacterClassByName(FString CharacterName);

	

	
	
	//UI ���� �ν��Ͻ��� ����
	UPROPERTY()
	UUserWidget* CharacterSelectWidgetInstance;
	
	

	// UI ���� Ŭ���� Ÿ��
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> CharacterSelectWidgetClass;

	
	// Begin Actor interface
protected:


	

	virtual void BeginPlay() override;

	// End Actor interface
};
