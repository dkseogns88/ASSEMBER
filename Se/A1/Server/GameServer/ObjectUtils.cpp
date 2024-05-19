#include "pch.h"
#include "ObjectUtils.h"
#include "Player.h"
#include "Monster.h"
#include "GameSession.h"

atomic<int64> ObjectUtils::s_idGenerator = 1;

PlayerRef ObjectUtils::CreatePlayer(GameSessionRef session)
{
	// ID »ý¼º±â
	const int64 newId = s_idGenerator.fetch_add(1);

	PlayerRef player = make_shared<Player>();
	player->objectInfo->set_object_id(newId);
	player->objectInfo->set_creature_type(Protocol::CreatureType::CREATURE_TYPE_PLAYER);
	player->objectInfo->set_player_type(Protocol::PlayerType::PLAYER_TYPE_SIDA);


	player->posInfo->set_object_id(newId);

	player->session = session;
	session->player.store(player);

	return player;
}

MonsterRef ObjectUtils::CreateMonster()
{
	const int64 newId = s_idGenerator.fetch_add(1);
	MonsterRef monster = make_shared<Monster>();
	monster->objectInfo->set_object_id(newId);
	monster->objectInfo->set_creature_type(Protocol::CreatureType::CREATURE_TYPE_MONSTER);
	monster->objectInfo->set_monster_type(Protocol::MonsterType::MONSTER_TYPE_TEST);

	monster->posInfo->set_object_id(newId);

	return monster;
}
