// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"

UGameOverWidget::UGameOverWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

void UGameOverWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (GameOverText)
    {
        GameOverText->SetText(FText::FromString("Game Over"));
    }

    if (ExitButton)
    {
        ExitButton->OnClicked.AddDynamic(this, &UGameOverWidget::OnExitButtonClicked);
    }
}

void UGameOverWidget::OnExitButtonClicked()
{
    UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}