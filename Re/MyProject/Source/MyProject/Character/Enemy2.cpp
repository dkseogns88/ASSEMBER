// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy2.h"
#include "Components/SkeletalMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
AEnemy2::AEnemy2()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;


    // Create and initialize the skeletal mesh component
    USkeletalMeshComponent* SkeletalMesh = GetMesh();
    if (!SkeletalMesh)
    {
        SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
        SetRootComponent(SkeletalMesh);
    }
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("/Game/Asset/Enemy02/fanatic_animation.fanatic_animation"));


    if (MeshAsset.Succeeded())
    {
        SkeletalMesh->SetSkeletalMesh(MeshAsset.Object);

    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load skeletal mesh."));
    }

    // Setup collision and visibility
    SkeletalMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -160.0f));
    SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    SkeletalMesh->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
    SkeletalMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
    SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
    SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
    SkeletalMesh->SetVisibility(true, true);

    // Setup CapsuleComponent for collision
    UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
    CapsuleComp->InitCapsuleSize(200.0f, 100.0f);
    CapsuleComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    CapsuleComp->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
    CapsuleComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
    CapsuleComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
    CapsuleComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
    // Constructor에서 애니메이션 로드

    static ConstructorHelpers::FObjectFinder<UAnimBlueprintGeneratedClass> AnimBP(TEXT("AnimBlueprintGeneratedClass'/Game/AnimationBlueprint/Enemy2.Enemy2_C'"));
    if (AnimBP.Succeeded())
    {
        SkeletalMesh->SetAnimInstanceClass(AnimBP.Object);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load animation blueprint class."));
    }


}

void AEnemy2::CheckAndTeleport()
{
    FVector CurrentLocation = GetActorLocation();
    if (CurrentLocation.Z < -300.0f)
    {
        FVector NewLocation(0.0f, 0.0f, 300.0f);
        SetActorLocation(NewLocation);
    }
}
// Called when the game starts or when spawned
void AEnemy2::BeginPlay()
{
    Super::BeginPlay();
    // 강제로 메쉬의 가시성을 활성화
    GetMesh()->SetVisibility(true);

    // 물리 및 충돌 설정 확인

    GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

    CheckMeshSetup();
}

// Called every frame
void AEnemy2::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    CheckAndTeleport();
    //UpdateAnimation();
}




void AEnemy2::CheckMeshSetup()
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

void AEnemy2::UpdateAnimation()
{



}