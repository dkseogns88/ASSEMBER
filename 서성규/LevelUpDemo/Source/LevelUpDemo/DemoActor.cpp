// Fill out your copyright notice in the Description page of Project Settings.


#include "DemoActor.h"

// Sets default values
ADemoActor::ADemoActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	RootComponent = SkeletalMeshComponent;

	UE_LOG(LogTemp, Error, TEXT("Before Upload ADemoActor SkeletonMesh!"))
	ConstructorHelpers::FObjectFinder<USkeletalMesh> Valueable_Name(TEXT("/Game/Characters/Mannequins/Meshes/SKM_Quinn_Simple"));
	UE_LOG(LogTemp, Error, TEXT("After Upload ADemoActor SkeletonMesh!"))
	USkeletalMesh* SkeletalMeshAsset = Valueable_Name.Object;
	SetSkeletalMesh(SkeletalMeshAsset);


}

// Called when the game starts or when spawned
void ADemoActor::BeginPlay()
{
	Super::BeginPlay();
	gameInst = Cast<ULevelUpGameInstance>(GetWorld()->GetGameInstance());
}

// Called every frame
void ADemoActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (gameInst) {
		for (FActorInfo& actor_info : gameInst->ACTOR_LIST) {
			if (actor_info.player_id == m_id) {
				FVector NewLocation;
				NewLocation.X = actor_info.x;
				NewLocation.Y = actor_info.y;
				NewLocation.Z = actor_info.z;
				SetActorLocation(NewLocation);
				UE_LOG(LogTemp, Log, TEXT("DemoActor PLAYER_ID  :: %d"), m_id);
			}
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("DemoActor Dosen`t gameInst"));
	}

}

void ADemoActor::SetSkeletalMesh(USkeletalMesh* Mesh)
{
	if (SkeletalMeshComponent && Mesh)
	{
		SkeletalMeshComponent->SetSkeletalMesh(Mesh);

	}


}
