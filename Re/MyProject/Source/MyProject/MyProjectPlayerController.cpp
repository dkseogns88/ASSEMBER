// Copyright Epic Games, Inc. All Rights Reserved.


#include "MyProjectPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "MyProjectMyPlayer.h"
#include "MyProjectMyPlayerSida.h"
#include "Components/InputComponent.h"




void AMyProjectPlayerController::ExecuteCharacterChange(FString CharacterName)
{
    //ĳ���� �̸����� Ŭ���� ã��
    TSubclassOf<APawn> NewCharacterClass = FindCharacterClassByName(CharacterName);
    if (NewCharacterClass == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("Unable to find character class for: %s"), *CharacterName);
        return;
    }

    // ���� ĳ���� �ı�
    APawn* CurrentPawn = GetPawn();
    if (CurrentPawn)
    {
        CurrentPawn->Destroy();
    }

    // �� ĳ���� ����
    FVector NewSpawnLocation = FVector(0, 0, 100); // ���� ��ġ
    FRotator SpawnRotation = FRotator(0, 0, 0); // ���� ȸ��
    APawn* NewCharacter = GetWorld()->SpawnActor<APawn>(NewCharacterClass, NewSpawnLocation, SpawnRotation);
    if (NewCharacter == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to spawn new character: %s"), *CharacterName);
        return;
    }

    // �� ĳ���ͷ� �÷��̾� ������ ����
    Possess(NewCharacter);

    UE_LOG(LogTemp, Log, TEXT("Character successfully changed to: %s"), *CharacterName);

    // ĳ���� ���� �� UI �����
    if (CharacterSelectWidgetInstance != nullptr)
    {
        CharacterSelectWidgetInstance->RemoveFromViewport();
        CharacterSelectWidgetInstance = nullptr;
        bShowMouseCursor = false;
        SetInputMode(FInputModeGameOnly());
    }
}

//ĳ���� �̸��� �ش��ϴ� Ŭ������ ã�Ƽ� ��ȯ
TSubclassOf<APawn> AMyProjectPlayerController::FindCharacterClassByName(FString CharacterName)
{
    // ĳ���� �̸��� Ŭ������ �����ϴ� �� key - value ������ ������
    static TMap<FString, TSubclassOf<APawn>> CharacterClassMap;

    //Map�� ����
    if (CharacterClassMap.IsEmpty())
    {
        CharacterClassMap.Add("Rinty", AMyProjectMyPlayer::StaticClass());
        CharacterClassMap.Add("Sida", AMyProjectMyPlayerSida::StaticClass());
        
    }

    // �ʿ��� ĳ���� �̸��� �ش��ϴ� Ŭ���� ã��
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
    // ������ ĳ���� ���� ��û
 
    UE_LOG(LogTemp, Log, TEXT("Requesting server for character change to: %s"), *CharacterName);
}

void AMyProjectPlayerController::OnServerCharacterChangeResponse(bool bIsChangeApproved, FString CharacterName, FString AdditionalInfo)
{
    if (bIsChangeApproved)
    {
        // �����κ��� ĳ���� ���� ����

        ExecuteCharacterChange(CharacterName);
    }
    else
    {
        // ����
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

