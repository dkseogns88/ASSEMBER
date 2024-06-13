// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"
#include "Components/SkeletalMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Animation/AnimInstance.h"
#include "TimerManager.h"
#include "../Objects/MonsterProjectile.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "Components/SphereComponent.h" 


AMonster::AMonster()
{
    PrimaryActorTick.bCanEverTick = true;
    bIsAttacking = false;
    bIsDamaged = false;
    bIsDead = false;
    TimeSinceLastAttack = 0.0f;
    AttackDuration = 1.0f;
    
    BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
    BoxComponent->SetupAttachment(RootComponent);
}

AMonster::~AMonster()
{

}
void AMonster::BeginPlay()
{

    Super::BeginPlay();
}

void AMonster::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Attack interval handling
    TimeSinceLastAttack += DeltaTime;
    if (TimeSinceLastAttack >= AttackInterval && !bIsDamaged)
    {
        ReadyAttack(true);
        TimeSinceLastAttack = 0.0f;
    }

    if (Health <= 0)
    {
        Die();
    }
}

void AMonster::ReadyAttack(bool canattack)
{
    if (canattack && !bIsAttacking && !bIsDamaged)
    {
        bIsAttacking = true;
        UE_LOG(LogTemp, Log, TEXT("%s Attack triggered"), *GetName());
        Attack(canattack);
    }
}

void AMonster::Attack(bool canattack)
{
    if (canattack && !bIsDamaged)
    {
       
        UE_LOG(LogTemp, Log, TEXT("%s is attacking"), *GetName());
        FireProjectile();
        
        GetWorld()->GetTimerManager().SetTimer(AttackResetTimerHandle, this, &AMonster::ResetAttack, AttackDuration, false);
    }

   
}

void AMonster::TakeDMG(float Value)
{
    if (!bIsDamaged)
    {
        bIsDamaged = true;
        UE_LOG(LogTemp, Log, TEXT("%s Damage taken"), *MonName);
        Health = Health - Value;
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
    UE_LOG(LogTemp, Log, TEXT("%s has finished attacking"), *GetName());

    // ���� �ð� �� canattack�� true�� �����Ͽ� �ٽ� ������ �� �ֵ��� ��
    GetWorld()->GetTimerManager().SetTimer(AttackResetTimerHandle, this, &AMonster::EnableAttack, AttackInterval, false);
}

void AMonster::ResetDamage()
{
    bIsDamaged = false;
}

void AMonster::EnableAttack()
{
    ReadyAttack(true);
   
}

void AMonster::FireProjectile()
{

    if (ProjectileClass)
    {
        FVector MuzzleLocation = GetActorLocation() + FVector(0, 0, 100);
        FRotator MuzzleRotation = GetActorRotation();

        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;  // This ensures the projectile knows its owner
        SpawnParams.Instigator = GetInstigator();

        AMonsterProjectile* Projectile = GetWorld()->SpawnActor<AMonsterProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
        if (Projectile)
        {
            FVector LaunchDirection = MuzzleRotation.Vector();
            Projectile->ProjectileMovementComponent->Velocity = LaunchDirection * Projectile->ProjectileMovementComponent->InitialSpeed;

            // Ignore the owner (monster) that fired the projectile
            Projectile->CollisionComponent->IgnoreActorWhenMoving(this, true);

            if (GEngine)
            {
                GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Projectile fired by %s"), *GetName()));
            }
        }
        else
        {
            if (GEngine)
            {
                GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Failed to spawn projectile")));
            }
        }
    }
    else
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("ProjectileClass is not set")));
        }
    }
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

    // GetSkinnedAsset()�� ����Ͽ� SkeletalMesh ��������
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
