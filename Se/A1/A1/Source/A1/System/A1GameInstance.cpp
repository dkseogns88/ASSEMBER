#include "A1GameInstance.h"
#include "A1AssetManager.h"
#include "Sockets.h"
#include "Common/TcpSocketBuilder.h"

UA1GameInstance::UA1GameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

	ConstructorHelpers::FClassFinder<AA1OtherPlayer> FindOtherPlayerClass(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/Character/BP_OtherPlayer.BP_OtherPlayer_C'"));
	if (FindOtherPlayerClass.Succeeded())
	{
		OtherPlayerClass = FindOtherPlayerClass.Class;
	}

	ConstructorHelpers::FClassFinder<AA1Monster> FindMonsterClass(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/Character/BP_Monster.BP_Monster_C'"));
	if (FindMonsterClass.Succeeded())
	{
		MonsterClass = FindMonsterClass.Class;
	}

}

void UA1GameInstance::Init()
{
	Super::Init();

	UA1AssetManager::Initialize();
}

void UA1GameInstance::Shutdown()
{
	Super::Shutdown();

}