#include "pch.h"
#include "ObjectUtils.h"
#include "Player.h"
#include "Monster.h"
#include "GameSession.h"

atomic<int64> ObjectUtils::s_idGenerator = 1;

PlayerRef ObjectUtils::CreatePlayer(GameSessionRef session)
{
	// ID ������
	const int64 newId = s_idGenerator.fetch_add(1);

	PlayerRef player = make_shared<Player>();
	player->objectInfo->set_object_id(newId);
	
	player->objectInfo->set_object_type(Protocol::ObjectType::OBJECT_TYPE_PLAYER);

	player->posInfo->set_object_id(newId);
	player->statInfo->set_object_id(newId);
	player->state->set_object_id(newId);

	player->session = session;
	session->player.store(player);

	return player;
}

MonsterRef ObjectUtils::CreateMonster()
{
	const int64 newId = s_idGenerator.fetch_add(1);

	MonsterRef monster = make_shared<Monster>();
	monster->objectInfo->set_object_id(newId);

	monster->objectInfo->set_object_type(Protocol::OBJECT_TYPE_MONSTER);

	monster->posInfo->set_object_id(newId);
	monster->statInfo->set_object_id(newId);
	monster->state->set_object_id(newId);

	return monster;
}