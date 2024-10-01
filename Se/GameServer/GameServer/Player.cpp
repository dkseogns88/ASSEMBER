#include "pch.h"
#include "Player.h"

Player::Player()
{
	_isPlayer = true;

	objectInfo->set_object_type(Protocol::OBJECT_TYPE_PLAYER);
}

Player::~Player()
{

}

