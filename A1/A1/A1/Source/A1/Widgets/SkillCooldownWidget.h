// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/BaseWidget.h"
#include "SkillCooldownWidget.generated.h"

/**
 * 
 */
UCLASS()
class A1_API USkillCooldownWidget : public UBaseWidget
{
	GENERATED_BODY()

public:
    USkillCooldownWidget(const FObjectInitializer& ObjectInitializer);

    virtual void NativeConstruct() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    void InitializeCooldown(float CooldownDuration);
    void StartCooldown();
    void ResetCooldown();

protected:
    UPROPERTY(meta = (BindWidget))
    class UImage* SkillIcon;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* CooldownText;

private:
    float CooldownDuration;
    float CooldownRemaining;

    bool bIsCooldownActive;
};
