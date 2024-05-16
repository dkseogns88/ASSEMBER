#include "pch.h"
#include "Room.h"
#include "Player.h"
#include "GameSession.h"
#include "Monster.h"
#include "Physics.h"
#include "ObjectUtils.h"
#include <cmath>

RoomRef GRoom = make_shared<Room>();

Room::Room()
{
	/*{
		MonsterRef monster = ObjectUtils::CreateMonster();
		if (AddMonster(monster) == false)
			return;

		monster->posInfo->set_x(1510.f);
		monster->posInfo->set_y(2880.f);
		monster->posInfo->set_z(100.f);
		monster->posInfo->set_yaw(Utils::GetRandom(0.f, 500.f));
	}

	{
		MonsterRef monster = ObjectUtils::CreateMonster();
		if (AddMonster(monster) == false)
			return;

		monster->posInfo->set_x(1200.f);
		monster->posInfo->set_y(2880.f);
		monster->posInfo->set_z(100.f);
		monster->posInfo->set_yaw(Utils::GetRandom(0.f, 500.f));
	}

	{
		MonsterRef monster = ObjectUtils::CreateMonster();
		if (AddMonster(monster) == false)
			return;

		monster->posInfo->set_x(-2200.f);
		monster->posInfo->set_y(1210.f);
		monster->posInfo->set_z(140.f);
		monster->posInfo->set_yaw(Utils::GetRandom(0.f, 500.f));
	}

	{
		MonsterRef monster = ObjectUtils::CreateMonster();
		if (AddMonster(monster) == false)
			return;

		monster->posInfo->set_x(-2460.f);
		monster->posInfo->set_y(1210.f);
		monster->posInfo->set_z(-140.f);
		monster->posInfo->set_yaw(Utils::GetRandom(0.f, 500.f));
	}

	{
		MonsterRef monster = ObjectUtils::CreateMonster();
		if (AddMonster(monster) == false)
			return;

		monster->posInfo->set_x(-2740.f);
		monster->posInfo->set_y(1210.f);
		monster->posInfo->set_z(-140.f);
		monster->posInfo->set_yaw(Utils::GetRandom(0.f, 500.f));
	}

	{
		MonsterRef monster = ObjectUtils::CreateMonster();
		if (AddMonster(monster) == false)
			return;

		monster->posInfo->set_x(-4480.f);
		monster->posInfo->set_y(430.f);
		monster->posInfo->set_z(-140.f);
		monster->posInfo->set_yaw(Utils::GetRandom(0.f, 500.f));
	}

	{
		MonsterRef monster = ObjectUtils::CreateMonster();
		if (AddMonster(monster) == false)
			return;

		monster->posInfo->set_x(-4480.f);
		monster->posInfo->set_y(-790.f);
		monster->posInfo->set_z(-140.f);
		monster->posInfo->set_yaw(Utils::GetRandom(0.f, 500.f));
	}

	{
		MonsterRef monster = ObjectUtils::CreateMonster();
		if (AddMonster(monster) == false)
			return;

		monster->posInfo->set_x(-1570.f);
		monster->posInfo->set_y(-1020.f);
		monster->posInfo->set_z(120.f);
		monster->posInfo->set_yaw(Utils::GetRandom(0.f, 500.f));
	}

	{
		MonsterRef monster = ObjectUtils::CreateMonster();
		if (AddMonster(monster) == false)
			return;

		monster->posInfo->set_x(-230.f);
		monster->posInfo->set_y(-730.f);
		monster->posInfo->set_z(-120.f);
		monster->posInfo->set_yaw(Utils::GetRandom(0.f, 500.f));
	};*/
}

Room::~Room()
{

}

void Room::Init()
{
	{
		MonsterRef monster = ObjectUtils::CreateMonster();
		if (AddMonster(monster) == false)
			return;

		monster->posInfo->set_x(1510.f);
		monster->posInfo->set_y(2880.f);
		monster->posInfo->set_z(160.f);
		monster->posInfo->set_yaw(Utils::GetRandom(0.f, 500.f));
	}

	{
		MonsterRef monster = ObjectUtils::CreateMonster();
		if (AddMonster(monster) == false)
			return;

		monster->posInfo->set_x(1200.f);
		monster->posInfo->set_y(2880.f);
		monster->posInfo->set_z(160.f);
		monster->posInfo->set_yaw(Utils::GetRandom(0.f, 500.f));
	}

	{
		MonsterRef monster = ObjectUtils::CreateMonster();
		if (AddMonster(monster) == false)
			return;

		monster->posInfo->set_x(-2200.f);
		monster->posInfo->set_y(1210.f);
		monster->posInfo->set_z(-100.f);
		monster->posInfo->set_yaw(Utils::GetRandom(0.f, 500.f));
	}

	{
		MonsterRef monster = ObjectUtils::CreateMonster();
		if (AddMonster(monster) == false)
			return;

		monster->posInfo->set_x(-2460.f);
		monster->posInfo->set_y(1210.f);
		monster->posInfo->set_z(-100.f);
		monster->posInfo->set_yaw(Utils::GetRandom(0.f, 500.f));
	}

	{
		MonsterRef monster = ObjectUtils::CreateMonster();
		if (AddMonster(monster) == false)
			return;

		monster->posInfo->set_x(-2740.f);
		monster->posInfo->set_y(1210.f);
		monster->posInfo->set_z(-100.f);
		monster->posInfo->set_yaw(Utils::GetRandom(0.f, 500.f));
	}

	{
		MonsterRef monster = ObjectUtils::CreateMonster();
		if (AddMonster(monster) == false)
			return;

		monster->posInfo->set_x(-4480.f);
		monster->posInfo->set_y(430.f);
		monster->posInfo->set_z(-100.f);
		monster->posInfo->set_yaw(Utils::GetRandom(0.f, 500.f));
	}

	{
		MonsterRef monster = ObjectUtils::CreateMonster();
		if (AddMonster(monster) == false)
			return;

		monster->posInfo->set_x(-4480.f);
		monster->posInfo->set_y(-790.f);
		monster->posInfo->set_z(-100.f);
		monster->posInfo->set_yaw(Utils::GetRandom(0.f, 500.f));
	}

	{
		MonsterRef monster = ObjectUtils::CreateMonster();
		if (AddMonster(monster) == false)
			return;

		monster->posInfo->set_x(-1570.f);
		monster->posInfo->set_y(-1020.f);
		monster->posInfo->set_z(160.f);
		monster->posInfo->set_yaw(Utils::GetRandom(0.f, 500.f));
	}

	{
		MonsterRef monster = ObjectUtils::CreateMonster();
		if (AddMonster(monster) == false)
			return;

		monster->posInfo->set_x(-230.f);
		monster->posInfo->set_y(-730.f);
		monster->posInfo->set_z(-120.f);
		monster->posInfo->set_yaw(Utils::GetRandom(0.f, 500.f));
	};
}

bool Room::HandleEnterPlayer(PlayerRef player)
{
	bool success = AddPlayer(player);

	if (_player1 == nullptr)
	{
		_player1 = player;

		player->posInfo->set_x(1210.f);
		player->posInfo->set_y(100.f);
		player->posInfo->set_z(140.f);
		player->posInfo->set_yaw(Utils::GetRandom(0.f, 500.f));

		// 랜덤 위치
		/*player->posInfo->set_x(Utils::GetRandom(0.f, 300.f));
		player->posInfo->set_y(Utils::GetRandom(0.f, 400.f));
		player->posInfo->set_z(Utils::GetRandom(100.f, 100.f));
		player->posInfo->set_yaw(Utils::GetRandom(0.f, 500.f));*/
	}
	else if (_player2 == nullptr)
	{
		_player2 = player;
		player->posInfo->set_x(1480.f);
		player->posInfo->set_y(100.f);
		player->posInfo->set_z(140.f);
		player->posInfo->set_yaw(Utils::GetRandom(0.f, 500.f));

		// 랜덤 위치
		//player->posInfo->set_x(Utils::GetRandom(0.f, 300.f));
		//player->posInfo->set_y(Utils::GetRandom(0.f, 400.f));
		//player->posInfo->set_z(Utils::GetRandom(100.f, 100.f));
		//player->posInfo->set_yaw(Utils::GetRandom(0.f, 500.f));
	}
	
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

		Protocol::ObjectInfo* objectInfo = spawnPkt.add_players();
		objectInfo->CopyFrom(*player->objectInfo);

		SendBufferRef sendBuffer = ClientPacketHandler::MakeSendBuffer(spawnPkt);
		Broadcast(sendBuffer);
	}

	// 입장한 플레이어들을 새로운 플레이어에게 알려줘야 한다.
	{
		Protocol::S_SPAWN spawnPkt;

		for (auto& item : _players)
		{
			if (item.second->IsPlayer() == false)
				continue;

			Protocol::ObjectInfo* objectInfo = spawnPkt.add_players();
			objectInfo->CopyFrom(*item.second->objectInfo);
		}

		SendBufferRef sendBuffer = ClientPacketHandler::MakeSendBuffer(spawnPkt);
		if (auto session = player->session.lock())
			session->Send(sendBuffer);
	}

	// 입장한 플레이어 모두에게 기존 몬스터들을 알리자.
	{
		Protocol::S_SPAWN_MONSTER spawnPkt;

		for (auto& item : _monsters)
		{
			Protocol::ObjectInfo* objectInfo = spawnPkt.add_monsters();
			objectInfo->CopyFrom(*item.second->objectInfo);


		}

		SendBufferRef sendBuffer = ClientPacketHandler::MakeSendBuffer(spawnPkt);
		if (auto session = player->session.lock())
			session->Send(sendBuffer);
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

void Room::HandleJump(Protocol::C_JUMP pkt)
{
	const uint64 objectId = pkt.info().object_id();
	if (_objects.find(objectId) == _objects.end())
		return;

	PlayerRef player = dynamic_pointer_cast<Player>(_objects[objectId]);
	player->posInfo->CopyFrom(pkt.info());

	{
		Protocol::S_JUMP jumpPkt;
		{
			Protocol::PosInfo* info = jumpPkt.mutable_info();
			info->CopyFrom(pkt.info());
		}

		SendBufferRef sendBuffer = ClientPacketHandler::MakeSendBuffer(jumpPkt);
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

void Room::HandleHit(Protocol::C_HIT pkt)
{
	
	const uint64 objectId = pkt.object_id(); // 몬스터 id
	if (_monsters.find(objectId) == _monsters.end())
		return;

	{

		Protocol::S_HIT hitPkt;
		hitPkt.set_object_id(pkt.object_id());
		hitPkt.set_on_hit(pkt.on_hit());

		SendBufferRef sendBuffer = ClientPacketHandler::MakeSendBuffer(hitPkt);
		Broadcast(sendBuffer);
	}

}


void Room::HandleSelect(Protocol::C_SELECT pkt)
{
	auto& msg_pkt = pkt.msg();
	if (msg_pkt == "Rindty" || msg_pkt == "Sida")
	{
		Protocol::S_SELECT selectPkt;
		selectPkt.set_success(true);
		selectPkt.set_msg(msg_pkt);

		SendBufferRef sendBuffer = ClientPacketHandler::MakeSendBuffer(selectPkt);
		Broadcast(sendBuffer);
	}

}

void Room::MonserSpawn()
{

	MonsterRef monster = ObjectUtils::CreateMonster();
	if (AddMonster(monster) == false)
		return;

	monster->posInfo->set_x(Utils::GetRandom(100.f, 250.f));
	monster->posInfo->set_y(Utils::GetRandom(100.f, 250.f));
	monster->posInfo->set_z(Utils::GetRandom(150.f, 150.f));
	monster->posInfo->set_yaw(Utils::GetRandom(0.f, 500.f));



	Protocol::S_SPAWN_MONSTER spawnKkt;
	Protocol::ObjectInfo* objectInfo = spawnKkt.add_monsters();
	objectInfo->CopyFrom(*monster->objectInfo);

	SendBufferRef sendBuffer = ClientPacketHandler::MakeSendBuffer(spawnKkt);
	Broadcast(sendBuffer);

	{
		DoTimer(10'000, &Room::MonserSpawn);
	}

}

RoomRef Room::GetRoomRef()
{
	return static_pointer_cast<Room>(shared_from_this());
}

bool Room::AddObject(ObjectRef object)
{
	if (_objects.find(object->objectInfo->object_id()) != _objects.end())
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