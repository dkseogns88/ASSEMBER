#include "Data/A1PathFinder.h"

AA1PathFinder::AA1PathFinder()
{
    PrimaryActorTick.bCanEverTick = true;

}

void AA1PathFinder::BeginPlay()
{
    Super::BeginPlay();

    FString SavePath = FPaths::ProjectDir() / TEXT("Saved") / TEXT("SavedMaps") / TEXT("EdgesMap.txt");

    GenerateEdges(GenerateNodes(GetWorld(), _GridSize), _GridSize);
    SaveMapToFile(EdgesMap, SavePath);
}

void AA1PathFinder::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

TArray<FVector> AA1PathFinder::GenerateNodes(UWorld* World, float GridSize)
{
    TArray<FVector> Nodes;

    // 레벨의 경계를 기준으로 노드를 생성
    for (float x = -LevelBoundary; x <= LevelBoundary; x += GridSize)
    {
        for (float y = -LevelBoundary; y <= LevelBoundary; y += GridSize)
        {
            FVector Location(x, y, 0);
            if (IsLocationNavigable(World, Location))
            {
                Nodes.Add(Location);
            }
        }
    }

    NodeArr = Nodes;

    return Nodes;
}

bool AA1PathFinder::IsLocationNavigable(UWorld* World, FVector Location)
{
    // 레이캐스트 등을 사용하여 Location이 이동 가능한지 확인
    FHitResult Hit;
    FVector Start = Location + FVector(0, 0, 100);
    FVector End = Location - FVector(0, 0, 100);
    FCollisionQueryParams Params;

    bool Result = !World->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);

    if (Result)
        DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1000.f);
    else
        DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1000.f);

    return Result;
}

TMap<FVector, TArray<FVector>> AA1PathFinder::GenerateEdges(const TArray<FVector>& Nodes, float GridSize)
{
    TMap<FVector, TArray<FVector>> Edges;

    for (const FVector& Node : Nodes)
    {
        TArray<FVector> Neighbors;
        for (const FVector& Offset : NeighborOffsets)
        {
            FVector Neighbor = Node + Offset * GridSize;
            if (Nodes.Contains(Neighbor))
            {
                Neighbors.Add(Neighbor);
            }
        }
        Edges.Add(Node, Neighbors);
    }

    EdgesMap = Edges;

    return Edges;
}

void AA1PathFinder::SaveMapToFile(const TMap<FVector, TArray<FVector>>& Map, const FString& FilePath)
{
    FString FileContent;

    for (const auto& Elem : Map)
    {
        FVector Key = Elem.Key;
        const TArray<FVector>& Values = Elem.Value;

        FileContent += FString::Printf(TEXT("Key: (%f,%f,%f)\n"), Key.X, Key.Y, Key.Z);

        for (int32 i = 0; i < Values.Num(); ++i)
        {
            FileContent += FString::Printf(TEXT("Value: (%f,%f,%f)\n"), Values[i].X, Values[i].Y, Values[i].Z);
        }
    }

    FFileHelper::SaveStringToFile(FileContent, *FilePath);
}