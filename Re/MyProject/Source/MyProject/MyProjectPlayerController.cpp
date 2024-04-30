// Copyright Epic Games, Inc. All Rights Reserved.


#include "MyProjectPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "EnhancedInputSubsystems.h"
#include "MyProjectMyPlayer.h"
#include "MyProjectMyPlayerSida.h"
#include "HealthBarWidgets.h"
#include "Components/InputComponent.h"
#include "MyProject.h"
#include "Protocol.pb.h"



AMyProjectPlayerController::AMyProjectPlayerController()
{
    // 위젯 블루프린트 클래스 로드
    static ConstructorHelpers::FClassFinder<UHealthBarWidgets> HealthBarBPClass(TEXT("/Game/MyBP/UI/HealthBarWidgets.HealthBarWidgets_C"));
    if (HealthBarBPClass.Succeeded())
    {
        HealthBarWidgetClass = HealthBarBPClass.Class;
    }
}



void AMyProjectPlayerController::BeginPlay()
{
    Super::BeginPlay();

    
    
    // get the enhanced input subsystem
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        Subsystem->AddMappingContext(InputMappingContext, 0);

        UE_LOG(LogTemp, Warning, TEXT("BeginPlay"));


    }


    // Create and display the health bar widget
    if (IsLocalController())
    {
        HealthBarWidgets = CreateWidget<UHealthBarWidgets>(this, HealthBarWidgetClass);
        if (HealthBarWidgets)
        {
            UE_LOG(LogTemp, Log, TEXT("Health bar widget created successfully."));
            HealthBarWidgets->AddToViewport(1);
            // Ensuring NativeConstruct is called right after adding to viewport
            HealthBarWidgets->NativeConstruct();
            HealthBarWidgets->UpdateHealth(PlayerHealth / 100.0f);
           
        }
    }
}

void AMyProjectPlayerController::SetHealth(float NewHealth)
{
    PlayerHealth = NewHealth;
    if (HealthBarWidgets)
    {
        HealthBarWidgets->UpdateHealth(PlayerHealth / 100.0f);
    }
}





void AMyProjectPlayerController::RequestServerForCharacterChange(FString CharacterName)
{
    // 서버에 캐릭터 변경 요청
    Protocol::C_SELECT SelectPkt;
    std::string SeletName = TCHAR_TO_ANSI(*CharacterName);
    SelectPkt.set_msg(SeletName);

    SEND_PACKET(SelectPkt);
    

    UE_LOG(LogTemp, Log, TEXT("Requested server for character change to: %s"), *CharacterName);
}

void AMyProjectPlayerController::RequestServerForAimingChange(bool bIsAiming)
{
   
    AMyProjectMyPlayer* MyCharacter = Cast<AMyProjectMyPlayer>(GetPawn());
    if (MyCharacter)
    {
        MyCharacter->SetAiming(bIsAiming);
        UE_LOG(LogTemp, Log, TEXT("Aiming set"));



        if (bIsAiming)
        {
            if (!AimUIInstance && AimUIClass)
            {
                AimUIInstance = CreateWidget<UUserWidget>(this,AimUIClass);
            }
            if (AimUIInstance && !AimUIInstance->IsInViewport())
            {
                AimUIInstance->AddToViewport();
            }
        }
        else
        {
            if (AimUIInstance && AimUIInstance->IsInViewport())
            {
                AimUIInstance->RemoveFromViewport();
            }
        }


        Protocol::C_ZOOM ZoomPkt;

        Protocol::ZoomInfo* Info = ZoomPkt.mutable_info();
        Info->set_object_id(MyCharacter->GetPlayerInfo()->object_id());
        Info->set_b_zoom(bIsAiming);

        SEND_PACKET(ZoomPkt);
    }

    UE_LOG(LogTemp, Log, TEXT("Requested server for aiming change: %s"), bIsAiming ? TEXT("True") : TEXT("False"));
}




void AMyProjectPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    InputComponent->BindAction("CharacterSelect", IE_Pressed, this, &AMyProjectPlayerController::ToggleCharacterSelectUI);
    InputComponent->BindAction("Aim", IE_Pressed, this, &AMyProjectPlayerController::OnAimPressed);
    InputComponent->BindAction("Aim", IE_Released, this, &AMyProjectPlayerController::OnAimReleased);
    
}


void AMyProjectPlayerController::OnAimPressed()
{
    RequestServerForAimingChange(true);


}

void AMyProjectPlayerController::OnAimReleased()
{
    RequestServerForAimingChange(false);
}

void AMyProjectPlayerController::ToggleCharacterSelectUI()
{
    //UI의 기본 false
    if (!bIsUIActive)   //UI가 켜지면
    {
        if (!CharacterSelectWidgetInstance) //UI없을때
        {
            CharacterSelectWidgetInstance = CreateWidget<UUserWidget>(this, CharacterSelectWidgetClass);    //UI생성
        }

        if (CharacterSelectWidgetInstance)  //UI가있을때
        {
            if (!CharacterSelectWidgetInstance->IsInViewport())
            {
                CharacterSelectWidgetInstance->AddToViewport();
                bShowMouseCursor = true;
                SetInputMode(FInputModeGameAndUI());
                UE_LOG(LogTemp, Log, TEXT("Changed to UIMODE"));
                bIsUIActive = true; // UI가 활성화되었다고 상태 업데이트
                UE_LOG(LogTemp, Log, TEXT("bisUIActive is true"));
            }
            else
            {
                CharacterSelectWidgetInstance->RemoveFromViewport();
                bShowMouseCursor = false;
                SetInputMode(FInputModeGameOnly());
                UE_LOG(LogTemp, Log, TEXT("Changed to GameMODE"));
                bIsUIActive = false; // UI가 비활성화되었다고 상태 업데이트
                UE_LOG(LogTemp, Log, TEXT("bisUIActive is false"));
            }
        }
    }
}


/* Rinty가 서버로 정보가 넘어가지 않음*/