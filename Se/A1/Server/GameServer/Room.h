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

	void HandleMove(Protocol::C_MOVE pkt);			// 여기도 나중에 몬스터랑 나누자!
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
	unordered_map<uint64, ObjectRef> _objects;			// 모든 오브젝트 관리
	unordered_map<uint64, ObjectRef> _players;			// 플레이어만 관리
	unordered_map<uint64, ObjectRef> _monsters;			// 몬스터만 관리

	shared_ptr<Monster> _testMonster;
};

extern RoomRef GRoom;