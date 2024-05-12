// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy1.h"
#include "Components/SkeletalMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
AEnemy1::AEnemy1()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    EnemyName = TEXT("Example Enemy");
    Health = 100.0f;
    // Create and initialize the skeletal mesh component
    USkeletalMeshComponent* SkeletalMesh = GetMesh();
    if (!SkeletalMesh)
    {
        SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent> ("SkeletalMeshComponent");
        SetRootComponent(SkeletalMesh);
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

    // Setup collision and visibility
    SkeletalMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -160.0f));
    SkeletalMesh->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
    SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    SkeletalMesh->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
    SkeletalMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
    SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
    SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
    SkeletalMesh->SetVisibility(true, true);
    
   
    // Create and initialize the box component for collision
    BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
    BoxComponent->SetupAttachment(RootComponent);
    BoxComponent->SetBoxExtent(FVector(25.0f, 25.0f, 100.0f));  // Adjust the size as necessary
    BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    BoxComponent->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
    BoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
    BoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
    BoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);

    // Set the relative location of the box component if necessary
    BoxComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -50.0f));
	// Constructor에서 애니메이션 로드
 
    static ConstructorHelpers::FObjectFinder<UAnimBlueprintGeneratedClass> AnimBP(TEXT("AnimBlueprintGeneratedClass'/Game/AnimationBlueprint/Enemy1.Enemy1_C'"));
    if (AnimBP.Succeeded())
    {
        SkeletalMesh->SetAnimInstanceClass(AnimBP.Object);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load animation blueprint class."));
    }
   
    
}

void AEnemy1::CheckAndTeleport()
{
    FVector CurrentLocation = GetActorLocation();
    if (CurrentLocation.Z < -300.0f)
    {
        FVector NewLocation(0.0f, 0.0f, 300.0f);
        SetActorLocation(NewLocation);
    }
}
// Called when the game starts or when spawned
void AEnemy1::BeginPlay()
{
	Super::BeginPlay();
    // 강제로 메쉬의 가시성을 활성화
    GetMesh()->SetVisibility(true);

    // 물리 및 충돌 설정 확인
   
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
   
    
    CheckMeshSetup();
}

// Called every frame
void AEnemy1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    TArray<FVector> CornerPoints = GetBoxCornerPoints();
    FVector Pivot = BoxComponent->GetComponentLocation(); // 중심 피봇 좌표
    /*
    UE_LOG(LogTemp, Log, TEXT("Enemy 1 Pivot Point: %s"), *Pivot.ToString()); // 피봇 좌표 출력

    for (const FVector& Point : CornerPoints)
    {
        DrawDebugSphere(GetWorld(), Point, 5.0f, 12, FColor::Green, false, -1.0f);
        UE_LOG(LogTemp, Log, TEXT("Enemy 1 Corner Point: %s"), *Point.ToString());

        // 꼭지점에서 피봇으로 나눈 상대 좌표
        FVector RelativePoint = Point - Pivot;
        UE_LOG(LogTemp, Log, TEXT(" Enemy 1 Relative Corner Point: %s"), *RelativePoint.ToString());
    }
    */
    CheckAndTeleport();
    //UpdateAnimation();
}

TArray<FVector> AEnemy1::GetBoxCornerPoints() const
{
    TArray<FVector> Points;
    FVector Extent = BoxComponent->GetScaledBoxExtent();
    FVector Origin = BoxComponent->GetComponentLocation();

    // Calculate the corner points
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
