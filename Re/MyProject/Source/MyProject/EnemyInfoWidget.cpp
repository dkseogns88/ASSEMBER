// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyInfoWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"


void UEnemyInfoWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Ensure the widgets are bound correctly
    if (!EnemyNameText)
    {
        UE_LOG(LogTemp, Error, TEXT("EnemyNameText is not bound correctly"));
    }
    if (!EnemyHealthBar)
    {
        UE_LOG(LogTemp, Error, TEXT("EnemyHealthBar is not bound correctly"));
    }
}



void UEnemyInfoWidget::SetEnemyName(FString Name)
{
    if (EnemyNameText)
    {
        UE_LOG(LogTemp, Log, TEXT("Setting enemy name to: %s"), *Name);
        EnemyNameText->SetText(FText::FromString(Name));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("EnemyNameText is null in SetEnemyName"));
    }
}

void UEnemyInfoWidget::SetEnemyHealth(float HealthPercent)
{
    if (EnemyHealthBar)
    {
        UE_LOG(LogTemp, Log, TEXT("Setting enemy health to: %f"), HealthPercent);
        EnemyHealthBar->SetPercent(HealthPercent);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("EnemyHealthBar is null in SetEnemyHealth"));
    }
}