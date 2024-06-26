#include "pch.h"
#include "Room.h"
#include "Player.h"
#include "Monster.h"
#include "GameSession.h"
#include "ObjectUtils.h"

RoomRef GRoom = make_shared<Room>();

Room::Room()
{

}

Room::~Room()
{

}

bool Room::InitializationRoom()
{
	{
		MonsterRef monster = ObjectUtils::CreateMonster();

		// TODO: 몬스터 타입 정의
		monster->objectInfo->set_monster_type(Protocol::MONSTER_TYPE_FANATIC);
		monster->posInfo->set_x(Utils::GetRandom(0.f, 500.f));
		monster->posInfo->set_y(Utils::GetRandom(0.f, 500.f));
		monster->posInfo->set_z(40.f);
		monster->posInfo->set_state(Protocol::MOVE_STATE_IDLE);

		// TODO: 몬스터 스탯 정의
		monster->statInfo->set_hp(400);
		monster->statInfo->set_max_hp(400);
		monster->statInfo->set_damage(50);;

		if (!AddMonster(monster)) return false;
	}

	{
		MonsterRef monster = ObjectUtils::CreateMonster();

		// TODO: 몬스터 타입 정의
		monster->objectInfo->set_monster_type(Protocol::MONSTER_TYPE_MONK);
		monster->posInfo->set_x(Utils::GetRandom(0.f, 500.f));
		monster->posInfo->set_y(Utils::GetRandom(0.f, 500.f));
		monster->posInfo->set_z(40.f);
		monster->posInfo->set_state(Protocol::MOVE_STATE_IDLE);

		// TODO: 몬스터 스탯 정의
		monster->statInfo->set_hp(400);
		monster->statInfo->set_max_hp(400);
		monster->statInfo->set_damage(50);;

		if (!AddMonster(monster)) return false;

	}

	cout << "Room Initial\n";
	return true;
}


bool Room::HandleEnterPlayer(PlayerRef player)
{
	bool success = AddPlayer(player);

	//player->posInfo->set_x(1360.f);
	//player->posInfo->set_y(-70.f);
	//player->posInfo->set_z(40.f);
	
	player->posInfo->set_x(Utils::GetRandom(0.f, 500.f));
	player->posInfo->set_y(Utils::GetRandom(0.f, 500.f));
	player->posInfo->set_z(40.f);
	player->posInfo->set_yaw(Utils::GetRandom(0.f, 500.f));
	player->posInfo->set_state(Protocol::MoveState::MOVE_STATE_IDLE);

	// 입장 사실을 들어온 플레이어에게 알린다.
	{
		Protocol::S_ENTER_GAME enterGamePkt;
		enterGamePkt.set_success(success);

		Protocol::ObjectInfo* objectInfo = new Protocol::ObjectInfo();
		objectInfo->CopyFrom(*player->objectInfo);
		enterGamePkt.set_allocated_player(objectInfo);

		SendBufferRef sendBuffer = ClientPacketHandler::MakeSendBuffer(enterGamePkt);
		if (auto session = player->session.lock())
			session->Send(sendBuffer);
	}


	// 입장 사실을 다른 플레이어에게 알린다.
	{
		Protocol::S_SPAWN spawnPkt;

		Protocol::ObjectInfo* objectInfo = spawnPkt.add_objects();
		objectInfo->CopyFrom(*player->objectInfo);

		// TODO: Sector 관리 
		SendBufferRef sendBuffer = ClientPacketHandler::MakeSendBuffer(spawnPkt);
		Broadcast(sendBuffer, player->objectInfo->object_id());
	}

	// 입장한 플레이어들을 새로운 플레이어에게 알려줘야 한다.
	{
		Protocol::S_SPAWN spawnPkt;

		for (auto& item : _players)
		{
			if (item.second->IsPlayer() == false)
				continue;

			Protocol::ObjectInfo* objectInfo = spawnPkt.add_objects();
			objectInfo->CopyFrom(*item.second->objectInfo);
		}

		SendBufferRef sendBuffer = ClientPacketHandler::MakeSendBuffer(spawnPkt);
		if (auto session = player->session.lock())
			session->Send(sendBuffer);
	}

	// 입장한 플레이어 모두에게 기존 몬스터들을 알리자.
	{
		/*Protocol::S_SPAWN spawnPkt;

		for (auto& item : _monsters)
		{
			Protocol::ObjectInfo* objectInfo = spawnPkt.add_objects();
			objectInfo->CopyFrom(*item.second->objectInfo);
		}

		SendBufferRef sendBuffer = ClientPacketHandler::MakeSendBuffer(spawnPkt);
		if (auto session = player->session.lock())
			session->Send(sendBuffer);*/
	}

	return success;
}

bool Room::HandleLeavePlayer(PlayerRef player)
{
	if (player == nullptr)
		return false;


	const uint64 objectId = player->objectInfo->object_id();
	bool success = RemovePlayer(objectId);

	// 퇴장 사실을 퇴장하는 플레이어에게 알린다.
	{
		Protocol::S_LEAVE_GAME leaveGamePkt;
		SendBufferRef sendBuffer = ClientPacketHandler::MakeSendBuffer(leaveGamePkt);
		if (auto session = player->session.lock())
			session->Send(sendBuffer);
	}

	// 퇴장 사실을 모두에게 알린다.
	{
		Protocol::S_DESPAWN despawnPkt;
		despawnPkt.add_object_ids(objectId);

		SendBufferRef sendBuffer = ClientPacketHandler::MakeSendBuffer(despawnPkt);
		Broadcast(sendBuffer, objectId);

		if (auto session = player->session.lock())
			session->Send(sendBuffer);
	}


	return true;

}

void Room::HandleMove(Protocol::C_MOVE pkt)
{
	const uint64 objectId = pkt.info().object_id();
	if (_objects.find(objectId) == _objects.end())
		return;

	PlayerRef player = dynamic_pointer_cast<Player>(_objects[objectId]);
	player->posInfo->CopyFrom(pkt.info());

	{
		Protocol::S_MOVE movePkt;
		{
			Protocol::PosInfo* info = movePkt.mutable_info();
			info->CopyFrom(pkt.info());
		}

		SendBufferRef sendBuffer = ClientPacketHandler::MakeSendBuffer(movePkt);
		Broadcast(sendBuffer);
	}
}

void Room::HandleZoom(Protocol::C_ZOOM pkt)
{
	const uint64 objectId = pkt.info().object_id();
	if (_objects.find(objectId) == _objects.end())
		return;

	PlayerRef player = dynamic_pointer_cast<Player>(_objects[objectId]);

	{
		Protocol::S_ZOOM zoomPkt;
		{
			Protocol::ZoomInfo* info = zoomPkt.mutable_info();
			info->CopyFrom(pkt.info());
		}

		SendBufferRef sendBuffer = ClientPacketHandler::MakeSendBuffer(zoomPkt);
		Broadcast(sendBuffer);
	}

}

void Room::HandleAttack(Protocol::C_ATTACK pkt)
{
	const uint64 attackId = pkt.info().attack_object_id();
	const uint64 hitId = pkt.info().hit_object_id();

	if (_objects.find(attackId) == _objects.end())
		return;
	if (_objects.find(hitId) == _objects.end())
		return;

	ObjectRef attackObject = _objects[attackId];
	ObjectRef HitObject = _objects[hitId];

	// TODO: 스킬 데미지로 처리해야 함
	const uint32 damage = attackObject->statInfo->damage();
	uint32 hp = HitObject->statInfo->hp();
	hp -= damage;

	cout << "공격한 ID: " << attackId << ",  피격당한 ID: " << hitId << endl;
	cout << "HP: " << hp << endl << endl;

	if (hp <= 0)
	{
		Protocol::S_DESPAWN despawnPkt;
		despawnPkt.add_object_ids(hitId);

		SendBufferRef sendBuffer = ClientPacketHandler::MakeSendBuffer(despawnPkt);
		Broadcast(sendBuffer);

		if (HitObject->objectInfo->object_type() == Protocol::ObjectType::OBJECT_TYPE_PLAYER)
		{
			// TODO: 플레이어는 추후 관리!
			cout << "플레이어 죽었어요~\n\n";

		}
		else if (HitObject->objectInfo->object_type() == Protocol::ObjectType::OBJECT_TYPE_PLAYER)
		{
			// TODO: 몬스터를 제거해서 하거나 다시 HP를 reset해서 동작시키거나
			cout << "몬스터 사망\n\n";
			RemoveMonster(hitId);
		}
		return;
	}

	if (pkt.info().attack_type() ==  Protocol::AttackType::ATTACK_TYPE_BASIC)
	{
		HitObject->statInfo->set_hp(hp);
		Protocol::S_ATTACK attackPkt;
		Protocol::AttackInfo* attack_info = attackPkt.mutable_attack_info();
		Protocol::StatInfo* stat_info = attackPkt.mutable_stat_info();

		attack_info->CopyFrom(pkt.info());
		stat_info->CopyFrom(*(HitObject->statInfo));

		SendBufferRef sendBuffer = ClientPacketHandler::MakeSendBuffer(attackPkt);
		Broadcast(sendBuffer);


	}
	else if (pkt.info().attack_type() == Protocol::AttackType::ATTACK_TYPE_SKILL)
	{
		HitObject->statInfo->set_hp(hp);

		Protocol::S_ATTACK attackPkt;
		Protocol::AttackInfo* attack_info = attackPkt.mutable_attack_info();
		Protocol::StatInfo* stat_info = attackPkt.mutable_stat_info();

		attack_info->CopyFrom(pkt.info());
		stat_info->CopyFrom(*(HitObject->statInfo));

		SendBufferRef sendBuffer = ClientPacketHandler::MakeSendBuffer(attackPkt);
		Broadcast(sendBuffer);
	}

}


RoomRef Room::GetRoomRef()
{
	return static_pointer_cast<Room>(shared_from_this());
}

bool Room::AddObject(ObjectRef object)
{
	if(_objects.find(object->objectInfo->object_id()) != _objects.end())
		return false;

	_objects.insert(make_pair(object->objectInfo->object_id(), object));

	object->room.store(GetRoomRef());

	return true;
}

bool Room::AddPlayer(ObjectRef object)
{
	if (AddObject(object) == false)
		return false;

	if (_players.find(object->objectInfo->object_id()) != _players.end())
		return false;
	_players.insert(make_pair(object->objectInfo->object_id(), object));


	return true;
}

bool Room::AddMonster(ObjectRef object)
{
	if (AddObject(object) == false)
		return false;

	if (_monsters.find(object->objectInfo->object_id()) != _monsters.end())
		return false;
	_monsters.insert(make_pair(object->objectInfo->object_id(), object));
	
	return true;
}

bool Room::RemoveObject(uint64 objectId)
{
	if (_objects.find(objectId) == _objects.end())
		return false;

	ObjectRef object = _objects[objectId];
	object->room.store(weak_ptr<Room>());

	_objects.erase(objectId);

	return true;
}

bool Room::RemovePlayer(uint64 objectId)
{
	if (RemoveObject(objectId) == false)
		return false;

	if (_players.find(objectId) == _players.end())
		return false;
	
	ObjectRef object = _players[objectId];
	object->room.store(weak_ptr<Room>());

	_players.erase(objectId);

	return true;
}

bool Room::RemoveMonster(uint64 objectId)
{
	if (RemoveObject(objectId) == false)
		return false;

	if (_monsters.find(objectId) == _monsters.end())
		return false;

	ObjectRef object = _monsters[objectId];
	object->room.store(weak_ptr<Room>());

	_monsters.erase(objectId);

	return true;
}

void Room::Broadcast(SendBufferRef sendBuffer, uint64 exceptId)
{
	for (auto& item : _players)
	{
		PlayerRef player = dynamic_pointer_cast<Player>(item.second);
		if (player == nullptr)
			break;

		if (player->objectInfo->object_id() == exceptId)
			continue;

		if (GameSessionRef session = player->session.lock())
			session->Send(sendBuffer);
	}
}