
#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "GradAssetManager.generated.h"

UCLASS()
class GRADGAME_API UGradAssetManager : public UAssetManager
{
	GENERATED_BODY()
	
public:
	UGradAssetManager();

	static UGradAssetManager& Get();

	/**
	* UAssetManager's interfaces
	*/
	virtual void StartInitialLoading() final;
	/**
	 * �ش� �Լ��� FScopeLogTime�� ������� ������ ���� �Ǵ��� ���� ���
	 * - ���� Ŭ�� �ڵ��� �ʿ������, �˾Ƶθ� ���� ������ ������ �غ���
	 */
	static bool ShouldLogAssetLoads();

	static UObject* SynchronousLoadAsset(const FSoftObjectPath& AssetPath);

	/**
	 * GetAsset�� ���� �ε����� BP Class�� Object �ε�
	 * - �ָ��ؾ� �� �κ��� bKeepInMemory
	 */
	template <typename AssetType>
	static AssetType* GetAsset(const TSoftObjectPtr<AssetType>& AssetPointer, bool bKeepInMemory = true);

	template <typename AssetType>
	static TSubclassOf<AssetType> GetSubclass(const TSoftClassPtr<AssetType>& AssetPointer, bool bKeepInMemory = true);

	/** [THREAD-SAFE] �޸𸮿� �ε��� ���� ĳ�� */
	void AddLoadedAsset(const UObject* Asset);

	// GC�� ���
	UPROPERTY()
	TSet<TObjectPtr<const UObject>> LoadedAssets;

	// Object ���� Locking
	FCriticalSection SyncObject;
	
};

template <typename AssetType>
AssetType* UGradAssetManager::GetAsset(const TSoftObjectPtr<AssetType>& AssetPointer, bool bKeepsInMemory)
{
	AssetType* LoadedAsset = nullptr; 
	const FSoftObjectPath& AssetPath = AssetPointer.ToSoftObjectPath();

	if (AssetPath.IsValid())
	{
		// �ε��� �Ǿ��ִ�? -> �ٷ� ������
		// �ε��� �ȵǾ� �ִ� -> Null
		LoadedAsset = AssetPointer.Get();
		if (!LoadedAsset)
		{
			LoadedAsset = Cast<AssetType>(SynchronousLoadAsset(AssetPath));
			ensureAlwaysMsgf(LoadedAsset, TEXT("Failed to load asset [%s]"), *AssetPointer.ToString());
		}

		if (LoadedAsset && bKeepsInMemory)
		{
			// ���⼭ AddLoadAsset�� �޸𸮿� �����ϱ� ���� ��ġ��� �����ϸ� ��:
			// - �ѹ� ��ϵǸ� ���� ������ �ʴ��� Unload�� ���� ���� (== ĳ��)
			// - GC�� �������� �ʱ� �ϱ� ����
			Get().AddLoadedAsset(Cast<UObject>(LoadedAsset));
		}
	}

	return LoadedAsset;
}

template <typename AssetType>
TSubclassOf<AssetType> UGradAssetManager::GetSubclass(const TSoftClassPtr<AssetType>& AssetPointer, bool bKeepInMemory)
{
	TSubclassOf<AssetType> LoadedSubclass;

	const FSoftObjectPath& AssetPath = AssetPointer.ToSoftObjectPath();
	if (AssetPath.IsValid())
	{
		LoadedSubclass = AssetPointer.Get();
		if (!LoadedSubclass)
		{
			LoadedSubclass = Cast<UClass>(SynchronousLoadAsset(AssetPath));
			ensureAlwaysMsgf(LoadedSubclass, TEXT("Failed to load asset class [%s]"), *AssetPointer.ToString());
		}

		if (LoadedSubclass && bKeepInMemory)
		{
			Get().AddLoadedAsset(Cast<UObject>(LoadedSubclass));
		}
	}

	return LoadedSubclass;
}