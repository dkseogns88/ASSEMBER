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
    Super::NativeConstruct(); // Always call the base implementation first


    // Check the bound widgets for additional setup or validation
    if (HealthProgressBar)
    {
        // Perform any extra initialization or setup here
        UE_LOG(LogTemp, Log, TEXT("HealthProgressBar found and ready to use."));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("HealthProgressBar not found."));
    }

    if (HealthText)
    {
        // Additional setup for HealthText
        UE_LOG(LogTemp, Log, TEXT("HealthText found and ready to use."));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("HealthText not found."));
    }

    // Example of setting default values or running start-up animations
    UpdateHealth(1.0f); // Initialize with full health
}
