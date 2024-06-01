// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBarWidgets.h"

void UHealthBarWidgets::UpdateHealth(float HealthPercentage)
{
    if (HealthProgressBar)
    {
        HealthProgressBar->SetPercent(HealthPercentage);
    }
    if (HealthText)
    {
        FString HealthTextString = FString::Printf(TEXT("%.0f%%"), HealthPercentage * 100);
        HealthText->SetText(FText::FromString(HealthTextString));
    }
}

void UHealthBarWidgets::NativeConstruct()
{
    Super::NativeConstruct(); // Always call Super first to ensure all elements are initialized

    UE_LOG(LogTemp, Log, TEXT("NativeConstruct called."));

    HealthProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HealthProgressBar")));
    if (HealthProgressBar)
    {
        UE_LOG(LogTemp, Log, TEXT("HealthProgressBar found and ready to use."));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("HealthProgressBar not found."));
    }

    HealthText = Cast<UTextBlock>(GetWidgetFromName(TEXT("HealthText")));
    if (HealthText)
    {
        UE_LOG(LogTemp, Log, TEXT("HealthText found and ready to use."));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("HealthText not found."));
    }

    UpdateHealth(1.0f); // Initialize with full health to ensure the bar and text reflect this at start.
}
