#pragma once

#include "CoreMinimal.h"
#include "ServerPacketHandler.h"
#include "PacketSession.h"
#include "Protocol.pb.h"
#include "A1.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "A1NetworkManager.generated.h"

class AA1Character;
class AA1Monster;

UCLASS()
class A1_API UA1NetworkManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void ConnectToGameServer();

	UFUNCTION(BlueprintCallable)
	void DisconnectFromGameServer();

	UFUNCTION(BlueprintCallable)
	void HandleRecvPackets();

	void SendPacket(SendBufferRef SendBuffer);

	template<typename T>
	void SendPacket(T packet) const;

public:
	void HandleSpawn(const Protocol::ObjectInfo& PlayerInfo, bool IsMine);
	void HandleSpawn(const Protocol::S_ENTER_GAME& EnterGamePkt);
	void HandleSpawn(const Protocol::S_SPAWN& SpawnPkt);

	void HandleMonsterSpawn(const Protocol::S_SPAWN_MONSTER& SpawnPkt);
	void HandleMonsterSpawn(const Protocol::ObjectInfo& MonsterInfo);

	void HandleDespawn(uint64 ObjectId);
	void HandleDespawn(const Protocol::S_DESPAWN& DespawnPkt);

	void HandleMove(const Protocol::S_MOVE& MovePkt);

	void HandleJump(const Protocol::S_JUMP& JumpPkt);

	void HandleZoom(const Protocol::S_ZOOM& ZoomPkt);

	void HandleNpcMove(const Protocol::S_NPCMOVE& MovePkt);

	void HandlePathFinding(const Protocol::S_PATHFINDING& pathPkt);
	void TestFunc();



private:
	AA1Character* ValidationPlayer(int ObjectId);
	AA1Character* ValidationMonster(int ObjectId);
public:
	class FSocket* Socket;
	FString IpAddress = TEXT("127.0.0.1");
	int16 Port = 7777;

	TSharedPtr<class PacketSession> GameServerSession;

public:
	UPROPERTY()
	TMap<uint64, AA1Character*> Players;	// 다른 클라이언트들에 캐릭터

	UPROPERTY()
	TMap<uint64, AA1Character*> Monsters;		// 몬스터들 관리

	UPROPERTY()
	TObjectPtr<AA1Character> MyPlayer;		// 이건 내가 컨트롤하는 플레이어
};

template <typename T> 
void UA1NetworkManager::SendPacket(T packet) const
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	const SendBufferRef SendBuffer = ServerPacketHandler::MakeSendBuffer(packet);
	GameServerSession->SendPacket(SendBuffer);
}
