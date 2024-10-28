#pragma once

#include "CoreMinimal.h"
#include "ServerPacketHandler.h"
#include "PacketSession.h"
#include "Protocol.pb.h"
#include "../GradGame.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "NetworkManager.generated.h"

class AGradCharacter;
class AGradNetCharacter;

UCLASS()
class GRADGAME_API UNetworkManager : public UGameInstanceSubsystem
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


public:
	void HandleSpawn(const Protocol::ObjectInfo& ObjectInfo, bool IsMine);
	void HandleSpawn(const Protocol::S_ENTER_GAME& EnterGamePkt);
	void HandleSpawn(const Protocol::S_SPAWN& SpawnPkt);

	void HandleDespawn(uint64 ObjectId);
	void HandleDespawn(const Protocol::S_DESPAWN& DespawnPkt);

	void HandleLeave(const Protocol::S_LEAVE_GAME& LeavePkt);

	void HandleMove(const Protocol::S_MOVE& MovePkt);

private:
	void SpawnPlayer(const Protocol::ObjectInfo& ObjectInfo, bool IsMine);




public:
	class FSocket* Socket;
	FString IpAddress = TEXT("127.0.0.1");
	int16 Port = 7777;

	TSharedPtr<class PacketSession> GameServerSession;

public:
	UPROPERTY()
	TObjectPtr<AGradCharacter> MyPlayer;

	UPROPERTY()
	TMap<uint64, TObjectPtr<ACharacter>> Objects;
};

template <typename T> 
void UNetworkManager::SendPacket(T packet) const
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	const SendBufferRef SendBuffer = ServerPacketHandler::MakeSendBuffer(packet);
	GameServerSession->SendPacket(SendBuffer);
}
