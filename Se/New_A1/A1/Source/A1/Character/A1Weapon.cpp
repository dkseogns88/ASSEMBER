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
		GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, FireRate, false);
	}
}

void AA1Weapon::FireOFF()
{
	GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
}

void AA1Weapon::Fire()
{
	AA1Character* WeaponOwner = Cast<AA1Character>(GetOwner());

	if (WeaponSound) {
		UGameplayStatics::PlaySoundAtLocation(this, WeaponSound, GetActorLocation());
		UGameplayStatics::SpawnEmitterAttached(FlashEmitter, SkeletalMesh, TEXT("TIP"));
		FVector Location;
		AActor* HitActor;
		UPrimitiveComponent* HitComponent;
		FName BoneName;
		FVector ReturnVector;
		ReturnVector = TraceCamera(Range, Location, HitActor, HitComponent, BoneName);

		FVector ImpactPoint;
		UPrimitiveComponent* SocketComponent;
		TraceSocket(Location, ImpactPoint, SocketComponent);
		FTransform SpawnEmitterTransform;

		if (WeaponOwner->IsMyPlayer() == false)
		{
			SpawnEmitterTransform.SetLocation(ImpactLocationFromServer);
		}
		else {
			if ((HitComponent != SocketComponent) && (ImpactPoint != FVector(0.f, 0.f, 0.f)))
			{
				SpawnEmitterTransform.SetLocation(ImpactPoint);
			}
			else
			{
				SpawnEmitterTransform.SetLocation(Location);
			}
		}
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEmitter, SpawnEmitterTransform, true, EPSCPoolMethod::None, true);
	
		// ToServer
		if (WeaponOwner->IsMyPlayer())
		{
			if (AA1MyPlayer* MyPlayer = Cast<AA1MyPlayer>(WeaponOwner)) {
				Protocol::C_ATTACK AttackPkt;
				Protocol::AttackInfo* Info = AttackPkt.mutable_info();
				Info->set_attack_object_id(MyPlayer->GetObjectInfo()->object_id());
				Info->set_impact_location_x(SpawnEmitterTransform.GetLocation().X);
				Info->set_impact_location_y(SpawnEmitterTransform.GetLocation().Y);
				Info->set_impact_location_z(SpawnEmitterTransform.GetLocation().Z);

				MyPlayer->GetNetworkManager()->SendPacket(AttackPkt);
			}
		}
	}
}

FVector AA1Weapon::TraceCamera(float WeaponRange, FVector& Location, AActor*& HitActor, UPrimitiveComponent*& HitComponent, FName& BoneName)
{
	FVector WorldLocation;
	FVector ForwardVector; 
	if (OverlapActor->GetClass()->ImplementsInterface(UA1PickupInterface::StaticClass()))
	{
		IA1PickupInterface* PickupInterface = Cast<IA1PickupInterface>(OverlapActor);
		if (PickupInterface)
		{
			PickupInterface->GetCamera(WorldLocation, ForwardVector);
		}
	}
	
	FVector End = (ForwardVector * WeaponRange) + WorldLocation;

	FHitResult HitResult;
	ETraceTypeQuery TraceChannel = UEngineTypes::ConvertToTraceType(ECC_Visibility);
	TArray<AActor*> ActorsToIgnore;

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
	return ForwardVector * WeaponRange;

}

void AA1Weapon::TraceSocket(FVector End, FVector& ImpactPoint, UPrimitiveComponent*& HitComponent)
{
	FVector Start = SkeletalMesh->GetSocketLocation(FName());
	ETraceTypeQuery TraceChannel = UEngineTypes::ConvertToTraceType(ECC_Visibility);
	TArray<AActor*> ActorsToIgnore;
	FHitResult HitResult;

	UKismetSystemLibrary::LineTraceSingle(
		GetWorld(),
		Start,
		End,
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

	ImpactPoint = HitResult.ImpactPoint;
	HitComponent = HitResult.GetComponent();
}

void AA1Weapon::SetImpacLocation(float x, float y, float z)
{
	ImpactLocationFromServer.X = x;
	ImpactLocationFromServer.Y = y;
	ImpactLocationFromServer.Z = z;
}


