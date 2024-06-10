// Copyright Epic Games, Inc. All Rights Reserved.


#include "A1PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "../Characters/PlayerChar.h"
#include "../Widgets/CrosshairWidget.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/LocalPlayer.h"

AA1PlayerController::AA1PlayerController()
{
    // 위젯 블루프린트 클래스 로드
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

    MaxAmmo = 6;
    CurrentAmmo = MaxAmmo;

 /*   static ConstructorHelpers::FObjectFinder<USoundBase> FireSoundObj(TEXT("/Game/Sound/pistol.pistol"));
    if (FireSoundObj.Succeeded())
    {
        FireSound = FireSoundObj.Object;
    }

    static ConstructorHelpers::FObjectFinder<USoundBase> ReloadSoundObj(TEXT("/Game/Sound/pistolroad.pistolroad"));
    if (ReloadSoundObj.Succeeded())
    {
        ReloadSound = ReloadSoundObj.Object;
    }*/

    static ConstructorHelpers::FClassFinder<UEnemyInfoWidget> EnemyInfoWidgetBPClass(TEXT("/Game/MyBP/Widgets/EnemyInfoWidget.EnemyInfoWidget_C"));
    if (EnemyInfoWidgetBPClass.Succeeded())
    {
        EnemyInfoWidgetClass = EnemyInfoWidgetBPClass.Class;
        UE_LOG(LogTemp, Log, TEXT("Successfully found BP_EnemyInfoWidget"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to find BP_EnemyInfoWidget"));
    }

    static ConstructorHelpers::FClassFinder<UUserWidget> LevelUpWidgetBPClass(TEXT("/Game/MyBP/Widgets/LevelUpWidget.LevelUpWidget_C"));
    if (LevelUpWidgetBPClass.Succeeded())
    {
        LevelUpWidgetClass = LevelUpWidgetBPClass.Class;
    }

    //SkillManager = CreateDefaultSubobject<USkillManager>(TEXT("SkillManager"));

    ////스킬 추가
    //USkill* Hellgun = NewObject<USkill>();
    //Hellgun->Initialize("Hellgun", 300.0f, 50.0f);
    //SkillManager->AddSkill(Hellgun);

    static ConstructorHelpers::FClassFinder<UIPAddressWidget> IPAddressWidgetBPClass(TEXT("/Game/MyBP/Widgets/IPAddressWidget.IPAddressWidget_C"));
    if (IPAddressWidgetBPClass.Succeeded())
    {
        IPAddressWidgetClass = IPAddressWidgetBPClass.Class;
        UE_LOG(LogTemp, Warning, TEXT("Successfully found BP_IPAddressWidget"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to find BP_IPAddressWidget"));
    }

    //NearbyStatue = nullptr;
	static ConstructorHelpers::FClassFinder<UCrosshairWidget> CrosshairBPClass(TEXT("/Game/MyBP/Widgets/BP_CrosshairWidget.BP_CrosshairWidget_C"));
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
    InitializeStats(100.0f, 500.0f, 20.0f);


    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        Subsystem->AddMappingContext(InputMappingContext, 0);
    }

    if (IsLocalController())
    {
        HealthBarWidgets = CreateWidget<UHealthBarWidget>(this, HealthBarWidgetClass);
        if (HealthBarWidgets)
        {
            UE_LOG(LogTemp, Log, TEXT("Health bar widget created successfully."));
            HealthBarWidgets->AddToViewport(1);
            HealthBarWidgets->NativeConstruct();
            HealthBarWidgets->UpdateHealth(PlayerHealth / 100.0f);

        }

        // Create and display AmmoUI
        AmmoWidget = CreateWidget<UAmmoWidget>(this, AmmoWidgetClass);
        if (AmmoWidget)
        {
            UE_LOG(LogTemp, Log, TEXT("Ammo widget created successfully."));
            AmmoWidget->AddToViewport(1);
            AmmoWidget->NativeConstruct();
            AmmoWidget->UpdateAmmoCount(CurrentAmmo, MaxAmmo);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to create Ammo widget."));
        }

        //Create LevelUpWidget
        LevelUpWidgetInstance = CreateWidget<ULevelUpWidget>(this, LevelUpWidgetClass);

    }

    SetHealth(PlayerHealth);

}

void AA1PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Aim", IE_Pressed, this, &AA1PlayerController::AimPressed);
	InputComponent->BindAction("Aim", IE_Released, this, &AA1PlayerController::AimReleased);
    InputComponent->BindAction("Fire", IE_Pressed, this, &AA1PlayerController::TryFireWeapon);
    InputComponent->BindAction("Reload", IE_Pressed, this, &AA1PlayerController::ReloadWeapon);

    //InputComponent->BindAction("UseSkill", IE_Pressed, this, &AA1PlayerController::UseSkill);

    InputComponent->BindAction("LevelUpUI", IE_Pressed, this, &AA1PlayerController::ShowLevelUpUI);

    InputComponent->BindAction("ToggleIPAddressWidget", IE_Pressed, this, &AA1PlayerController::ToggleIPAddressWidget);

    InputComponent->BindAction("InteractStatue", IE_Pressed, this, &AA1PlayerController::Interact);

    //InputComponent->BindAction("Roll", IE_Pressed, this, &AA1PlayerController::OnRollPressed);

   
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
    GetPlayerViewPoint(CameraLoc, CameraRot); // 플레이어의 카메라 위치와 회전을 가져옴


    FVector Start = CameraLoc + CameraRot.Vector() * 400;

    FVector End = CameraLoc + CameraRot.Vector() * 10000; // 히트스캔 거리 설정

    // 디버깅용 라인 그리기 (에디터에서만 보임)
    DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f, 0, 1.0f);


   



    // 클라이언트

    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(GetPawn()); // 자기 자신은 무시

    if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Pawn, Params))
    {

        //몬스터충돌처리 예정
    }
    // 디버깅용 라인 그리기 (에디터에서만 보임)
    DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f, 0, 1.0f);

}

void AA1PlayerController::TryFireWeapon()
{
    
    APlayerChar* MyCharacter = Cast<APlayerChar>(GetPawn());
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

void AA1PlayerController::SetHealth(float NewHealth)
{
    PlayerHealth = NewHealth;
    if (HealthBarWidgets)
    {
        HealthBarWidgets->UpdateHealth(PlayerHealth / 100.0f);  // Assuming Health is out of 100
    }
    UE_LOG(LogTemp, Log, TEXT("Health set to %f"), PlayerHealth);
}

void AA1PlayerController::InitializeStats(float InitialHealth, float InitialMovementSpeed, float InitialAttackPower)
{
    PlayerHealth = InitialHealth;
    MovementSpeed = InitialMovementSpeed;
    AttackPower = InitialAttackPower;
    UE_LOG(LogTemp, Log, TEXT("Stats Initialized: Health = %f, Movement Speed = %f, Attack Power = %f"), PlayerHealth, MovementSpeed, AttackPower);

    // 동기화: 플레이어 캐릭터의 이동 속도를 업데이트
    if (APawn* ControlledPawn = GetPawn())
    {
        if (APlayerChar* Players = Cast<APlayerChar>(ControlledPawn))
        {
            Players->SetMovementSpeed(MovementSpeed);
        }
    }
}

void AA1PlayerController::UpdateStats(float NewHealth, float NewMovementSpeed, float NewAttackPower)
{
    PlayerHealth = NewHealth;
    MovementSpeed = NewMovementSpeed;
    AttackPower = NewAttackPower;
    UE_LOG(LogTemp, Log, TEXT("Stats Updated: Health = %f, Movement Speed = %f, Attack Power = %f"), PlayerHealth, MovementSpeed, AttackPower);

    // 동기화: 플레이어 캐릭터의 이동 속도를 업데이트
    if (APawn* ControlledPawn = GetPawn())
    {
        if (APlayerChar* Players = Cast<APlayerChar>(ControlledPawn))
        {
            Players->SetMovementSpeed(MovementSpeed);
        }
    }
}

void AA1PlayerController::ShowLevelUpUI()
{
    if (LevelUpWidgetInstance && !LevelUpWidgetInstance->IsInViewport())
    {
        LevelUpWidgetInstance->AddToViewport(1);
        bShowMouseCursor = true;
        SetInputMode(FInputModeGameAndUI());
        UE_LOG(LogTemp, Log, TEXT("Level Up UI shown"));
    }
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
    CurrentAmmo = MaxAmmo;
    if (AmmoWidget)
    {
        AmmoWidget->UpdateAmmoCount(CurrentAmmo, MaxAmmo);
    }

    /*if (ReloadSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, ReloadSound, GetPawn()->GetActorLocation());
    }*/
}

void AA1PlayerController::HandleLevelUpOption(int OptionIndex)
{
    UpdateStatsBasedOnOption(OptionIndex);
    if (LevelUpWidgetInstance && LevelUpWidgetInstance->IsInViewport())
    {
        LevelUpWidgetInstance->RemoveFromParent();
        bShowMouseCursor = false;
        SetInputMode(FInputModeGameOnly());
        UE_LOG(LogTemp, Log, TEXT("Level Up UI hidden"));
    }
}

void AA1PlayerController::ToggleIPAddressWidget()
{
    UE_LOG(LogTemp, Warning, TEXT("ToggleIPAddressWidget called"));

    if (!IPAddressWidgetClass)
    {
        UE_LOG(LogTemp, Error, TEXT("IPAddressWidgetClass is null in ToggleIPAddressWidget"));
        return;
    }

    if (!IPAddressWidget)
    {
        IPAddressWidget = CreateWidget<UIPAddressWidget>(this, IPAddressWidgetClass);
        if (IPAddressWidget)
        {
            UE_LOG(LogTemp, Warning, TEXT("IPAddressWidget created successfully"));
            IPAddressWidget->AddToViewport(100);
            IPAddressWidget->SetVisibility(ESlateVisibility::Collapsed);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("IPAddressWidget creation failed"));
        }
    }

    if (bIsIPWidgetVisible)
    {
        IPAddressWidget->SetVisibility(ESlateVisibility::Collapsed);
        bShowMouseCursor = false;
        SetInputMode(FInputModeGameOnly());
        UE_LOG(LogTemp, Warning, TEXT("UI hidden, switched to game mode"));
    }
    else
    {
        IPAddressWidget->SetVisibility(ESlateVisibility::Visible);
        bShowMouseCursor = true;
        SetInputMode(FInputModeUIOnly());
        UE_LOG(LogTemp, Warning, TEXT("UI visible, switched to UI mode"));
    }

    bIsIPWidgetVisible = !bIsIPWidgetVisible;
}

void AA1PlayerController::ShowEnemyInfo_Internal(FString EnemyName, float Health)
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
                CurrentEnemyInfoWidget->AddToViewport(1);
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

void AA1PlayerController::UpdateStatsBasedOnOption(int OptionIndex)
{
    float NewHealth = PlayerHealth;
    float NewMovementSpeed = MovementSpeed;
    float NewAttackPower = AttackPower;

    switch (OptionIndex)
    {
    case 1:
        NewAttackPower *= 1.20f; // Increase attack power by 20%
        break;
    case 2:
        NewHealth *= 1.15f; // Increase health by 15%
        break;
    case 3:
        NewHealth = 100.0f; // Restore health to 100%
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
            NewHealth *= 1.25f; // Increase health by 25%
        else
            NewMovementSpeed *= 1.25f; // Increase movement speed by 25%
        break;
    }
    default:
        break;
    }

    // Update all stats and sync with character
    UpdateStats(NewHealth, NewMovementSpeed, NewAttackPower);

    UE_LOG(LogTemp, Log, TEXT("Stats updated: Health = %f, Movement Speed = %f, Attack Power = %f"), NewHealth, NewMovementSpeed, NewAttackPower);
}

void AA1PlayerController::Interact()
{
    /*if (NearbyStatue)
    {
        NearbyStatue->OnInteract(this);
    }*/
}
