// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"


UBaseWidget::UBaseWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

void UBaseWidget::NativeConstruct()
{
    Super::NativeConstruct();
   
}

void UBaseWidget::NativeDestruct()
{
    Super::NativeDestruct();
   
}

void UBaseWidget::ShowWidget()
{
    this->AddToViewport();
    SetUIMode(true);
}

void UBaseWidget::HideWidget()
{
    this->RemoveFromViewport();
    SetGameMode();
}

void UBaseWidget::SetUIMode(bool bIsUIMode)
{
    SetInputMode(bIsUIMode);
}

void UBaseWidget::SetGameMode()
{
    SetInputMode(false);
}

void UBaseWidget::SetInputMode(bool bIsUIMode)
{
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
    if (PlayerController)
    {
        if (bIsUIMode)
        {
            FInputModeUIOnly InputMode;
            InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
            PlayerController->SetInputMode(InputMode);
            PlayerController->bShowMouseCursor = true;
        }
        else
        {
            FInputModeGameOnly InputMode;
            PlayerController->SetInputMode(InputMode);
            PlayerController->bShowMouseCursor = false;
        }
    }
}