#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../A1Types.h"
#include "A1Pickup.generated.h"

UCLASS()
class A1_API AA1Pickup : public AActor
{
	GENERATED_BODY()
	
public:	
	AA1Pickup();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION()
	void OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* SphereCollision;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemCategories ItemCategory;
};
