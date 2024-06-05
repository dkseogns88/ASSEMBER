// Fill out your copyright notice in the Description page of Project Settings.


#include "CrosshairWidget.h"
#include "Components/Image.h"


void UCrosshairWidget::NativeConstruct()
{
    Super::NativeConstruct();

   
    if (CrosshairImage)
    {
        CrosshairImage->SetVisibility(ESlateVisibility::Hidden);
    }
}

void UCrosshairWidget::ShowCrosshair(bool bIsAiming)
{
    if (CrosshairImage)
    {
        CrosshairImage->SetVisibility(bIsAiming ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }
}