#include "A1GameInstance.h"
#include "A1AssetManager.h"
#include "Sockets.h"
#include "Common/TcpSocketBuilder.h"

UA1GameInstance::UA1GameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

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