#pragma once
#include "JobQueue.h"

class Room : public JobQueue
{
public:
	Room();
	virtual ~Room();

	void InitializationRoom();
	void UpdateMonsterAI();


	bool HandleEnterPlayer(PlayerRef player);
	bool HandleLeavePlayer(PlayerRef player);

	void HandleMove(Protocol::C_MOVE pkt);			// ���⵵ ���߿� ���Ͷ� ������!
	void HandleJump(Protocol::C_JUMP pkt);

	void HandleZoom(Protocol::C_ZOOM pkt);

	void HandleSelect(Protocol::C_SELECT pkt);

	void HandlePathFinding(Protocol::C_PATHFINDING pkt);

	void HandleNpcMove(Protocol::C_NPCMOVE pkt);

	void UpdateTick();

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
	void Broadcast(SendBufferRef sendBuffer, uint64 exceptId = 0);
	
	// �÷��̾�
	PlayerRef _player1;
	PlayerRef _player2;

private:
	unordered_map<uint64, ObjectRef> _objects;			// ��� ������Ʈ ����
	unordered_map<uint64, ObjectRef> _players;			// �÷��̾ ����
	unordered_map<uint64, ObjectRef> _monsters;			// ���͸� ����
};

extern RoomRef GRoom;