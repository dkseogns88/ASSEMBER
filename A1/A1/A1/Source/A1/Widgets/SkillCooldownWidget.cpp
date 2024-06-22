// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillCooldownWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"


USkillCooldownWidget::USkillCooldownWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)

    , CooldownDuration(0.0f)
    , CooldownRemaining(0.0f)
    , bIsCooldownActive(false)
{
}

void USkillCooldownWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (CooldownText)
    {
        CooldownText->SetText(FText::FromString(TEXT("")));
    }
}

void USkillCooldownWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if (bIsCooldownActive)
    {
        CooldownRemaining -= InDeltaTime;
        if (CooldownRemaining <= 0.0f)
        {
            CooldownRemaining = 0.0f;
            bIsCooldownActive = false;
            CooldownText->SetText(FText::FromString(TEXT("")));
        }
        else
        {
            CooldownText->SetText(FText::AsNumber(FMath::CeilToInt(CooldownRemaining)));
        }
    }
}

void USkillCooldownWidget::InitializeCooldown(float InCooldownDuration)
{
    CooldownDuration = InCooldownDuration;
    CooldownRemaining = 0.0f;
    CooldownText->SetText(FText::FromString(TEXT("")));
    bIsCooldownActive = false;
}


void USkillCooldownWidget::StartCooldown()
{
    CooldownRemaining = CooldownDuration;
    bIsCooldownActive = true;
}


void USkillCooldownWidget::ResetCooldown()
{
    CooldownRemaining = 0.0f;
    CooldownText->SetText(FText::FromString(TEXT("")));
    bIsCooldownActive = false;
}