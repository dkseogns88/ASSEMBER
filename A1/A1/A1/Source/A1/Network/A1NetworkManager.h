#pragma once

#include "CoreMinimal.h"
#include "ServerPacketHandler.h"
#include "PacketSession.h"
#include "Protocol.pb.h"
#include "A1.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "A1NetworkManager.generated.h"

class ABaseChar;

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

	void HandleDespawn(uint64 ObjectId);
	void HandleDespawn(const Protocol::S_DESPAWN& DespawnPkt);

	void HandleLeave(const Protocol::S_LEAVE_GAME& LeavePkt);

	void HandleMove(const Protocol::S_MOVE& MovePkt);

	void HandleZoom(const Protocol::S_ZOOM& ZoomPkt);


private:
	ABaseChar* ValidationPlayer(int ObjectId);

public:
	class FSocket* Socket;
	FString IpAddress = TEXT("127.0.0.1");
	int16 Port = 7777;

	TSharedPtr<class PacketSession> GameServerSession;
public:
	UPROPERTY()
	TMap<uint64, ABaseChar*> Players;

	UPROPERTY()
	TObjectPtr<ABaseChar> MyPlayer;
};

template <typename T> 
void UA1NetworkManager::SendPacket(T packet) const
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	const SendBufferRef SendBuffer = ServerPacketHandler::MakeSendBuffer(packet);
	GameServerSession->SendPacket(SendBuffer);
}