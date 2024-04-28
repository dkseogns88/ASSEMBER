// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy1.h"
#include "Components/SkeletalMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
AEnemy1::AEnemy1()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    
    // Create and initialize the skeletal mesh component
    USkeletalMeshComponent* SkeletalMesh = GetMesh();
    if (!SkeletalMesh)
    {
        SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent> ("SkeletalMeshComponent");
    }
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("/Game/Asset/Enemy01/ENEMY01.ENEMY01"));

    if (MeshAsset.Succeeded())
    {
        SkeletalMesh->SetSkeletalMesh(MeshAsset.Object);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load skeletal mesh."));
    }
    
	// Constructor에서 애니메이션 로드
	static ConstructorHelpers::FObjectFinder<UAnimSequence> IdleAnimAsset(TEXT("/Game/Asset/Enemy01/enemy01walk_Anim.enemy01walk_Anim"));
	if (IdleAnimAsset.Succeeded())
	{
		IdleAnimation = IdleAnimAsset.Object;
		UE_LOG(LogTemp, Log, TEXT("Idle Animation loaded successfully: %s"), *IdleAnimAsset.Object->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load animation from path:/Game/Asset/Enemy01/enemy01walk_Anim.enemy01walk_Anim"));
	}

    
}

// Called when the game starts or when spawned
void AEnemy1::BeginPlay()
{
	Super::BeginPlay();

    if (!GetMesh() || !IdleAnimation)
    {
        UE_LOG(LogTemp, Error, TEXT("Mesh or animation not properly configured."));
        return;
    }
	PlayIdleAnimation();
    CheckMeshSetup();
}

// Called every frame
void AEnemy1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    //UpdateAnimation();
}

// Called to bind functionality to input
void AEnemy1::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy1::PlayIdleAnimation()
{
	if (IdleAnimation && GetMesh())
	{
		GetMesh()->PlayAnimation(IdleAnimation, true);
	}
}

void AEnemy1::CheckMeshSetup()
{
    USkeletalMeshComponent* SkeletalMeshComponent = GetMesh();
    if (!SkeletalMeshComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("SkeletalMeshComponent not found in %s"), *GetName());
        return;
    }

    // 스켈레탈 메시
    USkeletalMesh* SkeletalMesh = SkeletalMeshComponent->SkeletalMesh;
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

void AEnemy1::UpdateAnimation()
{
   
   
    
}
