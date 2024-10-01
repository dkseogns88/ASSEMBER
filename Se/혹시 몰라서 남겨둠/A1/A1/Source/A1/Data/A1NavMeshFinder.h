#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "A1NavMeshFinder.generated.h"

UCLASS()
class A1_API AA1NavMeshFinder : public AActor
{
	GENERATED_BODY()
	
public:	
	AA1NavMeshFinder();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;


	void GenerateNavMesh();
};
