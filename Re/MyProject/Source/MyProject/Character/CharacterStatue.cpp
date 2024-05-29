// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterStatue.h"
#include "Components/BoxComponent.h"
#include "MyProjectPlayerController.h"

// Sets default values
ACharacterStatue::ACharacterStatue()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
    CollisionBox->SetupAttachment(RootComponent);
    CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ACharacterStatue::OnOverlapBegin);
    CollisionBox->OnComponentEndOverlap.AddDynamic(this, &ACharacterStatue::OnOverlapEnd);
}

// Called when the game starts or when spawned
void ACharacterStatue::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACharacterStatue::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACharacterStatue::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != this)
    {
        APawn* PlayerPawn = Cast<APawn>(OtherActor);
        if (PlayerPawn)
        {
            AMyProjectPlayerController* PlayerController = Cast<AMyProjectPlayerController>(PlayerPawn->GetController());
            if (PlayerController)
            {
                PlayerController->SetNearbyStatue(this);
            }
        }
    }
}

void ACharacterStatue::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor && OtherActor != this)
    {
        APawn* PlayerPawn = Cast<APawn>(OtherActor);
        if (PlayerPawn)
        {
            AMyProjectPlayerController* PlayerController = Cast<AMyProjectPlayerController>(PlayerPawn->GetController());
            if (PlayerController)
            {
                PlayerController->SetNearbyStatue(nullptr);
            }
        }
    }
}


void ACharacterStatue::OnInteract(APlayerController* PlayerController)
{
    if (PlayerController)
    {
        AMyProjectPlayerController* MyController = Cast<AMyProjectPlayerController>(PlayerController);
        if (MyController)
        {
            UE_LOG(LogTemp, Log, TEXT("try to CharacterChange"));
            MyController->ChangeCharacter(CharacterName);
        }
    }
}