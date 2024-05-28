// Fill out your copyright notice in the Description page of Project Settings.


#include "IPAddressWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "MyProjectGameInstance.h"
#include "MyProjectPlayerController.h"


void UIPAddressWidget::OnIPAddressChanged(const FText& Text)
{
	FString IPAddress = Text.ToString();
	if (UMyProjectGameInstance* GameInstance = Cast<UMyProjectGameInstance>(GetGameInstance()))
	{
		GameInstance->ServerIPAddress = IPAddress;
		IPAddressDisplay->SetText(FText::FromString(IPAddress));
	}
}

void UIPAddressWidget::OnToggleButtonClicked()
{
	if (AMyProjectPlayerController* PlayerController = Cast<AMyProjectPlayerController>(GetOwningPlayer()))
	{
		PlayerController->ToggleIPAddressWidget();
	}
}

void UIPAddressWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (UMyProjectGameInstance* GameInstance = Cast<UMyProjectGameInstance>(GetGameInstance()))
	{
		IPAddressDisplay->SetText(FText::FromString(GameInstance->ServerIPAddress));
	}
	if (IPAddressInput)
	{
		IPAddressInput->OnTextChanged.AddDynamic(this, &UIPAddressWidget::OnIPAddressChanged);
	}
	if (ToggleButton)
	{
		ToggleButton->OnClicked.AddDynamic(this, &UIPAddressWidget::OnToggleButtonClicked);
	}

}