// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStatWidget.h"
#include "Components/TextBlock.h"

void UPlayerStatWidget::NativeConstruct()
{
	Super::NativeConstruct();
	// À§Á¬ ÃÊ±âÈ­
	UpdateStats(0, 0, 0);
}

void UPlayerStatWidget::UpdateStats(float Health, float Speed, float Attack)
{
	if (HealthText)
	{
		HealthText->SetText(FText::AsNumber(Health));
	}
	if (SpeedText)
	{
		SpeedText->SetText(FText::AsNumber(Speed));
	}
	if (AttackText)
	{
		AttackText->SetText(FText::AsNumber(Attack));
	}
}
