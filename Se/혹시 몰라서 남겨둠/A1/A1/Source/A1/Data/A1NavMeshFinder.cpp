#include "Data/A1NavMeshFinder.h"
#include "NavMesh/RecastNavMesh.h"
#include "NavigationData.h"
#include "AI/NavDataGenerator.h"
#include "NavigationSystem.h"
#include "A1LogChannels.h"
#include "Detour/DetourNavMesh.h"
#include "DrawDebugHelpers.h"

AA1NavMeshFinder::AA1NavMeshFinder()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AA1NavMeshFinder::BeginPlay()
{
	Super::BeginPlay();
	GenerateNavMesh();
}

void AA1NavMeshFinder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	

}

void AA1NavMeshFinder::GenerateNavMesh()
{
	// navmesh 정점 좌표들
	{
		/*auto NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
		if (NavSys)
		{
			ARecastNavMesh* NavMesh = Cast<ARecastNavMesh>(NavSys->GetDefaultNavDataInstance
			(FNavigationSystem::DontCreate));
			{
				if (NavMesh)
				{
					FRecastDebugGeometry NavMeshGeometry;
					NavMesh->GetDebugGeometry(NavMeshGeometry);


					NavMeshGeometry.AreaIndices;

					FString SavePath = FPaths::ProjectDir() / TEXT("Saved") / TEXT("SavedMaps") / TEXT("NavMesh.txt");
					FString FileContent;

					int32 Count = 0;
					for (const FVector& Vert : NavMeshGeometry.MeshVerts)
					{
						FileContent += FString::Printf(TEXT("X: %f, Y: %f, Z: %f\n"), Vert.X, Vert.Y, Vert.Z);
						DrawDebugSphere(GetWorld(), Vert, 10.f, 12, FColor::Cyan, true, 10.f);
						++Count;
						//if (Count == 5)break;
					}
					FFileHelper::SaveStringToFile(FileContent, *SavePath);
				    UE_LOG(LogA1, Log, TEXT("Navmesh Point Count = %d"), Count);
				}
			}
		}*/
	}

	// 그냥 맵 데이터 obj
	{
		/*auto NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
		if (NavSys) {
			ANavigationData* NavData = NavSys->GetDefaultNavDataInstance();
			if (NavData) {
				ARecastNavMesh* NavMesh = Cast<ARecastNavMesh>(NavData);
				if (NavMesh) {
					auto generator = NavMesh->GetGenerator();
					if (generator) {
						FString SavePath2 = FPaths::ProjectDir() / TEXT("Saved") / TEXT("SavedMaps");
						generator->ExportNavigationData(SavePath2);

						TArray<FVector> OutDatas;
						NavMesh->GetPolyVerts(0, OutDatas);

					}
				}
			}
		}*/
	}

	// 실패한 시도
	{
		//auto NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
		//ANavigationData* NavData = NavSys->GetDefaultNavDataInstance();
		//ARecastNavMesh* NavMesh = Cast<ARecastNavMesh>(NavData);
		//const dtNavMesh* DetourNavMesh = NavMesh->GetRecastMesh();

		//int32 TriangleCount = 0;
		//int32 TestCount = 0;
		//if (DetourNavMesh)
		//{
		//	int32 MaxTiles = DetourNavMesh->getMaxTiles();
		//	for (int32 TileIndex = 0; TileIndex < MaxTiles; ++TileIndex)
		//	{
		//		const dtMeshTile* Tile = DetourNavMesh->getTile(TileIndex);
		//		if (Tile && Tile->header)
		//		{
		//			int32 PolyCount = Tile->header->polyCount;
		//			for (int32 PolyIndex = 0; PolyIndex < PolyCount; ++PolyIndex)
		//			{
		//				const dtPoly* Poly = &Tile->polys[PolyIndex];

		//				if (Poly->getType() == DT_POLYTYPE_GROUND)
		//				{
		//					int32 VertCount = Poly->vertCount;
		//					for (int32 VertIndex = 0; VertIndex < VertCount; ++VertIndex)
		//					{
		//						// Vert[0] = Y값
		//						// Vert[1] = Z값  - 고정
		//						// Vert[2] = X값
		//						const double* Vert = &Tile->verts[Poly->verts[VertIndex] * 3];
		//						UE_LOG(LogTemp, Log, TEXT("Vertex[%d]: (%f, %f, %f)"), VertIndex, Vert[2], Vert[0], Vert[1]);
		//					}

		//					for (int32 TriIndex = 0; TriIndex < VertCount - 2; ++TriIndex)
		//					{
		//						const double* v0 = &Tile->verts[Poly->verts[0] * 3];
		//						const double* v1 = &Tile->verts[Poly->verts[TriIndex + 1] * 3];
		//						const double* v2 = &Tile->verts[Poly->verts[TriIndex + 2] * 3];
		//						UE_LOG(LogTemp, Log, TEXT("Triangle: (%f, %f, %f) - (%f, %f, %f) - (%f, %f, %f)"),
		//							v0[2], v0[0], v0[1],
		//							v1[2], v1[0], v1[1],
		//							v2[2], v2[0], v2[1]);

		//						TriangleCount++;
		//						
		//					}
		//				}
		//			}
		//		}
		//	}
		//}

	}

	// 대훈이가 올려준 거
	{
		//UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
		//if (NavSys == nullptr) return;

		//ARecastNavMesh* NavMesh = Cast<ARecastNavMesh>(NavSys->GetMainNavData());
		//if (NavMesh == nullptr) return;

		//const FBox NavBounds = NavMesh->GetNavMeshBounds();


		//const float TileSize = NavMesh->TileSizeUU;
		//const FIntPoint TileCount = FIntPoint(
		//	FMath::CeilToInt(NavBounds.GetSize().X / TileSize),
		//	FMath::CeilToInt(NavBounds.GetSize().Y / TileSize)
		//);


		//TArray<FVector> TileCenters;


		//for (int32 TileY = 0; TileY < TileCount.Y; ++TileY)
		//{
		//	for (int32 TileX = 0; TileX < TileCount.X; ++TileX)
		//	{
		//		FVector TileCenter = FVector(
		//			NavBounds.Min.X + TileX * TileSize + TileSize / 2,
		//			NavBounds.Min.Y + TileY * TileSize + TileSize / 2,
		//			NavBounds.GetCenter().Z
		//		);

		//		TileCenters.Add(TileCenter);
		//	}
		//}

		//int32 Count = 0;

		//for (int32 TileX = 0; TileX < TileCount.X - 1; ++TileX)
		//{
		//	for (int32 TileY = 0; TileY < TileCount.Y - 1; ++TileY)
		//	{
		//		//상대좌표
		//		FVector Triangle1[] = {
		//			FVector(TileX, TileY, 0),
		//			FVector(TileX + 1, TileY, 0),
		//			FVector(TileX, TileY + 1, 0)
		//		};

		//		FVector Triangle2[] = {
		//			FVector(TileX + 1, TileY, 0),
		//			FVector(TileX, TileY + 1, 0),
		//			FVector(TileX + 1, TileY + 1, 0)
		//		};

		//		//절대좌표
		//		FVector WorldTriangle1[] = {
		//			TileCenters[TileX + TileY * TileCount.X],
		//			TileCenters[(TileX + 1) + TileY * TileCount.X],
		//			TileCenters[TileX + (TileY + 1) * TileCount.X]
		//		};

		//		FVector WorldTriangle2[] = {
		//			TileCenters[(TileX + 1) + TileY * TileCount.X],
		//			TileCenters[TileX + (TileY + 1) * TileCount.X],
		//			TileCenters[(TileX + 1) + (TileY + 1) * TileCount.X]
		//		};
		//		
		//		if (Count == 0) {

		//			DrawDebugLine(GetWorld(), WorldTriangle1[0], WorldTriangle1[1], FColor::Black, false, 10.f, 0, 5.0f);
		//			//DrawDebugLine(GetWorld(), WorldTriangle1[1], WorldTriangle1[2], FColor::Black, false, 10.f, 0, 5.0f);
		//			//DrawDebugLine(GetWorld(), WorldTriangle1[2], WorldTriangle1[0], FColor::Black, false, 10.f, 0, 5.0f);

		//			/*DrawDebugLine(GetWorld(), WorldTriangle2[0], WorldTriangle2[1], FColor::Cyan, false, 5.f, 0, 5.0f);
		//			DrawDebugLine(GetWorld(), WorldTriangle2[1], WorldTriangle2[2], FColor::Cyan, false, 5.f, 0, 5.0f);
		//			DrawDebugLine(GetWorld(), WorldTriangle2[2], WorldTriangle2[0], FColor::Cyan, false, 5.f, 0, 5.0f);*/
		//		}
		//		++Count;
		//	}
		//}
	}

	// 버텍스, 인덱스 버퍼 출력
	{
		/*auto NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
		ARecastNavMesh* NavMesh = Cast<ARecastNavMesh>(NavSys->GetDefaultNavDataInstance(FNavigationSystem::DontCreate));
		TArray<FVector> VertexBuffer;
		TArray<int32> IndexBuffer;

		const dtNavMesh* DetourNavMesh = NavMesh->GetRecastMesh();
		if (NavMesh == nullptr) return;
		
		for (int32 TileIndex = 0; TileIndex < DetourNavMesh->getMaxTiles(); ++TileIndex)
		{
			const dtMeshTile* Tile = DetourNavMesh->getTile(TileIndex);
			if (Tile && Tile->header)
			{
				const dtPoly* Poly = Tile->polys;
				for (int32 PolyIndex = 0; PolyIndex < Tile->header->polyCount; ++PolyIndex)
				{
					const dtPoly& CurrentPoly = Poly[PolyIndex];

					for (int32 VertIndex = 0; VertIndex < CurrentPoly.vertCount; ++VertIndex)
					{
						const double* Vert = &Tile->verts[CurrentPoly.verts[VertIndex] * 3];
						VertexBuffer.Add(FVector(Vert[0], Vert[2], Vert[1]));
					}

					IndexBuffer.Add(CurrentPoly.verts[0]);
					IndexBuffer.Add(CurrentPoly.verts[1]);
					IndexBuffer.Add(CurrentPoly.verts[3]);
					
					IndexBuffer.Add(CurrentPoly.verts[1]);
					IndexBuffer.Add(CurrentPoly.verts[2]);
					IndexBuffer.Add(CurrentPoly.verts[3]);
				}
			}
		}
	

		int32 Count = 0;
		int32 Index = -4;
		for (int i = 0; i < VertexBuffer.Num(); i += 3)
		{
			if (Count % 2 == 0) Index += 4;
			Count++;

			TArray<FVector> Vertex = {
			VertexBuffer[Index],
			VertexBuffer[Index + 1],
			VertexBuffer[Index + 2],
			VertexBuffer[Index + 3]
			};

			FVector V0 = Vertex[IndexBuffer[i]];
			FVector V1 = Vertex[IndexBuffer[i + 1]];
			FVector V2 = Vertex[IndexBuffer[i + 2]];

			DrawDebugLine(GetWorld(), V0, V1, FColor::Green, true, -1, 0, 5.0f);
			DrawDebugLine(GetWorld(), V1, V2, FColor::Green, true, -1, 0, 5.0f);
			DrawDebugLine(GetWorld(), V2, V0, FColor::Green, true, -1, 0, 5.0f);
		}*/
	}

	// 뭔가 시도 했는데 실패
	{
		/*auto NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
		ARecastNavMesh* NavMesh = Cast<ARecastNavMesh>(NavSys->GetDefaultNavDataInstance(FNavigationSystem::DontCreate));
		TArray<FVector> VertexBuffer;
		TArray<int32> IndexBuffer;

		const dtNavMesh* DetourNavMesh = NavMesh->GetRecastMesh();
		if (NavMesh == nullptr) return;

		for (int32 TileIndex = 0; TileIndex < DetourNavMesh->getMaxTiles(); ++TileIndex)
		{
			const dtMeshTile* Tile = DetourNavMesh->getTile(TileIndex);
			if (Tile && Tile->header)
			{
				const dtPoly* Poly = Tile->polys;
				for (int32 PolyIndex = 0; PolyIndex < Tile->header->polyCount; ++PolyIndex)
				{
					const dtPoly& CurrentPoly = Poly[PolyIndex];

					for (int32 VertIndex = 0; VertIndex < CurrentPoly.vertCount; ++VertIndex)
					{
						const double* Vert = &Tile->verts[CurrentPoly.verts[VertIndex] * 3];
						VertexBuffer.Add(FVector(Vert[0], Vert[2], Vert[1]));
					}

					for (int32 LinkIndex = 0; LinkIndex < CurrentPoly.vertCount - 2; ++LinkIndex)
					{
						IndexBuffer.Add(CurrentPoly.verts[0]);
						IndexBuffer.Add(CurrentPoly.verts[LinkIndex + 1]);
						IndexBuffer.Add(CurrentPoly.verts[LinkIndex + 2]);
					}
				}
			}
		}
		
		int32 NumVertices = VertexBuffer.Num();
		for (int32 i = 0; i < IndexBuffer.Num(); i += 3)
		{
			if (IndexBuffer[i] < NumVertices && IndexBuffer[i + 1] < NumVertices && IndexBuffer[i + 2] < NumVertices)
			{
				FVector V0 = VertexBuffer[IndexBuffer[i]];
				FVector V1 = VertexBuffer[IndexBuffer[i + 1]];
				FVector V2 = VertexBuffer[IndexBuffer[i + 2]];
				
				DrawDebugLine(GetWorld(), V0, V1, FColor::Green, true, -1, 0, 5.0f);
				DrawDebugLine(GetWorld(), V1, V2, FColor::Green, true, -1, 0, 5.0f);
				DrawDebugLine(GetWorld(), V2, V0, FColor::Green, true, -1, 0, 5.0f);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Invalid index in Indices: %d, %d, %d"), IndexBuffer[i], IndexBuffer[i + 1], IndexBuffer[i + 2]);
			}
		}*/
	}

	{
		

	}
}

