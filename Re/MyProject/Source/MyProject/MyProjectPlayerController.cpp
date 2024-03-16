// Copyright Epic Games, Inc. All Rights Reserved.


#include "MyProjectPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "MyProjectMyPlayer.h"
#include "Components/InputComponent.h"



void AMyProjectPlayerController::NotifyServerOfCharacterChange(const FCharacterChangeInfo& ChangeInfo)
{
    /* ������ ����Ͽ� ĳ���� ���� ���� ����
    ���� �����Һκ� , ChangeInfo�� ����� ĳ���� ������ ����Ǿ�����.����ɶ����� ��������.
    �ʿ信���� �߰����� ���� �����ҿ���
    */


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

    // ���� ĳ���� �ı�
    GetPawn()->Destroy();

    // �� ĳ���� ���� ��ġ �� ȸ�� ����
    FVector NewSpawnLocation = FVector(0, 0, 100);
    FRotator NewSpawnRotation = FRotator(0, 0, 0);

    // �� ĳ���� ����
    APawn* NewSpawnCharacter = GetWorld()->SpawnActor<APawn>(NewCharacterClass, NewSpawnLocation, NewSpawnRotation);
    if (NewSpawnCharacter)
    {
        // �� ĳ���� ����
        Possess(NewSpawnCharacter);

        // ������ ĳ���� ���� ���� ����
        FCharacterChangeInfo ChangeInfo;
        ChangeInfo.CharacterName = NewSpawnCharacter->GetName();
        ChangeInfo.CharacterClass = NewSpawnCharacter->GetClass();
        ChangeInfo.CharacterParentClass = NewSpawnCharacter->GetClass()->GetSuperClass();

        // �θ� Ŭ���� �̸� �α׷� ��� (�θ� Ŭ���� ������ ���� ��츸)
        if (ChangeInfo.CharacterParentClass != nullptr)
        {
            FString ParentClassName = ChangeInfo.CharacterParentClass->GetName();
            UE_LOG(LogTemp, Log, TEXT("Character Changed to: %s, Parent Class: %s"), *ChangeInfo.CharacterName, *ParentClassName);
        }

        NotifyServerOfCharacterChange(ChangeInfo);

        // ĳ���� ���� �� UI �����
        if (CharacterSelectWidgetInstance != nullptr)
        {
            CharacterSelectWidgetInstance->RemoveFromViewport();
            CharacterSelectWidgetInstance = nullptr;
            bShowMouseCursor = false;
            SetInputMode(FInputModeGameOnly());
        }
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
        // UI ������ ���� �������� �ʾҴٸ� ����
        CharacterSelectWidgetInstance = CreateWidget<UUserWidget>(this, CharacterSelectWidgetClass);
        if (CharacterSelectWidgetInstance != nullptr)
        {
            CharacterSelectWidgetInstance->AddToViewport();
            bShowMouseCursor = true; // ���콺 Ŀ�� ǥ��
            SetInputMode(FInputModeUIOnly()); // UI �Է� ��� ����
        }
    }
    else
    {
        // UI�� �̹� ǥ�õǾ� �ִٸ� ����
        CharacterSelectWidgetInstance->RemoveFromViewport();
        CharacterSelectWidgetInstance = nullptr;
        bShowMouseCursor = false; // ���콺 Ŀ�� ����
        SetInputMode(FInputModeGameOnly()); // ���� �Է� ���� ����
    }
}

