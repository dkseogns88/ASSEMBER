#include "Character/A1Weapon.h"
#include "Components/SphereComponent.h"
#include "../Pickup/A1PickupInterface.h"

AA1Weapon::AA1Weapon()
{
	PrimaryActorTick.bCanEverTick = false;

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	RootComponent = SkeletalMesh;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(SkeletalMesh);

	// 충돌 이벤트 등록
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AA1Weapon::OnSphereOverlapBegin);

}

void AA1Weapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AA1Weapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AA1Weapon::OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		if (OtherActor->GetClass()->ImplementsInterface(UA1PickupInterface::StaticClass()))
		{
			IA1PickupInterface* PickupInterface = Cast<IA1PickupInterface>(OtherActor);
			if (PickupInterface)
			{
				WeaponName = PickupInterface->GetWeaponName();
				UpdateValues();
				SphereCollision->OnComponentBeginOverlap.RemoveDynamic(this, &AA1Weapon::OnSphereOverlapBegin);
			}
		}
	}
}

