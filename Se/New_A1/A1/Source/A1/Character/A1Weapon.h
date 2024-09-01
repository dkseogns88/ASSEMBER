

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "A1WeaponInterface.h"
#include "A1Weapon.generated.h"

UCLASS()
class A1_API AA1Weapon : public AActor, public IA1WeaponInterface
{
	GENERATED_BODY()
	
public:	
	AA1Weapon();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION()
	void OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void UpdateValues();

	virtual void FireON()	override;
	virtual void FireOFF()	override;
	void Fire();
	
	void TraceCamera(float WeaponRange, FVector& Location, AActor*& HitActor, UPrimitiveComponent*& HitComponent, FName& BoneName, FVector& Impulse);
	void TraceSocket(FVector Location, AActor* HitActor, UPrimitiveComponent* HitComponent, FName BoneName, FVector Impulse, 
		FVector& OutLocation, AActor*& OutHitActor, UPrimitiveComponent*& OutHitComponent, FName& OutBoneName, FVector& OutImpulse);
	void ApplyPhysics(FVector Location, FVector Impulse, UPrimitiveComponent* HitComponent, FName BoneName);

public:
	void SetWorldLocation(float x, float y, float z);
	void SetForwardVector(float x, float y, float z);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* SphereCollision;


public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FName WeaponName;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float FireRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bAutomatic;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USoundBase* WeaponSound;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UParticleSystem* FlashEmitter;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float Range;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UParticleSystem* ImpactEmitter;

	FTimerHandle FireTimerHandle;

	AActor* OverlapActor; // Owner

	
	// To Server
	FVector WorldLocationToServer;
	FVector ForwardVectorToServer;

private:


	// From Server
	FVector WorldLocationFromServer;
	FVector ForwardVectorFromServer;

};
