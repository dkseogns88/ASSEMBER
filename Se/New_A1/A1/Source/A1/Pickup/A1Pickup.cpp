#include "Pickup/A1Pickup.h"
#include "Components/SphereComponent.h"
#include "A1PickupInterface.h"

AA1Pickup::AA1Pickup()
{
	PrimaryActorTick.bCanEverTick = false;

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	RootComponent = SkeletalMesh;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(SkeletalMesh);

	// 충돌 이벤트 등록
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AA1Pickup::OnSphereOverlapBegin);
}

void AA1Pickup::BeginPlay()
{
	Super::BeginPlay();
	
}

void AA1Pickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AA1Pickup::OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		if (OtherActor->GetClass()->ImplementsInterface(UA1PickupInterface::StaticClass()))
		{
			// 인터페이스 함수 호출
			IA1PickupInterface* PickupInterface = Cast<IA1PickupInterface>(OtherActor);
			if (PickupInterface)
			{
				bool Success = false;
				PickupInterface->PickupWeapon(ItemName, Success);
				if (Success)
				{
					Destroy();
				}
			}
		}
	}
}