
#include "PlayerChar.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "../Objects/AnimInstanceCustom.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Controller.h"


APlayerChar::APlayerChar()
{
    PrimaryActorTick.bCanEverTick = true;

   
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
        AnimInstance->SetMovementInput(MovementInput);
        AnimInstance->SetIsMovingBackward(bIsMovingBackward);
    }
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

void APlayerChar::UseSkill(int32 SkillIndex)
{
    
}

void APlayerChar::MoveForward(float Value)
{
    MovementInput.X = Value;
    if (Value != 0.0f)
    {
        AddMovementInput(GetActorForwardVector(), Value);
    }
     
}

void APlayerChar::MoveRight(float Value)
{
   
    MovementInput.Y = Value;
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

void APlayerChar::Jump()
{
    Super::Jump();
}

void APlayerChar::SetAiming(bool bNewAiming)
{
    bIsAiming = bNewAiming;
    UE_LOG(LogTemp, Log, TEXT("Aiming state set to: %s"), bIsAiming ? TEXT("True") : TEXT("False"));



    UAnimInstanceCustom* AnimInstance = Cast<UAnimInstanceCustom>(GetMesh()->GetAnimInstance());
    if (AnimInstance)
    {

        AnimInstance->bIsAiming = bIsAiming;
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