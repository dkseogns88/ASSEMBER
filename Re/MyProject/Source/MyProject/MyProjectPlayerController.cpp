// Copyright Epic Games, Inc. All Rights Reserved.


#include "MyProjectPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "EnhancedInputSubsystems.h"
#include "Enemy1.h"
#include "Skill.h"
#include "Kismet/GameplayStatics.h"
#include "MyProjectMyPlayer.h"
#include "MyProjectMyPlayerSida.h"
#include "DrawDebugHelpers.h"
#include "EnemyInfoWidget.h"
#include "IPAddressWidget.h"
#include "Components/InputComponent.h"
#include "MyProject.h"
#include "Protocol.pb.h"
#include "GameFramework/PlayerState.h"



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

    static ConstructorHelpers::FClassFinder<UUserWidget> LevelUpWidgetBPClass(TEXT("/Game/MyBP/UI/LevelUpWidget.LevelUpWidget_C"));
    if (LevelUpWidgetBPClass.Succeeded())
    {
        LevelUpWidgetClass = LevelUpWidgetBPClass.Class;
    }

    SkillManager = CreateDefaultSubobject<USkillManager>(TEXT("SkillManager"));

    //스킬 추가
    USkill* Hellgun = NewObject<USkill>();
    Hellgun->Initialize("Hellgun", 300.0f, 50.0f);
    SkillManager->AddSkill(Hellgun);
    
    static ConstructorHelpers::FClassFinder<UIPAddressWidget> IPAddressWidgetBPClass(TEXT("/Game/MyBP/UI/IPAddressWidget.IPAddressWidget_C"));
    if (IPAddressWidgetBPClass.Succeeded())
    {
        IPAddressWidgetClass = IPAddressWidgetBPClass.Class;
        UE_LOG(LogTemp, Warning, TEXT("Successfully found BP_IPAddressWidget"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to find BP_IPAddressWidget"));
    }

    NearbyStatue = nullptr;
}



void AMyProjectPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (IPAddressWidgetClass)
    {
        IPAddressWidget = CreateWidget<UIPAddressWidget>(this, IPAddressWidgetClass);
        if (IPAddressWidget)
        {
            IPAddressWidget->AddToViewport();
            bShowMouseCursor = true;
            SetInputMode(FInputModeUIOnly());
        }
    }

    InitializeStats(100.0f, 500.0f, 20.0f);
    
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
        
        //Create LevelUpWidget
         LevelUpWidgetInstance = CreateWidget<ULevelUpWidget>(this, LevelUpWidgetClass);
        
    }

    SetHealth(PlayerHealth);
    
}

// Implement InitializeStats
void AMyProjectPlayerController::InitializeStats(float InitialHealth, float InitialMovementSpeed, float InitialAttackPower)
{
    PlayerHealth = InitialHealth;
    MovementSpeed = InitialMovementSpeed;
    AttackPower = InitialAttackPower;
    UE_LOG(LogTemp, Log, TEXT("Stats Initialized: Health = %f, Movement Speed = %f, Attack Power = %f"), PlayerHealth, MovementSpeed, AttackPower);

    // 동기화: 플레이어 캐릭터의 이동 속도를 업데이트
    if (APawn* ControlledPawn = GetPawn())
    {
        if (AMyProjectPlayer* Players = Cast<AMyProjectPlayer>(ControlledPawn))
        {
            Players->SetMovementSpeed(MovementSpeed);
        }
    }
}

// Implement UpdateStats
void AMyProjectPlayerController::UpdateStats(float NewHealth, float NewMovementSpeed, float NewAttackPower)
{
    PlayerHealth = NewHealth;
    MovementSpeed = NewMovementSpeed;
    AttackPower = NewAttackPower;
    UE_LOG(LogTemp, Log, TEXT("Stats Updated: Health = %f, Movement Speed = %f, Attack Power = %f"), PlayerHealth, MovementSpeed, AttackPower);

    // 동기화: 플레이어 캐릭터의 이동 속도를 업데이트
    if (APawn* ControlledPawn = GetPawn())
    {
        if (AMyProjectPlayer* Players = Cast<AMyProjectPlayer>(ControlledPawn))
        {
            Players->SetMovementSpeed(MovementSpeed);
        }
    }
}


void AMyProjectPlayerController::SetHealth(float NewHealth)
{
    PlayerHealth = NewHealth;
    if (HealthBarWidgets)
    {
        HealthBarWidgets->UpdateHealth(PlayerHealth / 100.0f);  // Assuming Health is out of 100
    }
    UE_LOG(LogTemp, Log, TEXT("Health set to %f"), PlayerHealth);
}

// Show the Level Up UI
void AMyProjectPlayerController::ShowLevelUpUI()
{
    if (LevelUpWidgetInstance && !LevelUpWidgetInstance->IsInViewport())
    {
        LevelUpWidgetInstance->AddToViewport();
        bShowMouseCursor = true;
        SetInputMode(FInputModeGameAndUI());
        UE_LOG(LogTemp, Log, TEXT("Level Up UI shown"));
    }
}

// Handle the Level Up option selected by the player
void AMyProjectPlayerController::HandleLevelUpOption(int OptionIndex)
{
    UpdateStatsBasedOnOption(OptionIndex);
    if (LevelUpWidgetInstance && LevelUpWidgetInstance->IsInViewport())
    {
        LevelUpWidgetInstance->RemoveFromViewport();
        bShowMouseCursor = false;
        SetInputMode(FInputModeGameOnly());
        UE_LOG(LogTemp, Log, TEXT("Level Up UI hidden"));
    }
}

//Toggle IPAddress
void AMyProjectPlayerController::ToggleIPAddressWidget()
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
            IPAddressWidget->AddToViewport();
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

// Update stats based on the selected option
void AMyProjectPlayerController::UpdateStatsBasedOnOption(int OptionIndex)
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

   
    InputComponent->BindAction("Aim", IE_Pressed, this, &AMyProjectPlayerController::OnAimPressed);
    InputComponent->BindAction("Aim", IE_Released, this, &AMyProjectPlayerController::OnAimReleased);

    InputComponent->BindAction("Fire", IE_Pressed, this, &AMyProjectPlayerController::AttemptToFireWeapon);

    InputComponent->BindAction("Reload", IE_Pressed, this, &AMyProjectPlayerController::ReloadWeapon);

    InputComponent->BindAction("UseSkill", IE_Pressed, this, &AMyProjectPlayerController::UseSkill);

    InputComponent->BindAction("LevelUpUI", IE_Pressed, this, &AMyProjectPlayerController::ShowLevelUpUI);

    InputComponent->BindAction("ToggleIPAddressWidget", IE_Pressed, this, &AMyProjectPlayerController::ToggleIPAddressWidget);

    InputComponent->BindAction("InteractStatue", IE_Pressed, this, &AMyProjectPlayerController::Interact);
    
}

void AMyProjectPlayerController::PerformSkill(FName SkillName)
{
    if (SkillManager)
    {
        UE_LOG(LogTemp, Log, TEXT("SkillManager is valid"));


        USkill* SkillToUse = SkillManager->GetSkillByName(SkillName);
        if (SkillToUse)
        {
            APawn* MyPawn = GetPawn();
            if (MyPawn)
            {
                SkillToUse->ExecuteSkill(MyPawn);
                UE_LOG(LogTemp, Log, TEXT("Success to use skill"));
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Fail to call Using skill"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Fail to call skill Manager"));
    }
}
void AMyProjectPlayerController::LogSkillUsage(FName SkillName)
{
    int32& UsageCount = SkillUsageCount.FindOrAdd(SkillName);
    UsageCount++;

    APlayerState* MyPlayerState = GetPlayerState<APlayerState>();
    if (MyPlayerState)
    {
        int32 PlayerId = MyPlayerState->GetPlayerId();
        UE_LOG(LogTemp, Log, TEXT("Player %d used skill %s, total usage: %d"), PlayerId, *SkillName.ToString(), UsageCount);
    }
}

void AMyProjectPlayerController::UseSkill()
{
    UE_LOG(LogTemp, Log, TEXT("Try to use skill"));
    LogSkillUsage("Hellgun");
    PerformSkill("Hellgun");
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
                if (GameInstance->Monsters.Find(Enemy->MonsterInfo->object_id()))
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

void AMyProjectPlayerController::Interact()
{
    if (NearbyStatue)
    {
        NearbyStatue->OnInteract(this);
    }
}

void AMyProjectPlayerController::ChangeCharacter(const Protocol::PlayerType PlayerType)
{



    return;

    UMyProjectGameInstance* GameInstance = GetGameInstance<UMyProjectGameInstance>();
    if (GameInstance)
    {
        
        //UE_LOG(LogTemp, Log, TEXT("Successfully load ChangeCharacter , GameInstance"));
        //UClass* NewCharacterClass = GameInstance->GetCharacterClass(CharacterName);
        //if (NewCharacterClass)
        //{
        //    APawn* CurrentPawn = GetPawn();
        //    //기존 캐릭터의 위치 저장
        //    FVector Location = CurrentPawn->GetActorLocation();
        //    FRotator Rotation = CurrentPawn->GetActorRotation();

        //    //새캐릭터를 월드의 이전플레이어 위치에 스폰
        //    
        //    AMyProjectPlayer* NewCharacter = GetWorld()->SpawnActor<AMyProjectPlayer>(NewCharacterClass, Location, Rotation);
        //    if (NewCharacter)
        //    {
        //        Possess(NewCharacter); //소유권 이전
        //        UE_LOG(LogTemp, Log, TEXT("Successfully Change and Possess NewCharacter"));
        //        CurrentPawn->Destroy();

        //        // 플레이어 변경 로그 기록
        //        APlayerState* CurrentPlayerState = GetPlayerState<APlayerState>();
        //        int32 PlayerIndex = CurrentPlayerState ? CurrentPlayerState->GetPlayerId() : -1; //플레이어정보
        //        GameInstance->LogCharacterChange(PlayerIndex, CharacterName); //플레이어,캐릭터정보를 로그로출력하는 함수
        //    }
        //    else
        //    {
        //        UE_LOG(LogTemp, Error, TEXT("Failed to spawn new character of class: %s"), *NewCharacterClass->GetName());
        //    }
        //}
        //else
        //{
        //    UE_LOG(LogTemp, Error, TEXT("Failed to get new character class for: %s"), *CharacterName);
        //}

    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load game instance"));
    }
}

void AMyProjectPlayerController::SetNearbyStatue(ACharacterStatue* Statue)
{
    NearbyStatue = Statue;
}