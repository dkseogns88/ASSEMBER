// Copyright Epic Games, Inc. All Rights Reserved.


#include "A1PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerChar.h"
#include "CrosshairWidget.h"
#include "Engine/LocalPlayer.h"

AA1PlayerController::AA1PlayerController()
{
    
	static ConstructorHelpers::FClassFinder<UCrosshairWidget> CrosshairBPClass(TEXT("/Game/BP/BP_CrosshairWidget.BP_CrosshairWidget_C"));
	if (CrosshairBPClass.Succeeded())
	{
		CrosshairWidgetClass = CrosshairBPClass.Class;
	}
}

void AA1PlayerController::AimPressed()
{
    AimingChange(true);
}

void AA1PlayerController::AimReleased()
{
    AimingChange(false);
}

void AA1PlayerController::BeginPlay()
{
	Super::BeginPlay();

    if (CrosshairWidgetClass)
    {
        CrosshairWidgetInstance = CreateWidget<UCrosshairWidget>(this, CrosshairWidgetClass);
        if (CrosshairWidgetInstance)
        {
            CrosshairWidgetInstance->AddToViewport();
            CrosshairWidgetInstance->ShowCrosshair(false);
        }
    }

    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        Subsystem->AddMappingContext(InputMappingContext, 0);
    }
}

void AA1PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Aim", IE_Pressed, this, &AA1PlayerController::AimPressed);
	InputComponent->BindAction("Aim", IE_Released, this, &AA1PlayerController::AimReleased);
    InputComponent->BindAction("Fire", IE_Pressed, this, &AA1PlayerController::TryFireWeapon);

}

void AA1PlayerController::AimingChange(bool bIsAiming)
{
    APlayerChar* MyCharacter = Cast<APlayerChar>(GetPawn());
    if (MyCharacter)
    {
        MyCharacter->SetAiming(bIsAiming);
        UE_LOG(LogTemp, Log, TEXT("Aiming set to: %s"), bIsAiming ? TEXT("True") : TEXT("False"));

        if (CrosshairWidgetInstance)
        {
            CrosshairWidgetInstance->ShowCrosshair(bIsAiming);
            if (bIsAiming)
            {
                CrosshairWidgetInstance->ShowCrosshair(bIsAiming);
               
            }
            else
            {
                CrosshairWidgetInstance->ShowCrosshair(bIsAiming);
                
            }
        }
        
    }
    
}

void AA1PlayerController::FireWeapon()
{
    FVector CameraLoc;
    FRotator CameraRot;
    GetPlayerViewPoint(CameraLoc, CameraRot); // �÷��̾��� ī�޶� ��ġ�� ȸ���� ������


    FVector Start = CameraLoc + CameraRot.Vector() * 400;

    FVector End = CameraLoc + CameraRot.Vector() * 10000; // ��Ʈ��ĵ �Ÿ� ����

    // ������ ���� �׸��� (�����Ϳ����� ����)
    DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f, 0, 1.0f);


   



    // Ŭ���̾�Ʈ

    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(GetPawn()); // �ڱ� �ڽ��� ����

    if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Pawn, Params))
    {

        //�����浹ó�� ����
    }
    // ������ ���� �׸��� (�����Ϳ����� ����)
    DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f, 0, 1.0f);

}

void AA1PlayerController::TryFireWeapon()
{
    APlayerChar* MyCharacter = Cast<APlayerChar>(GetPawn());
    if (MyCharacter && MyCharacter->IsAiming())
    {
        FireWeapon();
    }

}