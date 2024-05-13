// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoWidget.h"

void UAmmoWidget::UpdateAmmoCount(int32 CurrentAmmo, int32 MaxAmmo)
{
    if (CurrentAmmoText)
    {
        CurrentAmmoText->SetText(FText::AsNumber(CurrentAmmo));
    }

    if (TotalAmmoText)
    {
        TotalAmmoText->SetText(FText::AsNumber(MaxAmmo));
    }
}
void UAmmoWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (!CurrentAmmoText)
    {
        UE_LOG(LogTemp, Error, TEXT("CurrentAmmoText is not bound correctly"));
    }

    if (!TotalAmmoText)
    {
        UE_LOG(LogTemp, Error, TEXT("TotalAmmoText is not bound correctly"));
    }
}