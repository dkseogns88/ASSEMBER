#pragma once
#include "Object.h"

class GameSession;
class Room;

class Player : public Object
{
public:
	Player();
	virtual ~Player();

	weak_ptr<GameSession>	session;
};