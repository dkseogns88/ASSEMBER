// Fill out your copyright notice in the Description page of Project Settings.


#include "KeyTipWidget.h"
#include "Components/TextBlock.h"


UKeyTipWidget::UKeyTipWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

void UKeyTipWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Initialize the widget elements here
    if (KeyTipsText)
    {
        KeyTipsText->SetText(FText::FromString("Move : W,A,S,D,Space \nAim : RClick\nShoot : LClick\nReload: R\nStat : I\nLevelUP : [\nSkill : LShift"));
    }
}

void UKeyTipWidget::ToggleVisibility()
{
    if (IsVisible())
    {
        SetVisibility(ESlateVisibility::Hidden);
    }
    else
    {
        SetVisibility(ESlateVisibility::Visible);
    }
}