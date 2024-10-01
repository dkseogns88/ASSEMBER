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


bool Room::HandleEnterPlayer(PlayerRef player)
{
	bool success = AddPlayer(player);
	
	player->posInfo->set_x(Utils::GetRandom(0.f, 500.f));
	player->posInfo->set_y(Utils::GetRandom(0.f, 500.f));
	player->posInfo->set_z(88.f);
	player->posInfo->set_yaw(Utils::GetRandom(0.f, 500.f));
	player->posInfo->set_move_state(Protocol::MoveState::MOVE_STATE_IDLE);

	player->state->set_locomotion_state(Protocol::LOCOMOTION_STATE_IDLE);
	player->state->set_overlay_state(Protocol::OVERLAY_STATE_NONE);
	player->state->set_aim(false);;

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

void Room::HandleSate(Protocol::C_STATE pkt)
{
	const uint64 objectId = pkt.state().object_id();
	if (_objects.find(objectId) == _objects.end())
		return;

	PlayerRef player = dynamic_pointer_cast<Player>(_objects[objectId]);
	player->state->CopyFrom(pkt.state());

	{
		Protocol::S_STATE statePkt;
		{
			Protocol::StateInfo* state = statePkt.mutable_state();
			state->CopyFrom(pkt.state());
		}

		SendBufferRef sendBuffer = ClientPacketHandler::MakeSendBuffer(statePkt);
		Broadcast(sendBuffer);
	}

}

void Room::HandleAttack(Protocol::C_ATTACK pkt)
{
	const uint64 objectId = pkt.info().attack_object_id();
	if (_objects.find(objectId) == _objects.end())
		return;

	cout << "공격 ID: " << pkt.info().attack_object_id() << endl;
	cout << "피격 ID: " << pkt.info().hit_object_id() << endl;
	cout << "공격 Damage: " << _objects[objectId]->statInfo->damage() << endl;

	float BeforeHp = _objects[pkt.info().hit_object_id()]->statInfo->hp();
	_objects[pkt.info().hit_object_id()]->statInfo->set_hp(BeforeHp - _objects[objectId]->statInfo->damage());
	
	
	cout << "피격전 HP: " << BeforeHp << endl;
	cout << "피격후 HP: " << _objects[pkt.info().hit_object_id()]->statInfo->hp() << endl;

	{
		Protocol::S_ATTACK attackPkt;
		{
			Protocol::AttackInfo* info = attackPkt.mutable_info();
			info->CopyFrom(pkt.info());
		}

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