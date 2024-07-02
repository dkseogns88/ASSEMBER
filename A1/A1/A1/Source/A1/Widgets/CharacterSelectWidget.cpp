// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSelectWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "../Objects/A1PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "A1GameInstance.h"


UCharacterSelectWidget::UCharacterSelectWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{

}

void UCharacterSelectWidget::NativeConstruct()
{
    Super::NativeConstruct();

    PlayerController = Cast<AA1PlayerController>(UGameplayStatics::GetPlayerController(this, 0));

    // 기존 바인딩 해제 후 버튼 클릭 이벤트 바인딩
    if (RintyButton)
    {
        RintyButton->OnClicked.RemoveDynamic(this, &UCharacterSelectWidget::RintyButtonClicked);
        RintyButton->OnClicked.AddDynamic(this, &UCharacterSelectWidget::RintyButtonClicked);
        ;
    }
    

    if (SidaButton)
    {
        SidaButton->OnClicked.RemoveDynamic(this, &UCharacterSelectWidget::SidaButtonClicked);
        SidaButton->OnClicked.AddDynamic(this, &UCharacterSelectWidget::SidaButtonClicked);
        
    }
   
    if (CloseButton)
    {
        CloseButton->OnClicked.RemoveDynamic(this, &UCharacterSelectWidget::CloseButtonClicked);
        CloseButton->OnClicked.AddDynamic(this, &UCharacterSelectWidget::CloseButtonClicked);
       
    }
  
    

    // 타이머 초기화
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UCharacterSelectWidget::UpdateTimer, 1.0f, true);
}

void UCharacterSelectWidget::NativeDestruct()
{
    Super::NativeDestruct();

    // 버튼 클릭 이벤트 바인딩 해제
    if (RintyButton)
    {
        RintyButton->OnClicked.RemoveDynamic(this, &UCharacterSelectWidget::RintyButtonClicked);
    }

    if (SidaButton)
    {
        SidaButton->OnClicked.RemoveDynamic(this, &UCharacterSelectWidget::SidaButtonClicked);
    }

    if (CloseButton)
    {
        CloseButton->OnClicked.RemoveDynamic(this, &UCharacterSelectWidget::CloseButtonClicked);
    }
}


void UCharacterSelectWidget::UpdateTimer()
{
    // Update the TimerText here
    if (TimerText)
    {
        static int32 TimeRemaining = 60; // Example timer value
        TimerText->SetText(FText::AsNumber(TimeRemaining));
        TimeRemaining--;
    }
}

void UCharacterSelectWidget::RintyButtonClicked()
{
  
    if (UA1GameInstance* GameInstance = Cast<UA1GameInstance>(GetGameInstance()))
    {
       
        GameInstance->CharacterSelect("Rinty");
    }
   
}

void UCharacterSelectWidget::SidaButtonClicked()
{
   
    if (UA1GameInstance* GameInstance = Cast<UA1GameInstance>(GetGameInstance()))
    {
        GameInstance->CharacterSelect("Sida");
    }
   
}

void UCharacterSelectWidget::CloseButtonClicked()
{
    this->RemoveFromViewport();
    SetGameMode();
}