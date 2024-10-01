// Fill out your copyright notice in the Description page of Project Settings.


#include "GradAssetManager.h"
#include "GradGame/GradGameplayTags.h"
#include "GradGame/GradLogChannels.h"

UGradAssetManager::UGradAssetManager()
{
}

UGradAssetManager& UGradAssetManager::Get()
{
	check(GEngine);

	// 우리는 AssetManager를 UEngine의 GEngine의 AssetManager의 Class를 오버라이드 했기 때문에, GEngine에 Asset Manager가 있음
	if (UGradAssetManager* Singleton = Cast<UGradAssetManager>(GEngine->AssetManager))
	{
		return *Singleton;
	}

	UE_LOG(LogGrad, Fatal, TEXT("invalid AssetManagerClassname in DefaultEngine.ini(project settings); it must be GradAssetManager"));
	// Fatal: 로깅이 비활성화된 경우에도 항상 치명적인 오류를 콘솔 및 로그 파일에 출력한 후 크래시를 발생시킵니다.

	// 위의 UE_LOG의 Fatal로 인해, Crash 나기 때문에 도달하지 않지만 컴파일을 위해 더미로 리턴
	return *NewObject<UGradAssetManager>();
}

PRAGMA_DISABLE_OPTIMIZATION // Development는 디버깅 관련해서 최소화 하는 데 이걸 넣으면 여길 제외한 곳만 Development한 매크로
void UGradAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FGradGameplayTags::InitializeNativeTags();

}
PRAGMA_ENABLE_OPTIMIZATION // Development는 디버깅 관련해서 최소화 하는 데 이걸 넣으면 여길 제외한 곳만 Development한 매크로

bool UGradAssetManager::ShouldLogAssetLoads()
{
	// FCommandLine - C++에서 int main(arg..) cmd에서 arg에 입력값을 넣는 것과 동일
	// 어디서 넣나? 프로젝트 속성 -> 디버깅에서 적용
	// FCommandLine::Get()을 통해 속성에서 넣은 입력값을 가지고 온다.
	const TCHAR* CommandLineContent = FCommandLine::Get();
	static bool bLogAssetLoads = FParse::Param(CommandLineContent, TEXT("LogAssetLoads"));
	return bLogAssetLoads;
}

UObject* UGradAssetManager::SynchronousLoadAsset(const FSoftObjectPath& AssetPath)
{
	// 해당 함수를 따로 만든 이유는 'synchronous load asset이 불필요하게 하는 것이 없는지 확인하기 위함' -> 프레임 드랍, 어디서 문제가 나는지 찾기 위함
	if (AssetPath.IsValid())
	{
		// FScopeLogTime을 확인해보자:
		TUniquePtr<FScopeLogTime> LogTimePtr;
		if (ShouldLogAssetLoads())
		{
			// 단순히 로깅하면서, 초단위로 로깅 진행
			LogTimePtr = MakeUnique<FScopeLogTime>(*FString::Printf(TEXT("synchronous loaded assets [%s]"), *AssetPath.ToString()), nullptr, FScopeLogTime::ScopeLog_Seconds);
		}

		// 여기서 두가지 분기:
		// 1. AssetManager가 있으면, AssetManager의 StreamableManager를 통해 정적 로딩
		// 2. 아니면, FSoftObjectPath를 통해 바로 정적 로딩
		if (UAssetManager::IsValid())
		{
			return UAssetManager::GetStreamableManager().LoadSynchronous(AssetPath); 
			// GetStreamableManager: 동기, 비동기 로딩을 관리하는 클래스
		}

		// if asset manager is not ready, use LoadObject(), TryLoad: 매우 느림
		// - 슥 보면, StaticLoadObject가 보인다: 
		// - 참고로, 항상 StaticLoadObject하기 전에 StaticFindObject를 통해 확인하고 실패하면 진짜 로딩함
		return AssetPath.TryLoad();
	}

	return nullptr;
}

void UGradAssetManager::AddLoadedAsset(const UObject* Asset)
{
	if (ensureAlways(Asset))
	{
		FScopeLock Lock(&SyncObject);
		LoadedAssets.Add(Asset);

		// 언리얼은 기본적으로 컨텐츠 작업할 때에는 멀티스레드에서 안전하게 동작한다.
		// 그런데 예외적인 상황이 존재하는데 바로 에디터가 켜지기 전 상황이다.즉, 멀티스레드 상황에서 안전하지 않다.
		// 에셋매니저는 에디터가 켜지기 전 환경이기 때문에 멀티스레드에서 안전한 보장이 없어서
		// 락을 사용한다.
	}
}