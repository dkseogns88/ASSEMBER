// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"
#include "Components/SkeletalMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Animation/AnimInstance.h"
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

void AMonster::BeginPlay()
{

    Super::BeginPlay();
}

void AMonster::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Attack interval handling
    TimeSinceLastAttack += DeltaTime;
    if (TimeSinceLastAttack >= AttackInterval)
    {
       
        ReadyAttack(bIsAttacking);
        TimeSinceLastAttack = 0.0f;
    }

    if (Health <= 0)
    {
        Die();
    }
}

void AMonster::ReadyAttack(bool canattack)
{
    if (!bIsAttacking)
    {
        canattack = true;
        bIsAttacking = true;
        UE_LOG(LogTemp, Log, TEXT("%s Attack triggered"), *MonName);
        // Reset attack after 1 second (duration of the attack animation)
        Attack(canattack);
    }
}

void AMonster::Attack(bool canattack)
{
    if (canattack)
    {
        // Execute attack logic
        UE_LOG(LogTemp, Log, TEXT("%s is attacking"), *MonName);

        // Reset attack state after attack duration
        GetWorld()->GetTimerManager().SetTimer(AttackResetTimerHandle, this, &AMonster::ResetAttack, 1.0f, false);
    }

   
}

void AMonster::TakeDMG(float Value)
{
    if (!bIsDamaged)
    {
        bIsDamaged = true;
        UE_LOG(LogTemp, Log, TEXT("%s Damage taken"), *MonName);
        Health = Health - Value;
        // Reset damage after 0.5 seconds (duration of the damage animation)
        GetWorld()->GetTimerManager().SetTimer(DamageResetTimerHandle, this, &AMonster::ResetDamage, 0.5f, false);
        
    }
}

void AMonster::Die()
{
    if (!bIsDead)
    {
        bIsDead = true;

        UE_LOG(LogTemp, Log, TEXT("%s died"), *MonName);

        
        GetWorld()->GetTimerManager().SetTimer(DeathHandle, [this]()
            {
                this->Destroy();
            }, 1.5f, false);
    }
}

void AMonster::TurnLeft(float Value)
{
    AddActorLocalRotation(FRotator(0.0f, -Value, 0.0f));
}

void AMonster::TurnRight(float Value)
{
    AddActorLocalRotation(FRotator(0.0f, Value, 0.0f));
}

void AMonster::ResetAttack()
{
    bIsAttacking = false;
}

void AMonster::ResetDamage()
{
    bIsDamaged = false;
}

AMonster::AMonster()
{
    PrimaryActorTick.bCanEverTick = true;
    bIsAttacking = false;
    bIsDamaged = false;
    bIsDead = false;
    TimeSinceLastAttack = 0.0f;
    

    BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
    BoxComponent->SetupAttachment(RootComponent);
}

AMonster::~AMonster()
{
   
}

TArray<FVector> AMonster::GetBoxCornerPoints() const
{
    TArray<FVector> Points;
    FVector Extent = BoxComponent->GetScaledBoxExtent();
    FVector Origin = BoxComponent->GetComponentLocation();

    FVector BoxPoints[] = {
        FVector(Extent.X, Extent.Y, Extent.Z),
        FVector(Extent.X, Extent.Y, -Extent.Z),
        FVector(Extent.X, -Extent.Y, Extent.Z),
        FVector(Extent.X, -Extent.Y, -Extent.Z),
        FVector(-Extent.X, Extent.Y, Extent.Z),
        FVector(-Extent.X, Extent.Y, -Extent.Z),
        FVector(-Extent.X, -Extent.Y, Extent.Z),
        FVector(-Extent.X, -Extent.Y, -Extent.Z)
    };

    for (const FVector& Point : BoxPoints)
    {
        Points.Add(Origin + Point);
    }

    return Points;
}

void AMonster::CheckMeshSetup()
{

    USkeletalMeshComponent* SkeletalMeshComponent = GetMesh();
    if (!SkeletalMeshComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("SkeletalMeshComponent not found in %s"), *GetName());
        return;
    }

    // GetSkinnedAsset()을 사용하여 SkeletalMesh 가져오기
    USkeletalMesh* SkeletalMesh = Cast<USkeletalMesh>(SkeletalMeshComponent->GetSkinnedAsset());
    if (!SkeletalMesh)
    {
        UE_LOG(LogTemp, Warning, TEXT("No skeletal mesh is set for %s"), *GetName());
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Skeletal Mesh for %s is %s"), *GetName(), *SkeletalMesh->GetName());
    }

    int32 MaterialCount = SkeletalMeshComponent->GetNumMaterials();
    UE_LOG(LogTemp, Log, TEXT("Number of materials on %s: %d"), *GetName(), MaterialCount);

    for (int32 i = 0; i < MaterialCount; ++i)
    {
        UMaterialInterface* Material = SkeletalMeshComponent->GetMaterial(i);
        if (!Material)
        {
            UE_LOG(LogTemp, Warning, TEXT("Material %d on %s is not set"), i, *GetName());
        }
        else
        {
            UE_LOG(LogTemp, Log, TEXT("Material %d on %s is %s"), i, *GetName(), *Material->GetName());
        }
    }
}

void AMonster::CheckAndTeleport()
{
    FVector CurrentLocation = GetActorLocation();
    if (CurrentLocation.Z < -300.0f)
    {
        FVector NewLocation(0.0f, 0.0f, 300.0f);
        SetActorLocation(NewLocation);
    }
}
