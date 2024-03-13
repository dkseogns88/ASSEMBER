// Copyright Epic Games, Inc. All Rights Reserved.


#include "MyProjectPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "MyProjectMyPlayer.h"
#include "Components/InputComponent.h"



void AMyProjectPlayerController::NotifyServerOfCharacterChange(const FCharacterChangeInfo& ChangeInfo)
{
    // 서버와 통신하여 캐릭터 변경 정보 전달
    // 구현은 서버 팀과 협의 필요
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


void AMyProjectPlayerController::ChangeCharacter(TSubclassOf<APawn> NewCharacterClass)
{
    if (!GetPawn() || !NewCharacterClass) return;

    // 현재 캐릭터 파괴
    GetPawn()->Destroy();

    // 새 캐릭터 스폰 위치 및 회전 설정
    FVector NewSpawnLocation = FVector(0, 0, 100);
    FRotator NewSpawnRotation = FRotator(0, 0, 0);

    // 새 캐릭터 스폰
    APawn* NewSpawnCharacter = GetWorld()->SpawnActor<APawn>(NewCharacterClass, NewSpawnLocation, NewSpawnRotation);
    if (NewSpawnCharacter)
    {
        // 새 캐릭터 소유
        Possess(NewSpawnCharacter);

        // 캐릭터 이름 로그로 출력
        UE_LOG(LogTemp, Log, TEXT("Character Changed to: %s"), *NewSpawnCharacter->GetName());

        // 캐릭터 변경 후 UI 숨기기 로직
        if (CharacterSelectWidgetInstance != nullptr)
        {
            CharacterSelectWidgetInstance->RemoveFromViewport();
            CharacterSelectWidgetInstance = nullptr;
            bShowMouseCursor = false; // 마우스 커서 숨김
            SetInputMode(FInputModeGameOnly()); // 게임 입력 모드로 복귀
        }

        // 서버에 캐릭터 변경 정보 전달
        FCharacterChangeInfo ChangeInfo;
        ChangeInfo.CharacterName = NewSpawnCharacter->GetName(); // GetName()으로 캐릭터 이름 설정
        NotifyServerOfCharacterChange(ChangeInfo);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Character spawn failed"));
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

