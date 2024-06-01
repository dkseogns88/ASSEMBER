#pragma once
#include "JobQueue.h"

class Room : public JobQueue
{
public:
	Room();
	virtual ~Room();

	void InitializationRoom();
	void UpdateMonsterAI();

	// Ŭ�󿡰Կ��� �Ѿ�� ��Ŷ�� ó��
	bool HandleEnterPlayer(PlayerRef player);
	bool HandleLeavePlayer(PlayerRef player);

	void HandleMove(Protocol::C_MOVE pkt);			// ���⵵ ���߿� ���Ͷ� ������!
	void HandleJump(Protocol::C_JUMP pkt);

	void HandleZoom(Protocol::C_ZOOM pkt);
	
	void HandleHit(Protocol::C_HIT pkt);

	void HandleSelect(Protocol::C_SELECT pkt);
	void HandleTelePort(Protocol::C_TELEPORT pkt);

	// ���� ����
	void MonserSpawn();

public:

	RoomRef GetRoomRef();

private:
	bool AddObject(ObjectRef object);
	bool AddPlayer(ObjectRef object);
	bool AddMonster(ObjectRef object);

	bool RemoveObject(uint64 objectId);
	bool RemovePlayer(uint64 objectId);
	bool RemoveMonster(uint64 objectId);

public:
	// �÷��̾�
	PlayerRef _player1;
	PlayerRef _player2;
	void Broadcast(SendBufferRef sendBuffer, uint64 exceptId = 0);

private:
	unordered_map<uint64, ObjectRef> _objects;			// ��� ������Ʈ ����
	unordered_map<uint64, ObjectRef> _players;			// �÷��̾ ����
	unordered_map<uint64, ObjectRef> _monsters;			// ���͸� ����


};

extern RoomRef GRoom;