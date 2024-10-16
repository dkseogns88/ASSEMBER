#pragma once

#include "CoreMinimal.h"
#include "ServerPacketHandler.h"
#include "PacketSession.h"
#include "Protocol.pb.h"
#include "A1.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "A1NetworkManager.generated.h"

class AA1Character;

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
	void HandleSpawn(const Protocol::ObjectInfo& ObjectInfo, bool IsMine);
	void HandleSpawn(const Protocol::S_ENTER_GAME& EnterGamePkt);
	void HandleSpawn(const Protocol::S_SPAWN& SpawnPkt);

	void HandleDespawn(uint64 ObjectId);
	void HandleDespawn(const Protocol::S_DESPAWN& DespawnPkt);

	void HandleLeave(const Protocol::S_LEAVE_GAME& LeavePkt);

	void HandleMove(const Protocol::S_MOVE& MovePkt);

	void HandleState(const Protocol::S_STATE& StatePkt);
	void HandleAttack(const Protocol::S_ATTACK& attackPkt);


private:
	AA1Character* ValidationPlayer(int ObjectId);

	void SpawnPlayer(const Protocol::ObjectInfo& ObjectInfo, bool IsMine);

public:
	class FSocket* Socket;
	FString IpAddress = TEXT("127.0.0.1");
	int16 Port = 7777;

	TSharedPtr<class PacketSession> GameServerSession;

public:
	UPROPERTY()
	TMap<uint64, AA1Character*> Objects;

	UPROPERTY()
	TObjectPtr<AA1Character> MyPlayer;
};

template <typename T> 
void UA1NetworkManager::SendPacket(T packet) const
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	const SendBufferRef SendBuffer = ServerPacketHandler::MakeSendBuffer(packet);
	GameServerSession->SendPacket(SendBuffer);
}
