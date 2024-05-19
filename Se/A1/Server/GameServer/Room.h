#pragma once
#include "JobQueue.h"

class Room : public JobQueue
{
public:
	Room();
	virtual ~Room();

	void InitializationRoom();
	void TestMonsterAI();


	bool HandleEnterPlayer(PlayerRef player);
	bool HandleLeavePlayer(PlayerRef player);

	void HandleMove(Protocol::C_MOVE pkt);			// ���⵵ ���߿� ���Ͷ� ������!
	void HandleJump(Protocol::C_JUMP pkt);

	void HandleZoom(Protocol::C_ZOOM pkt);

	void HandleSelect(Protocol::C_SELECT pkt);
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

private:
	unordered_map<uint64, ObjectRef> _objects;			// ��� ������Ʈ ����
	unordered_map<uint64, ObjectRef> _players;			// �÷��̾ ����
	unordered_map<uint64, ObjectRef> _monsters;			// ���͸� ����

	shared_ptr<Monster> _testMonster;
};

extern RoomRef GRoom;