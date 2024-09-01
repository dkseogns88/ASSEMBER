#include "Character/A1Weapon.h"
#include "Components/SphereComponent.h"
#include "../Pickup/A1PickupInterface.h"
#include "Kismet/GameplayStatics.h"
#include "A1MyPlayer.h"
#include "Network/A1NetworkManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

AA1Weapon::AA1Weapon()
{
	PrimaryActorTick.bCanEverTick = true;

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
				OverlapActor = Cast<AA1Character>(OtherActor);
			}
		}
	}
}


void AA1Weapon::FireON()
{
	if (!GetWorld()->GetTimerManager().IsTimerActive(FireTimerHandle))
	{
		Fire();
		GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, FireRate, false); // TODO: 총 발사 방식 바꿔야 함
	}
}

void AA1Weapon::FireOFF()
{
	GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
}

void AA1Weapon::Fire()
{
	if (WeaponSound) {
		UGameplayStatics::PlaySoundAtLocation(this, WeaponSound, GetActorLocation());
		UGameplayStatics::SpawnEmitterAttached(FlashEmitter, SkeletalMesh, TEXT("TIP"));
		FVector Location;
		AActor* HitActor;
		UPrimitiveComponent* HitComponent;
		FName BoneName;
		FVector Impulse;
		TraceCamera(Range, Location, HitActor, HitComponent, BoneName, Impulse);

		FVector OutLocation;
		AActor* OutHitActor;
		UPrimitiveComponent* OutHitComponent;
		FName OutBoneName;
		FVector OutImpulse;
		TraceSocket(Location, HitActor, HitComponent, BoneName, Impulse, OutLocation, OutHitActor, OutHitComponent, OutBoneName, OutImpulse);
		
		FTransform SpawnEmitterTransform;
		SpawnEmitterTransform.SetLocation(OutLocation);

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEmitter, SpawnEmitterTransform, true, EPSCPoolMethod::None, true);
		// ApplyPhysics(OutLocation, OutImpulse, OutHitComponent, OutBoneName);

		AA1Character* WeaponOwner = Cast<AA1Character>(GetOwner());
		if (WeaponOwner->IsMyPlayer() == false) return;

		UGameplayStatics::ApplyDamage(OutHitActor, 10.f, nullptr, this, nullptr);

		
	}
}

void AA1Weapon::TraceCamera(float WeaponRange, FVector& Location, AActor*& HitActor, UPrimitiveComponent*& HitComponent, FName& BoneName, FVector& Impulse)
{
	AA1Character* WeaponOwner = Cast<AA1Character>(GetOwner());

	FVector WorldLocation;
	FVector ForwardVector;
	if (OverlapActor->GetClass()->ImplementsInterface(UA1PickupInterface::StaticClass()))
	{
		IA1PickupInterface* PickupInterface = Cast<IA1PickupInterface>(OverlapActor);
		if (PickupInterface)
		{
			// ToServer
			if (WeaponOwner->IsMyPlayer()) {
				if (AA1MyPlayer* MyPlayer = Cast<AA1MyPlayer>(WeaponOwner)) {
					PickupInterface->GetCamera(WorldLocation, ForwardVector);

					WorldLocationToServer = WorldLocation;
					ForwardVectorToServer = ForwardVector;

				}
				
			}
			else { // FromServer
				WorldLocation = WorldLocationFromServer;
				ForwardVector = ForwardVectorFromServer;
			}
			
		}
	}
	
	FVector End = (ForwardVector * WeaponRange) + WorldLocation;

	FHitResult HitResult;
	ETraceTypeQuery TraceChannel = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel1);
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(OverlapActor);
	bool bHit = UKismetSystemLibrary::LineTraceSingle(
		GetWorld(),
		WorldLocation,
		End,
		TraceChannel,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		HitResult,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		1.f
		);

	bool EqualSelect = UKismetMathLibrary::EqualEqual_VectorVector(HitResult.Location, FVector(0.f, 0.f, 0.f), 0.f);
	if (EqualSelect) Location = HitResult.TraceEnd;
	else Location = HitResult.Location;

	HitActor = HitResult.GetActor();
	HitComponent = HitResult.GetComponent();
	BoneName = HitResult.BoneName;
	Impulse = ForwardVector * WeaponRange;

}

void AA1Weapon::TraceSocket(FVector Location, AActor* HitActor, UPrimitiveComponent* HitComponent, FName BoneName, FVector Impulse,
	FVector& OutLocation, AActor*& OutHitActor, UPrimitiveComponent*& OutHitComponent, FName& OutBoneName, FVector& OutImpulse)
{
	FVector Start = SkeletalMesh->GetSocketLocation(FName("TIP"));
	ETraceTypeQuery TraceChannel = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel1); // TODO: 권총 Error..
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(OverlapActor);
	ActorsToIgnore.Add(this);
	FHitResult HitResult;

	UKismetSystemLibrary::LineTraceSingle(
		GetWorld(),
		Start,
		Location,
		TraceChannel,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		HitResult,
		true,
		FLinearColor::Blue,
		FLinearColor::Green,
		1.f
	);

	if ((HitResult.ImpactPoint != FVector(0.f, 0.f, 0.f)) && (HitComponent != HitResult.GetComponent()))
	{
		OutLocation = HitResult.ImpactPoint;
		OutHitActor = HitResult.GetActor();   
		OutHitComponent = HitResult.GetComponent();
		OutBoneName = HitResult.BoneName;
		OutImpulse = Impulse;
	}
	else
	{
		OutLocation = Location;
		OutHitActor = HitActor;
		OutHitComponent = HitComponent;
		OutBoneName = BoneName;
		OutImpulse = Impulse;
	}

	Impulse = HitResult.ImpactPoint;
	HitComponent = HitResult.GetComponent();
}

void AA1Weapon::ApplyPhysics(FVector Location, FVector Impulse, UPrimitiveComponent* HitComponent, FName BoneName)
{
	if (HitComponent) {
		bool IsSimulate = HitComponent->IsSimulatingPhysics(BoneName);
		if (IsSimulate) {
			FVector AddImpulse = Impulse * 2.f;
			HitComponent->AddImpulseAtLocation(AddImpulse, Location, BoneName);
		}
	}
}

void AA1Weapon::SetWorldLocation(float x, float y, float z)
{
	WorldLocationFromServer.X = x;
	WorldLocationFromServer.Y = y;
	WorldLocationFromServer.Z = z;
}

void AA1Weapon::SetForwardVector(float x, float y, float z)
{
	ForwardVectorFromServer.X = x;
	ForwardVectorFromServer.Y = y;
	ForwardVectorFromServer.Z = z;
}
