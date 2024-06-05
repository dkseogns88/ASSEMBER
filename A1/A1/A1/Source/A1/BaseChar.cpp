
#include "BaseChar.h"


ABaseChar::ABaseChar()
{
 	
	PrimaryActorTick.bCanEverTick = true;
    IsMoving = false;
}

void ABaseChar::BeginPlay()
{
	Super::BeginPlay();
	
}


void ABaseChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ABaseChar::MoveForward(float Value)
{
    if (Value != 0.0f)
    {
        AddMovementInput(GetActorForwardVector(), Value);
        IsMoving = true;
    }
    else
    {
        IsMoving = false;
    }
}

void ABaseChar::MoveRight(float Value)
{
    if (Value != 0.0f)
    {
        AddMovementInput(GetActorRightVector(), Value);
        IsMoving = true;
    }
    else
    {
        IsMoving = false;
    }
}

void ABaseChar::Jump()
{
    Super::Jump();
}

void ABaseChar::ReadyAttack(bool canattack)
{
    canattack = true;
    
}

void ABaseChar::Attack(bool canattack)
{
    canattack = false;
}

void ABaseChar::TakeDMG(float Value)
{
    
}

void ABaseChar::Die()
{
  
}

void ABaseChar::TurnLeft(float Value)
{
    AddActorLocalRotation(FRotator(0.0f, -Value, 0.0f));
}

void ABaseChar::TurnRight(float Value)
{
    AddActorLocalRotation(FRotator(0.0f, Value, 0.0f));
}