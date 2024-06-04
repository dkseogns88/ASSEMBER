
#include "PlayerChar.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
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
    GetCharacterMovement()->bUseControllerDesiredRotation = false; 
    GetCharacterMovement()->JumpZVelocity = 300.f;
    IsMoving = false;
}

void APlayerChar::BeginPlay()
{
    Super::BeginPlay();
}

void APlayerChar::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

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
    if (Controller != nullptr)
    {
        if (Value != 0.0f)
        {
            const FRotator Rotation = Controller->GetControlRotation();
            const FRotator YawRotation(0, Rotation.Yaw, 0);
            const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
            AddMovementInput(Direction, Value);
            IsMoving = true;
        }
        else
        {
            IsMoving = false;
        }
    }
}

void APlayerChar::MoveRight(float Value)
{
    if (Controller != nullptr)
    {
        if (Value != 0.0f)
        {
            const FRotator Rotation = Controller->GetControlRotation();
            const FRotator YawRotation(0, Rotation.Yaw, 0);
            const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
            AddMovementInput(Direction, Value);
            IsMoving = true;
        }
        else
        {
            IsMoving = false;
        }
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