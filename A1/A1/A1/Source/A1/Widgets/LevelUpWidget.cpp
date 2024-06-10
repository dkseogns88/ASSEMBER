// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelUpWidget.h"
#include "../Objects/A1PlayerController.h"
#include "Kismet/GameplayStatics.h"

void ULevelUpWidget::NativeConstruct()
{
    Super::NativeConstruct();

    PlayerController = Cast<AA1PlayerController>(UGameplayStatics::GetPlayerController(this, 0));
}

void ULevelUpWidget::OnOptionSelected(int OptionIndex)
{
    if (PlayerController)
    {
        PlayerController->HandleLevelUpOption(OptionIndex);
    }
}
