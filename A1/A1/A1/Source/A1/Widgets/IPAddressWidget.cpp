// Fill out your copyright notice in the Description page of Project Settings.


#include "IPAddressWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "../A1GameInstance.h"
#include "../Objects/A1PlayerController.h"


void UIPAddressWidget::OnIPAddressChanged(const FText& Text)
{
	FString IPAddress = Text.ToString();
	if (UA1GameInstance* GameInstance = Cast<UA1GameInstance>(GetGameInstance()))
	{
		GameInstance->IpAddress = IPAddress;
		IPAddressDisplay->SetText(FText::FromString(IPAddress));

	}
}

void UIPAddressWidget::OnToggleButtonClicked()
{
	if (AA1PlayerController* PlayerController = Cast<AA1PlayerController>(GetOwningPlayer()))
	{
		PlayerController->ToggleIPAddressWidget();
	}
}

void UIPAddressWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (UA1GameInstance* GameInstance = Cast<UA1GameInstance>(GetGameInstance()))
	{
		IPAddressDisplay->SetText(FText::FromString(GameInstance->IpAddress));
	}
	if (IPAddressInput)
	{
		IPAddressInput->OnTextChanged.AddDynamic(this, &UIPAddressWidget::OnIPAddressChanged);
	}
	if (ToggleButton)
	{
		ToggleButton->OnClicked.AddDynamic(this, &UIPAddressWidget::OnToggleButtonClicked);
	}
	if (SubmitButton)
	{
		SubmitButton->OnClicked.AddDynamic(this, &UIPAddressWidget::OnSubmitButtonClicked);
	}

}

void UIPAddressWidget::NativeDestruct()
{

	Super::NativeDestruct();
}

void UIPAddressWidget::OnSubmitButtonClicked()
{
	if (UA1GameInstance* GameInstance = Cast<UA1GameInstance>(GetGameInstance()))
	{
		//Connected = GameInstance->ConnectToGameServer();

		if (Connected)
		{
			// IP 주소가 일치하는 경우, 위젯 닫기
			RemoveFromParent();

			// 게임 모드로 전환
			
			SetInputMode(true);
			
		}
		else
		{
			// IP 주소가 일치하지 않는 경우, 팝업 메시지 표시
			FText ErrorMessage = FText::FromString("IP address does not match. Please try again.");
			IPAddressDisplay->SetText(ErrorMessage);
			IPAddressInput->SetText(FText::GetEmpty());
		}
	}
}