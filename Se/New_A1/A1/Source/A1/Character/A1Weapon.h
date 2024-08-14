

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "A1Weapon.generated.h"

UCLASS()
class A1_API AA1Weapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AA1Weapon();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void UpdateValues();

	UFUNCTION()
	void OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* SphereCollision;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FName WeaponName;

};
