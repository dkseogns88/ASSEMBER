#pragma once

class Room;

class Object : public enable_shared_from_this<Object>
{
public:
	Object();
	virtual ~Object();

	bool IsPlayer() { return _isPlayer; }


public:
	Protocol::ObjectInfo* objectInfo;
	Protocol::PosInfo* posInfo;


public:
	atomic<weak_ptr<Room>> room;
	uint32 _hp = 100;

protected:
	bool _isPlayer = false;
};

