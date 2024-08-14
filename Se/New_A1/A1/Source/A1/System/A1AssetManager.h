

#pragma once

#include "A1LogChannels.h"
#include "Data/A1AssetData.h"
#include "Engine/AssetManager.h"
#include "A1AssetManager.generated.h"

class UA1AssetData;

DECLARE_DELEGATE_TwoParams(FAsyncLoadCompletedDelegate, const FName&/*AssetName or Label*/, UObject*/*LoadedAsset*/);

UCLASS()
class A1_API UA1AssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	UA1AssetManager();

	static UA1AssetManager& Get();

public:
	static void Initialize();

	template<typename AssetType>
	static AssetType* GetAssetByName(const FName& AssetName);

	static void LoadSyncByPath(const FSoftObjectPath& AssetPath);
	static void LoadSyncByName(const FName& AssetName);
	static void LoadSyncByLabel(const FName& Label);

	static void LoadAsyncByPath(const FSoftObjectPath& AssetPath, FAsyncLoadCompletedDelegate CompletedDelegate = FAsyncLoadCompletedDelegate());
	static void LoadAsyncByName(const FName& AssetName, FAsyncLoadCompletedDelegate CompletedDelegate = FAsyncLoadCompletedDelegate());

	static void ReleaseByPath(const FSoftObjectPath& AssetPath);
	static void ReleaseByName(const FName& AssetName);
	static void ReleaseByLabel(const FName& Label);
	static void ReleaseAll();

private:
	void LoadPreloadAssets();
	void AddLoadedAsset(const FName& AssetName, const UObject* Asset);

private:
	UPROPERTY()
	TObjectPtr<UA1AssetData> LoadedAssetData;

	UPROPERTY()
	TMap<FName, TObjectPtr<const UObject>> NameToLoadedAsset;
};

template<typename AssetType>
AssetType* UA1AssetManager::GetAssetByName(const FName& AssetName)
{
	UA1AssetData* AssetData = Get().LoadedAssetData;
	check(AssetData);

	AssetType* LoadedAsset = nullptr;
	const FSoftObjectPath& AssetPath = AssetData->GetAssetPathByName(AssetName);
	if (AssetPath.IsValid())
	{
		LoadedAsset = Cast<AssetType>(AssetPath.ResolveObject());
		if (LoadedAsset == nullptr)
		{
			UE_LOG(LogA1, Warning, TEXT("Attempted sync loading because asset hadn't loaded yet [%s]."), *AssetPath.ToString());
			LoadedAsset = Cast<AssetType>(AssetPath.TryLoad());
		}
	}
	return LoadedAsset;
}
