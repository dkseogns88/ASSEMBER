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
#include "PlayerChar.h"
#include "../Objects/A1PlayerController.h"
#include "Sound/SoundCue.h"
#include "Components/SphereComponent.h" 


AMonster::AMonster()
{
    PrimaryActorTick.bCanEverTick = true;
    bIsAttacking = false;
    bIsDamaged = false;
    bIsDead = false;
    TimeSinceLastAttack = 0.0f;
    AttackDuration = 1.0f;
    SwordDMG = 50.0f;
    
    BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
    BoxComponent->SetupAttachment(RootComponent);

    FireSound = nullptr;
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

    if (bIsAttacking && !bIsDealPlayer)
    {
        CheckSwordHit();
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

        if (MonsterType == "Gunner")
        {
            FireProjectile();
            if (FireSound)
            {
                UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
            }
            }
        }
        else if (MonsterType == "Warrior")
        {
            SwordAttack();
        }

        GetWorld()->GetTimerManager().SetTimer(AttackResetTimerHandle, this, &AMonster::ResetAttack, AttackDuration, false);
    
}


   


void AMonster::TakeDMG(float DMG)
{
    if (!bIsDamaged)
    {
        bIsDamaged = true;
        UE_LOG(LogTemp, Log, TEXT("%s Damage taken"), *MonName);
        Health = Health - DMG;
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
            }, 0.3f, false);
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

    // 일정 시간 후 canattack을 true로 설정하여 다시 공격할 수 있도록 함
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
        FVector MuzzleLocation = GetActorLocation() + FVector(0, 0, 50);
        FRotator MuzzleRotation = GetActorRotation();

        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;  
        SpawnParams.Instigator = GetInstigator();

        AMonsterProjectile* Projectile = GetWorld()->SpawnActor<AMonsterProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
        if (Projectile)
        {
            FVector LaunchDirection = MuzzleRotation.Vector();
            Projectile->ProjectileMovementComponent->Velocity = LaunchDirection * Projectile->ProjectileMovementComponent->InitialSpeed;

            
            Projectile->CollisionComponent->IgnoreActorWhenMoving(this, true);

           
        }
    }
    
}

void AMonster::SwordAttack()
{
    
    UE_LOG(LogTemp, Log, TEXT("%s performs a sword attack"), *GetName());

    // 공격 시작
    bIsAttacking = true;
    bIsDealPlayer = false;

    // 0.7초 후에 공격 종료
    GetWorld()->GetTimerManager().SetTimer(AttackEndHandle, this, &AMonster::EndSwordAttack, 0.7f, false);
    
}

void AMonster::EndSwordAttack()
{
    bIsAttacking = false;
    UE_LOG(LogTemp, Log, TEXT("%s ends the sword attack"), *GetName());
}

void AMonster::CheckSwordHit()
{
    FVector Start = GetActorLocation();
    FVector End = Start + GetActorForwardVector() * 100.0f; 

    TArray<FHitResult> HitResults;
    FCollisionShape CollisionShape = FCollisionShape::MakeSphere(50.0f); 

    bool bHit = GetWorld()->SweepMultiByChannel(HitResults, Start, End, FQuat::Identity, ECC_Pawn, CollisionShape);

    if (bHit)
    {
        for (FHitResult HitResult : HitResults)
        {
            AActor* HitActor = HitResult.GetActor();
            if (HitActor && HitActor->IsA(APlayerChar::StaticClass()))
            {
                APlayerChar* HitCharacter = Cast<APlayerChar>(HitActor);
                if (HitCharacter)
                {
                    AA1PlayerController* PlayerController = Cast<AA1PlayerController>(HitCharacter->GetController());
                    if (PlayerController)
                    {
                        PlayerController->ApplyDamage(SwordDMG);
                        //데미지로그
                        if (GEngine)
                        {
                            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Fanatic Dealing %f damage to %s"), SwordDMG, *HitCharacter->GetName()));
                        }
                        bIsDealPlayer = true;
                        break;
                    }
                    HitCharacter->IsDamaged(true);
                }
            }
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
