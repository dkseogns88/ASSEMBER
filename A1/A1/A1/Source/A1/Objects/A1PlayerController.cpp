// Copyright Epic Games, Inc. All Rights Reserved.


#include "A1PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "../Characters/PlayerChar.h"
#include "../Widgets/CrosshairWidget.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/LocalPlayer.h"
#include "Engine/Engine.h"
#include "AnimInstanceCustom.h"
#include "Sound/SoundBase.h"
#include "Network/A1NetworkManager.h"

AA1PlayerController::AA1PlayerController()
{
    
    PlayerMaxHealth = 300.0f;
    AttackPower = 60.0f;
    PlayerHealth = PlayerMaxHealth;
    MovementSpeed = 500.0f;
    SkillRange = 500.0f;

   
    static ConstructorHelpers::FClassFinder<UHealthBarWidget> HealthBarBPClass(TEXT("/Game/MyBP/Widgets/HealthBarWidget.HealthBarWidget_C"));
    if (HealthBarBPClass.Succeeded())
    {
        HealthBarWidgetClass = HealthBarBPClass.Class;
    }

    static ConstructorHelpers::FClassFinder<UAmmoWidget> AmmoWidgetBPClass(TEXT("/Game/MyBP/Widgets/AmmoWidget.AmmoWidget_C"));
    if (AmmoWidgetBPClass.Succeeded())
    {
        AmmoWidgetClass = AmmoWidgetBPClass.Class;
    }
    static ConstructorHelpers::FClassFinder<USkillCooldownWidget> GunSkillCooldownWidgetBPClass(TEXT("/Game/MyBP/Widgets/GunSkillCooldownWidget.GunSkillCooldownWidget_C"));
    if (GunSkillCooldownWidgetBPClass.Succeeded())
    {
        GunSkillCooldownWidgetClass = GunSkillCooldownWidgetBPClass.Class;
    }
    static ConstructorHelpers::FClassFinder<USkillCooldownWidget> BombSkillCooldownWidgetBPClass(TEXT("/Game/MyBP/Widgets/BombSkillCooldownWidget.BombSkillCooldownWidget_C"));
    if (BombSkillCooldownWidgetBPClass.Succeeded())
    {
        BombSkillCooldownWidgetClass = BombSkillCooldownWidgetBPClass.Class;
    }

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

    static ConstructorHelpers::FClassFinder<UEnemyInfoWidget> EnemyInfoWidgetBPClass(TEXT("/Game/MyBP/Widgets/EnemyInfoWidget.EnemyInfoWidget_C"));
    if (EnemyInfoWidgetBPClass.Succeeded())
    {
        EnemyInfoWidgetClass = EnemyInfoWidgetBPClass.Class;
       
    }
    static ConstructorHelpers::FClassFinder<UUserWidget> LevelUpWidgetBPClass(TEXT("/Game/MyBP/Widgets/LevelUpWidget.LevelUpWidget_C"));
    if (LevelUpWidgetBPClass.Succeeded())
    {
        LevelUpWidgetClass = LevelUpWidgetBPClass.Class;
    }

    static ConstructorHelpers::FClassFinder<UIPAddressWidget> IPAddressWidgetBPClass(TEXT("/Game/MyBP/Widgets/IPAddressWidget.IPAddressWidget_C"));
    if (IPAddressWidgetBPClass.Succeeded())
    {
        IPAddressWidgetClass = IPAddressWidgetBPClass.Class;
       
    }

    //NearbyStatue = nullptr;
	static ConstructorHelpers::FClassFinder<UCrosshairWidget> CrosshairBPClass(TEXT("/Game/MyBP/Widgets/BP_CrosshairWidget.BP_CrosshairWidget_C"));
	if (CrosshairBPClass.Succeeded())
	{
		CrosshairWidgetClass = CrosshairBPClass.Class;
	}

    static ConstructorHelpers::FClassFinder<UPlayerStatWidget> PlayerStatBPClass(TEXT("/Game/MyBP/Widgets/PlayerStatWidget.PlayerStatWidget_C"));
    if (PlayerStatBPClass.Succeeded())
    {
        PlayerStatWidgetClass = PlayerStatBPClass.Class;
    }

    static ConstructorHelpers::FClassFinder<UGameOverWidget> GameOverBPClass(TEXT("/Game/MyBP/Widgets/GameOverWidget.GameOverWidget_C"));
    if (GameOverBPClass.Succeeded())
    {
       GameOverWidgetClass = GameOverBPClass.Class;
    }

    static ConstructorHelpers::FClassFinder<UKeyTipWidget> KeyTipWidgetBPClass(TEXT("/Game/MyBP/Widgets/KeyTipWidget.KeyTipWidget_C"));
    if (KeyTipWidgetBPClass.Succeeded())
    {
        KeyTipWidgetClass = KeyTipWidgetBPClass.Class;
    }


    static ConstructorHelpers::FClassFinder<AGunSkill> SkillBPClass(TEXT("/Game/MyBP/Attack/BP_GunSkill.BP_GunSkill_C"));
    if (SkillBPClass.Succeeded())
    {
        SkillClass = SkillBPClass.Class;
    }
    static ConstructorHelpers::FClassFinder<ABombSkill> BombSkillBPClass(TEXT("/Game/MyBP/Attack/BP_BombSkill.BP_BombSkill_C"));
    if (BombSkillBPClass.Succeeded())
    {
        BombSkillClass = BombSkillBPClass.Class;
    }

    static ConstructorHelpers::FClassFinder<UCharacterSelectWidget> CharacterSelectBPClass(TEXT("/Game/MyBP/Widgets/NewCharacterSelectWidget.NewCharacterSelectWidget_C"));
    if (CharacterSelectBPClass.Succeeded())
    {
        CharacterSelectWidgetClass = CharacterSelectBPClass.Class;
    }

    
}

void AA1PlayerController::AimPressed()
{
    AimingChange(true);

    APlayerChar* MyCharacter = Cast<APlayerChar>(GetPawn());
    Protocol::C_ZOOM ZoomPkt;
    
    Protocol::ZoomInfo* Info = ZoomPkt.mutable_info();
    Info->set_object_id(MyCharacter->GetPosInfo()->object_id());
    Info->set_b_zoom(true);
    
    MyCharacter->GetNetworkManager()->SendPacket(ZoomPkt);
    
}

void AA1PlayerController::AimReleased()
{
    AimingChange(false);

    APlayerChar* MyCharacter = Cast<APlayerChar>(GetPawn());
    Protocol::C_ZOOM ZoomPkt;

    Protocol::ZoomInfo* Info = ZoomPkt.mutable_info();
    Info->set_object_id(MyCharacter->GetPosInfo()->object_id());
    Info->set_b_zoom(false);

    MyCharacter->GetNetworkManager()->SendPacket(ZoomPkt);
}

void AA1PlayerController::BeginPlay()
{
    Super::BeginPlay();

    
    if (CharacterSelectWidgetClass)
    {
        CharacterSelectWidgetInstance = CreateWidget<UCharacterSelectWidget>(this, CharacterSelectWidgetClass);
        if (CharacterSelectWidgetInstance)
        {
            CharacterSelectWidgetInstance->AddToViewport();
            CharacterSelectWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
        }
    }

   
    //Set Player Stat
    InitializeStats(PlayerHealth, MovementSpeed, AttackPower);
    SetHealth(PlayerHealth);

    //Create Widgets
    if (CrosshairWidgetClass)
    {
        CrosshairWidgetInstance = CreateWidget<UCrosshairWidget>(this, CrosshairWidgetClass);
        if (CrosshairWidgetInstance)
        {
            CrosshairWidgetInstance->AddToViewport();
            CrosshairWidgetInstance->ShowCrosshair(false);
        }
    }

    if (PlayerStatWidgetClass)
    {
        PlayerStatWidget = CreateWidget<UPlayerStatWidget>(this, PlayerStatWidgetClass);
        if (PlayerStatWidget)
        {
            PlayerStatWidget->AddToViewport();
            PlayerStatWidget->SetVisibility(ESlateVisibility::Hidden);
        }
    }

    if (KeyTipWidgetClass)
    {
        KeyTipWidgetInstance = CreateWidget<UKeyTipWidget>(this, KeyTipWidgetClass);
        if (KeyTipWidgetInstance)
        {
            KeyTipWidgetInstance->AddToViewport();
            KeyTipWidgetInstance->SetVisibility(ESlateVisibility::Visible);
        }
    }


    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        Subsystem->AddMappingContext(InputMappingContext, 0);
    }
    if (IsLocalController())
    {

        

        HealthBarWidgets = CreateWidget<UHealthBarWidget>(this, HealthBarWidgetClass);
        if (HealthBarWidgets)
        {
           
            HealthBarWidgets->AddToViewport(1);
            HealthBarWidgets->NativeConstruct();
            HealthBarWidgets->UpdateHealth(PlayerHealth);

        }

        

        //Create LevelUpWidget
        LevelUpWidgetInstance = CreateWidget<ULevelUpWidget>(this, LevelUpWidgetClass);

      

        if (GetPawn())
        {
            CharacterBlueprintClass = GetPawn()->GetClass();
           


            if (CharacterBlueprintClass->GetName() == "BP_Rinty_C")
            {
                PlayingRinty = true;
                

            }
            else if (CharacterBlueprintClass->GetName() == "BP_Sida_C")
            {
                PlayingSida = true;
               

            }
        }
        if (PlayingRinty && !PlayingSida)
        {
            //Stat Initialize
            MaxAmmo = 6;
            CurrentAmmo = MaxAmmo;
            SkillPower = (AttackPower * 2.0) + 150;

            if (GunSkillCooldownWidgetClass)
            {
                GunSkillCooldownWidgetInstance = CreateWidget<USkillCooldownWidget>(this, GunSkillCooldownWidgetClass);
                if (GunSkillCooldownWidgetInstance)
                {
                    GunSkillCooldownWidgetInstance->AddToViewport();
                    GunSkillCooldownWidgetInstance->InitializeCooldown(10.0f); // Cooldown duration set to 10 seconds
                }
            }
        }
        if (PlayingSida && !PlayingRinty)
        {
            //Stat Initialize
            MaxAmmo = 10;
            CurrentAmmo = MaxAmmo;
            SkillPower = (AttackPower * 1.5) + 200;

            if (BombSkillCooldownWidgetClass)
            {
                BombSkillCooldownWidgetInstance = CreateWidget<USkillCooldownWidget>(this, BombSkillCooldownWidgetClass);
                if (BombSkillCooldownWidgetInstance)
                {
                    BombSkillCooldownWidgetInstance->AddToViewport();
                    BombSkillCooldownWidgetInstance->InitializeCooldown(10.0f); // Cooldown duration set to 10 seconds
                }
            }
        }

        // Create and display AmmoUI
        AmmoWidget = CreateWidget<UAmmoWidget>(this, AmmoWidgetClass);
        if (AmmoWidget)
        {

            AmmoWidget->AddToViewport(1);
            AmmoWidget->NativeConstruct();
            AmmoWidget->UpdateAmmoCount(CurrentAmmo, MaxAmmo);
        }

       


    }
}

void AA1PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Aim", IE_Pressed, this, &AA1PlayerController::AimPressed);
	InputComponent->BindAction("Aim", IE_Released, this, &AA1PlayerController::AimReleased);
    InputComponent->BindAction("Fire", IE_Pressed, this, &AA1PlayerController::TryFireWeapon);
    InputComponent->BindAction("Reload", IE_Pressed, this, &AA1PlayerController::ReloadWeapon);

    InputComponent->BindAction("UseSkill", IE_Pressed, this, &AA1PlayerController::UseSkill);

    InputComponent->BindAction("LevelUpUI", IE_Pressed, this, &AA1PlayerController::ShowLevelUpUI);

    InputComponent->BindAction("ToggleIPAddressWidget", IE_Pressed, this, &AA1PlayerController::ToggleIPAddressWidget);

    InputComponent->BindAction("InteractStatue", IE_Pressed, this, &AA1PlayerController::Interact);

    InputComponent->BindAction("TogglePlayerStats", IE_Pressed, this, &AA1PlayerController::TogglePlayerStatWidget);
    InputComponent->BindAction("UseBombSkill", IE_Pressed, this, &AA1PlayerController::UseBombSkill);
    InputComponent->BindAction("ThrowBomb", IE_Pressed, this, &AA1PlayerController::ThrowBomb);
    InputComponent->BindAction("ToggleKeyTips", IE_Pressed, this, &AA1PlayerController::ToggleKeyTips);
    InputComponent->BindAction("ToggleCharacterSelect", IE_Pressed, this, &AA1PlayerController::ToggleCharacterSelectWidget);
   
}

void AA1PlayerController::AimingChange(bool bIsAiming)
{
    APlayerChar* MyCharacter = Cast<APlayerChar>(GetPawn());
    if (MyCharacter)
    {
        MyCharacter->SetAiming(bIsAiming);

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

//GetCameraCenterLocation for Shoot
FVector AA1PlayerController::GetCamCenLoc(FVector& CameraLocation, FRotator& CameraRotation)
{
    GetPlayerViewPoint(CameraLocation, CameraRotation);

    FVector Start = CameraLocation + CameraRotation.Vector();
    FVector End = CameraLocation + CameraRotation.Vector() * 10000.0f; 

    DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f, 0, 1.0f);

    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(GetPawn()); 

    if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
    {
        return HitResult.Location;
    }

    return End;
}

void AA1PlayerController::TryFireWeapon()
{

    APlayerChar* MyCharacter = Cast<APlayerChar>(GetPawn());
    if (MyCharacter && MyCharacter->IsAiming())
    {
        if (CurrentAmmo > 0)
        {
            if (!GetWorld()->GetTimerManager().IsTimerActive(FireCooldownTimerHandle))
            {
                FireWeapon();
                CurrentAmmo--;
                if (AmmoWidget)
                {
                    AmmoWidget->UpdateAmmoCount(CurrentAmmo, MaxAmmo);
                }

                // Cooltime at shoot
                GetWorld()->GetTimerManager().SetTimer(FireCooldownTimerHandle, 0.5f, false);
            }
        }
    }

}

void AA1PlayerController::FireWeapon()
{
    FVector CameraLoc;
    FRotator CameraRot;
    FVector TargetLocation = GetCamCenLoc(CameraLoc, CameraRot);
    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(GetPawn());

    if (FireSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetPawn()->GetActorLocation());
    }

    APlayerChar* MyCharacter = Cast<APlayerChar>(GetPawn());
    if (MyCharacter)
    {
        MyCharacter->SpawnMuzzleFlash();
    }
   
    if (GetWorld()->LineTraceSingleByChannel(HitResult, CameraLoc, TargetLocation, ECC_Pawn, Params))
    {
        //서버충돌처리
        AMonster* HitMonster = Cast<AMonster>(HitResult.GetActor());
        if (HitMonster)
        {
            // Dealing Monster
            // HitMonster->TakeDMG(AttackPower);
            
            
            
            Protocol::C_ATTACK AttackPkt;
            Protocol::AttackInfo* Info = AttackPkt.mutable_info();
            Info->set_attack_object_id(MyCharacter->GetPosInfo()->object_id());
            Info->set_hit_object_id(HitMonster->PosInfo->object_id());
            Info->set_attack_type(Protocol::AttackType::ATTACK_TYPE_BASIC);

            GetGameInstance()->GetSubsystem<UA1NetworkManager>()->SendPacket(AttackPkt);
        }
        
    }
 
}





void AA1PlayerController::SetHealth(float NewHealth)
{
    PlayerHealth = NewHealth;
    if (HealthBarWidgets)
    {
        HealthBarWidgets->UpdateHealth(PlayerHealth);  
    }
   
}

void AA1PlayerController::InitializeStats(float InitialHealth, float InitialMovementSpeed, float InitialAttackPower)
{
    PlayerHealth = InitialHealth;
    MovementSpeed = InitialMovementSpeed;
    AttackPower = InitialAttackPower;
  

   
    if (APawn* ControlledPawn = GetPawn())
    {
        if (APlayerChar* Players = Cast<APlayerChar>(ControlledPawn))
        {
            Players->SetMovementSpeed(MovementSpeed);
        }
    }
}

void AA1PlayerController::UpdateStats(float NewHealth, float NewMovementSpeed, float NewAttackPower, float NewMaxHealth)
{
    PlayerHealth = NewHealth;
    PlayerMaxHealth = NewMaxHealth;
    MovementSpeed = NewMovementSpeed;
    AttackPower = NewAttackPower;
    
    

    if (APawn* ControlledPawn = GetPawn())
    {
        if (APlayerChar* Players = Cast<APlayerChar>(ControlledPawn))
        {
            Players->SetMovementSpeed(MovementSpeed);
            HealthBarWidgets->UpdateHealth(PlayerHealth);
        }
    }
}

void AA1PlayerController::ShowLevelUpUI()
{
    if (LevelUpWidgetInstance && !LevelUpWidgetInstance->IsInViewport())
    {
        LevelUpWidgetInstance->ShowWidget();
        bShowMouseCursor = true;
        SetInputMode(FInputModeGameAndUI());
        
    }
}

void AA1PlayerController::TogglePlayerStatWidget()
{
    if (PlayerStatWidget)
    {
        if (PlayerStatWidget->IsVisible())
        {
            PlayerStatWidget->SetVisibility(ESlateVisibility::Hidden);
        }
        else
        {
            PlayerStatWidget->SetVisibility(ESlateVisibility::Visible);
            PlayerStatWidget->UpdateStats(PlayerHealth, MovementSpeed, AttackPower);
        }
    }
}

void AA1PlayerController::ToggleKeyTips()
{
    if (KeyTipWidgetInstance)
    {
        KeyTipWidgetInstance->ToggleVisibility();
    }
}


void AA1PlayerController::ApplyDamage(float DamageAmount)
{
    PlayerHealth -= DamageAmount;
    //When Player die
    if (PlayerHealth <= 0)
    {
        if (APawn* ControlledPawn = GetPawn())
        {
            ControlledPawn->Destroy();
        }
        if (GameOverWidgetClass)
        {
            GameOverWidgetInstance = CreateWidget<UGameOverWidget>(this, GameOverWidgetClass);
            if (GameOverWidgetInstance)
            {
                GameOverWidgetInstance->AddToViewport();

                // Switch to UI mode
                SetInputMode(FInputModeUIOnly());
                bShowMouseCursor = true;
            }
        }
        
    }
    HealthBarWidgets->UpdateHealth(PlayerHealth);
}

void AA1PlayerController::Tick(float DeltaTime)
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
            AMonster* HitNPC = Cast<AMonster>(HitResult.GetActor());


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

    if (PlayerStatWidget && PlayerStatWidget->IsVisible())
    {
        PlayerStatWidget->UpdateStats(PlayerHealth, MovementSpeed, AttackPower);
    }
}

void AA1PlayerController::RemoveEnemyInfo()
{
    if (CurrentEnemyInfoWidget)
    {
        CurrentEnemyInfoWidget->HideWidget();
        CurrentEnemyInfoWidget = nullptr;
    }
}

void AA1PlayerController::ShowEnemyInfo(AMonster* Enemy)
{
    if (!CurrentEnemyInfoWidget && EnemyInfoWidgetClass)
    {
        CurrentEnemyInfoWidget = CreateWidget<UEnemyInfoWidget>(this, EnemyInfoWidgetClass);
        if (CurrentEnemyInfoWidget)
        {
            CurrentEnemyInfoWidget->AddToViewport(1);
        }
    }

    if (CurrentEnemyInfoWidget)
    {
        CurrentEnemyInfoWidget->SetEnemyName(Enemy->GetMonsterName());
        CurrentEnemyInfoWidget->SetEnemyHealth(Enemy->GetHealth() / 100.0f);
    }
}

void AA1PlayerController::ReloadWeapon()
{
    if (!GetWorld()->GetTimerManager().IsTimerActive(ReloadCooldownTimerHandle))
    {
        
        GetWorld()->GetTimerManager().SetTimer(ReloadCooldownTimerHandle, [this]()
            {
                //서버 장전처리
                CurrentAmmo = MaxAmmo;
                if (AmmoWidget)
                {
                    AmmoWidget->UpdateAmmoCount(CurrentAmmo, MaxAmmo);
                }
                
                GetWorld()->GetTimerManager().ClearTimer(ReloadCooldownTimerHandle);

            }, 1.0f, false);
        if (ReloadSound)
        {
            UGameplayStatics::PlaySoundAtLocation(this, ReloadSound, GetPawn()->GetActorLocation());
        }
    }
}

void AA1PlayerController::HandleLevelUpOption(int OptionIndex)
{
    UpdateStatsBasedOnOption(OptionIndex);
    if (LevelUpWidgetInstance && LevelUpWidgetInstance->IsInViewport())
    {
        LevelUpWidgetInstance->RemoveFromParent();
        bShowMouseCursor = false;
        SetInputMode(FInputModeGameOnly());
       
    }
}

void AA1PlayerController::ToggleIPAddressWidget()
{
    

    if (!IPAddressWidgetClass)
    {
        
        return;
    }

    if (!IPAddressWidget)
    {
        IPAddressWidget = CreateWidget<UIPAddressWidget>(this, IPAddressWidgetClass);
        if (IPAddressWidget)
        {
           
            IPAddressWidget->AddToViewport(100);
            IPAddressWidget->SetVisibility(ESlateVisibility::Collapsed);
        }
        else 
        {
           
        }
    }

    if (bIsIPWidgetVisible)
    {
        IPAddressWidget->SetVisibility(ESlateVisibility::Collapsed);
        bShowMouseCursor = false;
        SetInputMode(FInputModeGameOnly());
       
    }
    else
    {
        IPAddressWidget->SetVisibility(ESlateVisibility::Visible);
        bShowMouseCursor = true;
        SetInputMode(FInputModeUIOnly());
        
    }

    bIsIPWidgetVisible = !bIsIPWidgetVisible;
}

void AA1PlayerController::ToggleCharacterSelectWidget()
{
    if (!CharacterSelectWidgetInstance)
    {
        // 위젯이 생성되지 않았으면 생성
        CharacterSelectWidgetInstance = CreateWidget<UCharacterSelectWidget>(this, CharacterSelectWidgetClass);
        if (CharacterSelectWidgetInstance)
        {
            CharacterSelectWidgetInstance->AddToViewport();
            CharacterSelectWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
        }
    }

    if (CharacterSelectWidgetInstance)
    {
        if (!CharacterSelectWidgetInstance->IsInViewport())
        {
            // 위젯을 뷰포트에 추가하고 입력 모드를 설정
            CharacterSelectWidgetInstance->AddToViewport();
            CharacterSelectWidgetInstance->SetVisibility(ESlateVisibility::Visible);
            SetInputMode(FInputModeGameAndUI());
            bShowMouseCursor = true;
        }
        else
        {
            // 위젯을 뷰포트에서 제거하고 입력 모드를 게임 전용으로 설정
            CharacterSelectWidgetInstance->RemoveFromParent();
            SetInputMode(FInputModeGameOnly());
            bShowMouseCursor = false;
        }
    }
}

void AA1PlayerController::UpdateStatsAndWidgets(FString CharacterName)
{
    if (GunSkillCooldownWidgetInstance)
    {
        GunSkillCooldownWidgetInstance->RemoveFromParent();
        GunSkillCooldownWidgetInstance = nullptr;
    }

    if (BombSkillCooldownWidgetInstance)
    {
        BombSkillCooldownWidgetInstance->RemoveFromParent();
        BombSkillCooldownWidgetInstance = nullptr;
    }

    if (CharacterName == "Rinty")
    {
        PlayingSida = false;
        PlayingRinty = true;
        // Rinty의 스탯 및 스킬 설정
        PlayerMaxHealth = 300.0f;
        AttackPower = 60.0f;
        MovementSpeed = 600.0f;
        SkillRange = 1000.0f;
        SkillPower = (AttackPower * 2.0) + 150;
        // 위젯 업데이트
        if (HealthBarWidgets)
        {
            HealthBarWidgets->UpdateHealth(PlayerMaxHealth);
        }
        if (AmmoWidget)
        {
            MaxAmmo = 6;
            CurrentAmmo = MaxAmmo;
            AmmoWidget->UpdateAmmoCount(CurrentAmmo, MaxAmmo);
        }
        if (GunSkillCooldownWidgetClass)
        {
            GunSkillCooldownWidgetInstance = CreateWidget<USkillCooldownWidget>(this, GunSkillCooldownWidgetClass);
            if (GunSkillCooldownWidgetInstance)
            {
                GunSkillCooldownWidgetInstance->AddToViewport();
                
                GunSkillCooldownWidgetInstance->InitializeCooldown(10.0f);
                
            }
            
        }
    }
    else if (CharacterName == "Sida")
    {
        PlayingRinty = false;
        PlayingSida = true;
        // Sida의 스탯 및 스킬 설정
        PlayerMaxHealth = 300.0f;
        AttackPower = 70.0f;
        MovementSpeed = 500.0f;
        SkillRange = 1000.0f;
        SkillPower = (AttackPower * 1.0) + 200;
        // 위젯 업데이트
        if (HealthBarWidgets)
        {
            HealthBarWidgets->UpdateHealth(PlayerMaxHealth);
        }
        if (AmmoWidget)
        {
            MaxAmmo = 10;
            CurrentAmmo = MaxAmmo;
            AmmoWidget->UpdateAmmoCount(CurrentAmmo, MaxAmmo);
        }
        if (BombSkillCooldownWidgetClass)
        {
            BombSkillCooldownWidgetInstance = CreateWidget<USkillCooldownWidget>(this, BombSkillCooldownWidgetClass);
            if (BombSkillCooldownWidgetInstance)
            {
                BombSkillCooldownWidgetInstance->AddToViewport();
                BombSkillCooldownWidgetInstance->ResetCooldown();
                BombSkillCooldownWidgetInstance->InitializeCooldown(10.0f);
               
            }
           
        }
    }

    // 기타 업데이트 로직 추가 가능
    SetHealth(PlayerMaxHealth);
    InitializeStats(PlayerMaxHealth, MovementSpeed, AttackPower);
}

void AA1PlayerController::ShowEnemyInfo_Internal(FString EnemyName, float Health)
{
    if (!CurrentEnemyInfoWidget && EnemyInfoWidgetClass)
    {
      

        try
        {
            CurrentEnemyInfoWidget = CreateWidget<UEnemyInfoWidget>(this, EnemyInfoWidgetClass);
           

            if (CurrentEnemyInfoWidget)
            {
                CurrentEnemyInfoWidget->AddToViewport(1);
               
            }
           
        }
        catch (const std::exception& e)
        {
            UE_LOG(LogTemp, Error, TEXT("Exception occurred while creating EnemyInfoWidget: %s"), *FString(e.what()));
        }
    }

    if (CurrentEnemyInfoWidget)
    {
       // UE_LOG(LogTemp, Log, TEXT("Setting enemy info: %s"), *EnemyName);
        CurrentEnemyInfoWidget->SetEnemyName(EnemyName);
        CurrentEnemyInfoWidget->SetEnemyHealth(Health / 100.0f);
    }
    
}

void AA1PlayerController::UpdateStatsBasedOnOption(int OptionIndex)
{
    float NewHealth = PlayerHealth;
    float NewMaxHealth = PlayerMaxHealth;
    float NewMovementSpeed = MovementSpeed;
    float NewAttackPower = AttackPower;

    switch (OptionIndex)
    {
    case 1:
        NewAttackPower *= 1.20f; // Increase attack power by 20%
        break;
    case 2:
        NewMaxHealth *= 1.15f; // Increase health by 15%
        NewHealth = PlayerHealth + 0.15f * (PlayerMaxHealth);
        break;
    case 3:
        NewHealth = NewMaxHealth; // Restore health to 100%
        break;
    case 4:
        NewMovementSpeed *= 1.10f; // Increase movement speed by 10%
        break;
    case 5:
    {
        int RandomStat = FMath::RandRange(1, 3);
        if (RandomStat == 1)
            NewAttackPower *= 1.25f; // Increase attack power by 25%
        else if (RandomStat == 2)
            NewMaxHealth *= 1.25f; // Increase health by 25%
        else
            NewMovementSpeed *= 1.25f; // Increase movement speed by 25%
        break;
    }
    default:
        break;
    }

    // Update all stats and sync with character
    UpdateStats(NewHealth, NewMovementSpeed, NewAttackPower,NewMaxHealth);

   
}

void AA1PlayerController::Interact()
{
    /*if (NearbyStatue)
    {
        NearbyStatue->OnInteract(this);
    }*/
}

void AA1PlayerController::UseSkill()
{
    if (SkillClass && PlayingRinty && !bIsSkillOnCooldown)
    {
        APlayerChar* PlayerChar = Cast<APlayerChar>(GetPawn());
        if (PlayerChar && !(PlayerChar->bIsJumping))
        {
            PlayerChar->UseSkillAnim(true); 
            FTimerHandle TimerHandle;
            GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, PlayerChar]()
                {
                    FVector Location = GetPawn()->GetActorLocation();
                    FRotator Rotation = GetPawn()->GetActorRotation();
                    FActorSpawnParameters SpawnParams;
                    SpawnParams.Owner = this;

                    AGunSkill* Skill = GetWorld()->SpawnActor<AGunSkill>(SkillClass, Location, Rotation, SpawnParams);
                    if (Skill)
                    {
                        Skill->OnSkillEnd.AddDynamic(this, &AA1PlayerController::OnSkillEnd);
                        Skill->InitializeSkill(GetPawn(),Location, SkillRange, SkillPower);  
                       
                    }
                   
                }, 1.0f, false); 

            
            bIsSkillOnCooldown = true;
            if (GunSkillCooldownWidgetInstance)
            {
                GunSkillCooldownWidgetInstance->StartCooldown();
               
            }
            
            GetWorld()->GetTimerManager().SetTimer(SkillCooldownTimerHandle, [this]()
                {
                    bIsSkillOnCooldown = false;
                    if (GunSkillCooldownWidgetInstance)
                    {
                        GunSkillCooldownWidgetInstance->ResetCooldown();
                       
                    }
                   

                }, 10.0f, false);

        }
    }
   
}

void AA1PlayerController::OnSkillEnd()
{
    APlayerChar* PlayerChar = Cast<APlayerChar>(GetPawn());
    if (PlayerChar)
    {
        PlayerChar->UseSkillAnim(false);
    }
}

void AA1PlayerController::UseBombSkill()
{
    if (BombSkillClass && PlayingSida && !bIsBombSkillOnCooldown)
    {
        FVector CameraLoc;
        FRotator CameraRot;
        FVector TargetLocation = GetCamCenLoc(CameraLoc, CameraRot);

        APlayerChar* PlayerChar = Cast<APlayerChar>(GetPawn());
        if (PlayerChar)
        {
            PlayerChar->UseSkillAnim(true); 

            FVector Location = GetPawn()->GetActorLocation();
            FRotator Rotation = CameraRot;
            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = this;

            AActor* SpawnedActor = GetWorld()->SpawnActor(BombSkillClass, &Location, &Rotation, SpawnParams);
            CurrentBombSkill = Cast<ABombSkill>(SpawnedActor);
            if (CurrentBombSkill)
            {
                CurrentBombSkill->OnSkillEnd.AddDynamic(this, &AA1PlayerController::OnSkillEnd);
                CurrentBombSkill->InitializeSkill(GetPawn(), TargetLocation, SkillRange, SkillPower);  // 폭탄 스킬 설정
               
            }
           

            
            bIsBombSkillOnCooldown = true;
            BombSkillCooldownWidgetInstance->StartCooldown();
            GetWorld()->GetTimerManager().SetTimer(SkillCooldownTimerHandle, [this]()
                {
                    bIsBombSkillOnCooldown = false;
                    BombSkillCooldownWidgetInstance->ResetCooldown();
                    
                }, 10.0f, false); 
        }
    }
   
}

void AA1PlayerController::ThrowBomb()
{
    if (CurrentBombSkill)
    {
        FVector CameraLoc;
        FRotator CameraRot;
        GetCamCenLoc(CameraLoc, CameraRot);

        FVector LaunchDirection = CameraRot.Vector();
        FVector LaunchVelocity = LaunchDirection * 3000.0f; 

        CurrentBombSkill->ThrowBomb(LaunchVelocity);
        CurrentBombSkill = nullptr;
    }
}