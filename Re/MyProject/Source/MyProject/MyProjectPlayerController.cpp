// Copyright Epic Games, Inc. All Rights Reserved.


#include "MyProjectPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "EnhancedInputSubsystems.h"
#include "Enemy1.h"
#include "MyProjectMyPlayer.h"
#include "MyProjectMyPlayerSida.h"
#include "DrawDebugHelpers.h"
#include "EnemyInfoWidget.h"
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

    static ConstructorHelpers::FClassFinder<UAmmoWidget> AmmoWidgetBPClass(TEXT("/Game/MyBP/UI/AmmoUI.AmmoUI_C"));
    if (AmmoWidgetBPClass.Succeeded())
    {
        AmmoWidgetClass = AmmoWidgetBPClass.Class;
    }

    MaxAmmo = 6;
    CurrentAmmo = MaxAmmo;

    static ConstructorHelpers::FObjectFinder<USoundBase> FireSoundObj(TEXT("/Game/Sound/pistol.pistol"));
    if (FireSoundObj.Succeeded())
    {
        FireSound = FireSoundObj.Object;
    }

    static ConstructorHelpers::FObjectFinder<USoundBase> ReloadSoundObj(TEXT("/Game/Sound/pistolroad.pistolroad"));
    if (ReloadSoundObj.Succeeded())
    {
        ReloadSound = ReloadSoundObj.Object;
    }

    static ConstructorHelpers::FClassFinder<UEnemyInfoWidget> EnemyInfoWidgetBPClass(TEXT("/Game/MyBP/UI/EnemyInfoWidget.EnemyInfoWidget_C"));
    if (EnemyInfoWidgetBPClass.Succeeded())
    {
        EnemyInfoWidgetClass = EnemyInfoWidgetBPClass.Class;
        UE_LOG(LogTemp, Log, TEXT("Successfully found BP_EnemyInfoWidget"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to find BP_EnemyInfoWidget"));
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
            HealthBarWidgets->AddToViewport();
            HealthBarWidgets->NativeConstruct();
            HealthBarWidgets->UpdateHealth(PlayerHealth / 100.0f);
           
        }

        // Create and display AmmoUI
        AmmoWidget = CreateWidget<UAmmoWidget>(this, AmmoWidgetClass);
        if (AmmoWidget)
        {
            UE_LOG(LogTemp, Log, TEXT("Ammo widget created successfully."));
            AmmoWidget->AddToViewport();
            AmmoWidget->NativeConstruct(); 
            AmmoWidget->UpdateAmmoCount(CurrentAmmo, MaxAmmo);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to create Ammo widget."));
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

void AMyProjectPlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FVector CameraLoc;
    FRotator CameraRot;
    GetPlayerViewPoint(CameraLoc, CameraRot); // 플레이어의 카메라 위치와 회전을 가져옴

    FVector Start = CameraLoc + CameraRot.Vector() * 400;
    FVector End = CameraLoc + CameraRot.Vector() * 10000; // 히트스캔 거리 설정

    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(GetPawn()); // 자기 자신은 무시

    bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Pawn, Params);
    if (bHit)
    {
        if (HitResult.GetActor()) // 어떤 액터와 충돌했는지 확인
        {
            ANPC* HitNPC = Cast<ANPC>(HitResult.GetActor());
           

            if (HitNPC) // 충돌한 액터가 AEnemy1인지 확인
            {
                ShowEnemyInfo(HitNPC);
            }
           
            else
            {
                RemoveEnemyInfo(); // 충돌한 액터가 AEnemy1이나 AEnemy2가 아닌 경우
            }
        }
        else
        {
            RemoveEnemyInfo(); // 충돌한 액터가 없는 경우
        }
    }
    else
    {
        RemoveEnemyInfo(); // LineTrace가 실패한 경우
    }
}

void AMyProjectPlayerController::RemoveEnemyInfo()
{
    // 현재 화면에 표시된 위젯 제거
    if (CurrentEnemyInfoWidget)
    {
        CurrentEnemyInfoWidget->RemoveFromViewport();
        CurrentEnemyInfoWidget = nullptr;
    }
}


void AMyProjectPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    InputComponent->BindAction("CharacterSelect", IE_Pressed, this, &AMyProjectPlayerController::ToggleCharacterSelectUI);
    InputComponent->BindAction("Aim", IE_Pressed, this, &AMyProjectPlayerController::OnAimPressed);
    InputComponent->BindAction("Aim", IE_Released, this, &AMyProjectPlayerController::OnAimReleased);

    InputComponent->BindAction("Fire", IE_Pressed, this, &AMyProjectPlayerController::AttemptToFireWeapon);

    InputComponent->BindAction("Reload", IE_Pressed, this, &AMyProjectPlayerController::ReloadWeapon);
    
}


void AMyProjectPlayerController::OnAimPressed()
{
    RequestServerForAimingChange(true);


}

void AMyProjectPlayerController::OnAimReleased()
{
    RequestServerForAimingChange(false);
}



void AMyProjectPlayerController::FireWeapon()
{
    FVector CameraLoc;
    FRotator CameraRot;
    GetPlayerViewPoint(CameraLoc, CameraRot); // 플레이어의 카메라 위치와 회전을 가져옴


    FVector Start = CameraLoc + CameraRot.Vector() * 400;

    FVector End = CameraLoc + CameraRot.Vector() * 10000; // 히트스캔 거리 설정

    /*
    Protocol::C_FIRE FirePkt;
    Protocol::FireInfo* fireInfo = FirePkt.mutable_info();

    AMyProjectMyPlayer* MyCharacter = Cast<AMyProjectMyPlayer>(GetPawn());
    fireInfo->set_object_id(MyCharacter->GetPlayerInfo()->object_id());
    fireInfo->set_s_x(Start.X);
    fireInfo->set_s_y(Start.Y);
    fireInfo->set_s_z(Start.Z);

    fireInfo->set_e_x(End.X);
    fireInfo->set_e_y(End.Y);
    fireInfo->set_e_z(End.Z);
    SEND_PACKET(FirePkt);
    */

    // 디버깅용 라인 그리기 (에디터에서만 보임)
    DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f, 0, 1.0f);


    if (FireSound)
    {

        UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetPawn()->GetActorLocation());
    }



    // 클라이언트
    
    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(GetPawn()); // 자기 자신은 무시

    if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Pawn, Params))
    {
        
        if (ANPC* Enemy = Cast<ANPC>(HitResult.GetActor())) // ANPC로 캐스팅
        {
           //UE_LOG(LogTemp, Log, TEXT("Hit: %s"), *HitResult.GetActor()->GetName());
            
            if (auto* GameInstance = Cast<UMyProjectGameInstance>(GWorld->GetGameInstance()))
            {
                if (GameInstance->monsters.Find(Enemy->MonsterInfo->object_id()))
                {

                    Protocol::C_HIT HitPkt;
                    HitPkt.set_object_id(Enemy->MonsterInfo->object_id());
                    HitPkt.set_on_hit(true);

                    SEND_PACKET(HitPkt);
                }
                
            }

        }
       
    }
    // 디버깅용 라인 그리기 (에디터에서만 보임)
    DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f, 0, 1.0f);
    
}




void AMyProjectPlayerController::ShowEnemyInfo(ANPC* Enemy)
{
    if (!CurrentEnemyInfoWidget && EnemyInfoWidgetClass)
    {
        CurrentEnemyInfoWidget = CreateWidget<UEnemyInfoWidget>(this, EnemyInfoWidgetClass);
        if (CurrentEnemyInfoWidget)
        {
            CurrentEnemyInfoWidget->AddToViewport();
        }
    }

    if (CurrentEnemyInfoWidget)
    {
        CurrentEnemyInfoWidget->SetEnemyName(Enemy->GetNPCName());
        CurrentEnemyInfoWidget->SetEnemyHealth(Enemy->GetHealth() / 100.0f);
    }
}



void AMyProjectPlayerController::ShowEnemyInfo_Internal(FString EnemyName, float Health)
{
    if (!CurrentEnemyInfoWidget && EnemyInfoWidgetClass)
    {
        UE_LOG(LogTemp, Log, TEXT("Attempting to create EnemyInfoWidget"));

        try
        {
            CurrentEnemyInfoWidget = CreateWidget<UEnemyInfoWidget>(this, EnemyInfoWidgetClass);
            UE_LOG(LogTemp, Log, TEXT("CreateWidget called"));

            if (CurrentEnemyInfoWidget)
            {
                CurrentEnemyInfoWidget->AddToViewport();
                UE_LOG(LogTemp, Log, TEXT("Success to add EnemyInfoWidget"));
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Failed to add EnemyInfoWidget"));
            }
        }
        catch (const std::exception& e)
        {
            UE_LOG(LogTemp, Error, TEXT("Exception occurred while creating EnemyInfoWidget: %s"), *FString(e.what()));
        }
    }

    if (CurrentEnemyInfoWidget)
    {
        UE_LOG(LogTemp, Log, TEXT("Setting enemy info: %s"), *EnemyName);
        CurrentEnemyInfoWidget->SetEnemyName(EnemyName);
        CurrentEnemyInfoWidget->SetEnemyHealth(Health / 100.0f);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("CurrentEnemyInfoWidget is null"));
    }
}

void AMyProjectPlayerController::AttemptToFireWeapon()
{
    AMyProjectMyPlayer* MyCharacter = Cast<AMyProjectMyPlayer>(GetPawn());
    if (MyCharacter && MyCharacter->IsAiming())
    {
        if (CurrentAmmo > 0)
        {
            FireWeapon();
            CurrentAmmo--;
            if (AmmoWidget)
            {
                AmmoWidget->UpdateAmmoCount(CurrentAmmo, MaxAmmo);
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("No ammo left to fire."));
        }
    }
}

void AMyProjectPlayerController::ReloadWeapon()
{
    CurrentAmmo = MaxAmmo;
    if (AmmoWidget)
    {
        AmmoWidget->UpdateAmmoCount(CurrentAmmo, MaxAmmo);
    }

    if (ReloadSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, ReloadSound, GetPawn()->GetActorLocation());
    }
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


