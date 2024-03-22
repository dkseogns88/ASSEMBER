// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h" 
#include "Blueprint/UserWidget.h"
#include "MyProjectPlayerController.generated.h"

class UInputMappingContext;

//캐릭터변경정보 
struct FCharacterChangeInfo
{
	FString CharacterName; // 캐릭터 이름
	UClass* CharacterClass; // 캐릭터의 클래스 정보
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
	// 캐릭터 변경 함수
	UFUNCTION(BlueprintCallable, Category = "Character")
	

	void RequestServerForCharacterChange(FString CharacterName);

	// 서버로부터 캐릭터 변경 승인 응답을 받았을 때 호출되는 함수
	void OnServerCharacterChangeResponse(bool bIsChangeApproved, FString CharacterName, FString AdditionalInfo);

    
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* InputMappingContext;

	
	virtual void SetupInputComponent() override;
	
	// UI 표시 및 숨기기 함수
	void ToggleCharacterSelectUI();

	

	

	// 서버로부터 캐릭터 변경에 대한 응답을 받았을 때 처리
	
	void ExecuteCharacterChange(FString CharacterName);

	TSubclassOf<APawn> FindCharacterClassByName(FString CharacterName);

	

	
	
	//UI 위젯 인스턴스를 관리
	UPROPERTY()
	UUserWidget* CharacterSelectWidgetInstance;
	
	

	// UI 위젯 클래스 타입
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> CharacterSelectWidgetClass;

	
	// Begin Actor interface
protected:


	

	virtual void BeginPlay() override;

	// End Actor interface
};
