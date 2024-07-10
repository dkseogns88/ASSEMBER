#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "A1PathFinder.generated.h"

UCLASS()
class A1_API AA1PathFinder : public AActor
{
	GENERATED_BODY()
public:
	AA1PathFinder();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere)
	float LevelBoundary = 0.f;

	UPROPERTY(EditAnywhere)
	float _GridSize = 0.f;

	TArray<FVector> GenerateNodes(UWorld* World, float GridSize);
	bool IsLocationNavigable(UWorld* World, FVector Location);
	TMap<FVector, TArray<FVector>> GenerateEdges(const TArray<FVector>& Nodes, float GridSize);
	void SaveMapToFile(const TMap<FVector, TArray<FVector>>& Map, const FString& FilePath);

	const TArray<FVector> NeighborOffsets = {
	FVector(1, 0, 0), FVector(-1, 0, 0),
	FVector(0, 1, 0), FVector(0, -1, 0)
	};

	TArray<FVector> NodeArr;
	TMap<FVector, TArray<FVector>> EdgesMap;

};
