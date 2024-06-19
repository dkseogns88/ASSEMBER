
#include "PlayerChar.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "../Objects/AnimInstanceCustom.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Controller.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnhancedInputComponent.h"
#include "Network/A1NetworkManager.h"


UA1NetworkManager* APlayerChar::GetNetworkManager() const
{
    return GetGameInstance()->GetSubsystem<UA1NetworkManager>();

}

APlayerChar::APlayerChar()
{
    PrimaryActorTick.bCanEverTick = true;
    
    bIsDamaged = false;
    bIsUsingSkill = false;
    bIsMoving = false;
    bIsAiming = false;
    bIsMovingBackward = false;
    bIsJumping = false;
   
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 0.0f; 
    CameraBoom->bUsePawnControlRotation = true; 

    
    FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
    FirstPersonCamera->SetupAttachment(CameraBoom);
    FirstPersonCamera->bUsePawnControlRotation = false;
    
   
    GetCharacterMovement()->MaxWalkSpeed = 600.0f;
    GetCharacterMovement()->bOrientRotationToMovement = true; 
    GetCharacterMovement()->bUseControllerDesiredRotation = true; 
    GetCharacterMovement()->JumpZVelocity = 300.f;
    GetCharacterMovement()->bAllowPhysicsRotationDuringAnimRootMotion = false; // Prevents rotation while root motion is active
   
    
    MovementInput = FVector2D(0.0f, 0.0f);
}

void APlayerChar::BeginPlay()
{
    Super::BeginPlay();
}


void APlayerChar::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    bIsMoving = MovementInput.SizeSquared() > 0.0f;
    bIsMovingBackward = MovementInput.X < 0.0f;

    if (UAnimInstanceCustom* AnimInstance = Cast<UAnimInstanceCustom>(GetMesh()->GetAnimInstance()))
    {
        //AnimInstance->SetMovementInput(MovementInput);
        //AnimInstance->SetIsMovingBackward(bIsMovingBackward);
    }
     
    StateTick();
    SendTick(DeltaTime);
    
}

void APlayerChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    
    PlayerInputComponent->BindAxis("MoveForward", this, &APlayerChar::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &APlayerChar::MoveRight);
    PlayerInputComponent->BindAxis("Turn", this, &APlayerChar::TurnRight);
    PlayerInputComponent->BindAxis("LookUp", this, &APlayerChar::AddControllerPitchInput);
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerChar::Jump);
    
}

void APlayerChar::SetMovementSpeed(float NewSpeed)
{
    Speed = NewSpeed;
    GetCharacterMovement()->MaxWalkSpeed = Speed;
}

void APlayerChar::StartRoll()
{
    
}

void APlayerChar::EndRoll()
{
    
}

void APlayerChar::UseSkillAnim(bool UsingSkill)
{
    bIsUsingSkill = UsingSkill;
    UAnimInstanceCustom* AnimInstance = Cast<UAnimInstanceCustom>(GetMesh()->GetAnimInstance());
    if (AnimInstance)
    {
        //AnimInstance->bIsUsingSkill = bIsUsingSkill;
    }
}



void APlayerChar::MoveForward(float Value)
{
    MovementInput.X = Value;
    MoveCache();
    if (Value != 0.0f)
    {
        AddMovementInput(GetActorForwardVector(), Value);
    }

}

void APlayerChar::MoveRight(float Value)
{
   
    MovementInput.Y = Value;
    MoveCache();
    if (Value != 0.0f)
    {
        AddMovementInput(GetActorRightVector(), Value);
    }
   
}

void APlayerChar::TurnLeft(float Value)
{
    AddControllerYawInput(-Value);
}

void APlayerChar::TurnRight(float Value)
{
    AddControllerYawInput(Value);
}

void APlayerChar::MoveCache()
{
    FVector ForwardDirection;
    FVector RightDirection;

    if (MovementInput.X != 0)
    {
        FRotator Rotator = GetControlRotation();
        ForwardDirection = UKismetMathLibrary::GetForwardVector(FRotator(0, Rotator.Yaw, 0));
    }

    if (MovementInput.Y != 0)
    {
        FRotator Rotator = GetControlRotation();
        RightDirection = UKismetMathLibrary::GetRightVector(FRotator(0, Rotator.Yaw, 0));
    }

    DesiredInput = MovementInput;

    DesiredMoveDirection = FVector::ZeroVector;
    DesiredMoveDirection += ForwardDirection * MovementInput.X;
    DesiredMoveDirection += RightDirection * MovementInput.Y;
    DesiredMoveDirection.Normalize();

    const FVector Location = GetActorLocation();
    FRotator Rotator = UKismetMathLibrary::FindLookAtRotation(Location, Location + DesiredMoveDirection);
    DesiredYaw = GetControlRotation().Yaw;

    //FRotator MyRotator = GetControlRotation();
    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(
    //    TEXT("Actor YAW: %.f"),
    //    MyRotator.Yaw
    //    )
    //);
}

void APlayerChar::Jump()
{
    Super::Jump();
    bIsJumping = true;
    UAnimInstanceCustom* AnimInstance = Cast<UAnimInstanceCustom>(GetMesh()->GetAnimInstance());
    if (AnimInstance)
    {

       // AnimInstance->bIsJumping = bIsJumping;

    }
}

void APlayerChar::Landed(const FHitResult& Hit)
{
    Super::Landed(Hit);
    bIsJumping = false;
    UAnimInstanceCustom* AnimInstance = Cast<UAnimInstanceCustom>(GetMesh()->GetAnimInstance());
    if (AnimInstance)
    {
        //AnimInstance->bIsJumping = bIsJumping;
    }
}

void APlayerChar::SetAiming(bool bNewAiming)
{
    bIsAiming = bNewAiming;
    UE_LOG(LogTemp, Log, TEXT("Aiming state set to: %s"), bIsAiming ? TEXT("True") : TEXT("False"));



    UAnimInstanceCustom* AnimInstance = Cast<UAnimInstanceCustom>(GetMesh()->GetAnimInstance());
    if (AnimInstance)
    {

       // AnimInstance->bIsAiming = bIsAiming;
        UE_LOG(LogTemp, Log, TEXT("Aiming state updated in animation blueprint to: %s"), bIsAiming ? TEXT("True") : TEXT("False"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to cast to UAnimInstanceCustom"));
    }
}

void APlayerChar::Aimingchanged()
{
    SetAiming(bIsAiming);
}

void APlayerChar::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(APlayerChar, bIsAiming);


}

void APlayerChar::IsDamaged(bool Damaged)
{
    if (!bIsDamaged)
    {
        bIsDamaged = Damaged;
        UAnimInstanceCustom* AnimInstance = Cast<UAnimInstanceCustom>(GetMesh()->GetAnimInstance());
        if (AnimInstance)
        {
           // AnimInstance->bIsDamaged = bIsDamaged;
        }
        
        UE_LOG(LogTemp, Log, TEXT("%s is damaged"), *GetName());

        // 일정 시간 후 피격 상태 초기화 
        GetWorld()->GetTimerManager().SetTimer(
            DamageResetTimerHandle,
            [this]()
            {
                bIsDamaged = false;
                UAnimInstanceCustom* AnimInstance = Cast<UAnimInstanceCustom>(GetMesh()->GetAnimInstance());
                if (AnimInstance)
                {
                    //AnimInstance->bIsDamaged = bIsDamaged;
                }
            },
            0.5f,
            false
        );

        // 게임 내에서 로그 출력
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%s is damaged 999"), *GetName()));
        }
    }
}

FVector2D APlayerChar::GetMovementInput() const
{
    const FVector InputVector = GetVelocity();
    return FVector2D(InputVector.X, InputVector.Y);
}

void APlayerChar::StateTick()
{
    if (DesiredInput == FVector2D::Zero())
    {
        SetMoveState(Protocol::MOVE_STATE_IDLE);
    }
    else if ((DesiredInput != FVector2D::Zero()))
    {
        SetMoveState(Protocol::MOVE_STATE_RUN);
    }
}

void APlayerChar::SendTick(float DeltaTime)
{
    // Send 판정
    bool ForceSendPacket = false;

    if ((LastDesiredInput != DesiredInput))
    {
        ForceSendPacket = true;
        LastDesiredInput = DesiredInput;
    }

    MovePacketSendTimer -= DeltaTime;

    if (MovePacketSendTimer <= 0 || ForceSendPacket)
    {
        MovePacketSendTimer = MOVE_PACKET_SEND_DELAY;

        if (GetMoveState() == Protocol::MOVE_STATE_RUN
            || GetMoveState() == Protocol::MOVE_STATE_IDLE)
        {
            Send_Idle_Move();
        }
    }
}

void APlayerChar::Send_Idle_Move()
{
    Protocol::C_MOVE MovePkt;

    Protocol::PosInfo* Info = MovePkt.mutable_info();
    Info->CopyFrom(*PlayerInfo);
    Info->set_yaw(DesiredYaw);
    Info->set_state(GetMoveState());
    Info->set_d_x(DesiredMoveDirection.X);
    Info->set_d_y(DesiredMoveDirection.Y);
    Info->set_d_z(DesiredMoveDirection.Z);

    GetNetworkManager()->SendPacket(MovePkt);
}

