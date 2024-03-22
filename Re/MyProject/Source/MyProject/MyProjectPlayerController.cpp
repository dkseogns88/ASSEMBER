// Copyright Epic Games, Inc. All Rights Reserved.


#include "MyProjectPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "MyProjectMyPlayer.h"
#include "MyProjectMyPlayerSida.h"
#include "Components/InputComponent.h"




void AMyProjectPlayerController::ExecuteCharacterChange(FString CharacterName)
{
    //캐릭터 이름으로 클래스 찾기
    TSubclassOf<APawn> NewCharacterClass = FindCharacterClassByName(CharacterName);
    if (NewCharacterClass == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("Unable to find character class for: %s"), *CharacterName);
        return;
    }

    // 현재 캐릭터 파괴
    APawn* CurrentPawn = GetPawn();
    if (CurrentPawn)
    {
        CurrentPawn->Destroy();
    }

    // 새 캐릭터 스폰
    FVector NewSpawnLocation = FVector(0, 0, 100); // 예시 위치
    FRotator SpawnRotation = FRotator(0, 0, 0); // 예시 회전
    APawn* NewCharacter = GetWorld()->SpawnActor<APawn>(NewCharacterClass, NewSpawnLocation, SpawnRotation);
    if (NewCharacter == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to spawn new character: %s"), *CharacterName);
        return;
    }

    // 새 캐릭터로 플레이어 소유권 변경
    Possess(NewCharacter);

    UE_LOG(LogTemp, Log, TEXT("Character successfully changed to: %s"), *CharacterName);

    // 캐릭터 변경 후 UI 숨기기
    if (CharacterSelectWidgetInstance != nullptr)
    {
        CharacterSelectWidgetInstance->RemoveFromViewport();
        CharacterSelectWidgetInstance = nullptr;
        bShowMouseCursor = false;
        SetInputMode(FInputModeGameOnly());
    }
}

//캐릭터 이름에 해당하는 클래스를 찾아서 반환
TSubclassOf<APawn> AMyProjectPlayerController::FindCharacterClassByName(FString CharacterName)
{
    // 캐릭터 이름과 클래스를 연결하는 맵 key - value 쌍으로 저장함
    static TMap<FString, TSubclassOf<APawn>> CharacterClassMap;

    //Map에 저장
    if (CharacterClassMap.IsEmpty())
    {
        CharacterClassMap.Add("Rinty", AMyProjectMyPlayer::StaticClass());
        CharacterClassMap.Add("Sida", AMyProjectMyPlayerSida::StaticClass());
        
    }

    // 맵에서 캐릭터 이름에 해당하는 클래스 찾기
    TSubclassOf<APawn>* FoundClass = CharacterClassMap.Find(CharacterName);
    if (FoundClass)
    {
        return *FoundClass;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Character class not found for name: %s"), *CharacterName);
        return nullptr;
    }
}




void AMyProjectPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// get the enhanced input subsystem
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(InputMappingContext, 0);

		UE_LOG(LogTemp, Warning, TEXT("BeginPlay"));

		
	}
}



void AMyProjectPlayerController::RequestServerForCharacterChange(FString CharacterName)
{
    // 서버에 캐릭터 변경 요청
 
    UE_LOG(LogTemp, Log, TEXT("Requesting server for character change to: %s"), *CharacterName);
}

void AMyProjectPlayerController::OnServerCharacterChangeResponse(bool bIsChangeApproved, FString CharacterName, FString AdditionalInfo)
{
    if (bIsChangeApproved)
    {
        // 서버로부터 캐릭터 변경 승인

        ExecuteCharacterChange(CharacterName);
    }
    else
    {
        // 거절
        UE_LOG(LogTemp, Warning, TEXT("Server denied character change request: %s"), *AdditionalInfo);
    }

}


void AMyProjectPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("CharacterSelect", IE_Pressed, this, &AMyProjectPlayerController::ToggleCharacterSelectUI);
}

void AMyProjectPlayerController::ToggleCharacterSelectUI()
{
 
    if (CharacterSelectWidgetInstance == nullptr)
    {
        // UI 위젯이 아직 생성되지 않았다면 생성
        CharacterSelectWidgetInstance = CreateWidget<UUserWidget>(this, CharacterSelectWidgetClass);
        if (CharacterSelectWidgetInstance != nullptr)
        {
            CharacterSelectWidgetInstance->AddToViewport();
            bShowMouseCursor = true; // 마우스 커서 표시
            SetInputMode(FInputModeUIOnly()); // UI 입력 모드 설정
        }
    }
    else
    {
        // UI가 이미 표시되어 있다면 숨김
        CharacterSelectWidgetInstance->RemoveFromViewport();
        CharacterSelectWidgetInstance = nullptr;
        bShowMouseCursor = false; // 마우스 커서 숨김
        SetInputMode(FInputModeGameOnly()); // 게임 입력 모드로 복귀
    }
}

