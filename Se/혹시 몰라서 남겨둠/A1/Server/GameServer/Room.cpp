#include "pch.h"
#include "Room.h"
#include "Player.h"
#include "GameSession.h"
#include "Monster.h"
#include "ObjectUtils.h"
#include "SequenceNode.h"
#include "ActionNode.h"
#include "SelectorNode.h"
#include "BehaviorTree.h"
#include "FindTargetNode.h"
#include "ConditionNode.h"
#include "CoroutineJob.h"
#include "PathFinder.h"
RoomRef GRoom = make_shared<Room>();

Room::Room()
{

}

Room::~Room()
{

}

void Room::InitializationRoom()
{
	MonsterRef monster = ObjectUtils::CreateMonster();
	monster->posInfo->set_x(-1900.f);
	monster->posInfo->set_y(-1900.f);
	monster->posInfo->set_z(Utils::GetRandom(100.f, 100.f));
	if (AddMonster(monster)) {

		monster->vector3D.x = monster->posInfo->x();
		monster->vector3D.y = monster->posInfo->y();
		monster->vector3D.z = monster->posInfo->z();

		/*ActionNode* wander = new ActionNode([monster]() {
			monster->MoveToRandom();
			return true;
			});

		ActionNode* chasePlayer = new ActionNode([monster]() {
			monster->ChasePlayer();
			return true;
			});

		SequenceNode* chaseSequence = new SequenceNode({ chasePlayer });

		SelectorNode* targetSelector = new SelectorNode({ chaseSequence });

		SequenceNode* noTargetSequence = new SequenceNode({ wander });


		FindTargetNode* behaviorTreeRoot = new FindTargetNode(
			monster, targetSelector, noTargetSequence
			);
		BehaviorTree* behaviorTree = new BehaviorTree(behaviorTreeRoot);
		monster->_behaviorTree = behaviorTree;*/
	}
}

void Room::UpdateMonsterAI()
{
	for (auto it = _monsters.begin(); it != _monsters.end(); ++it) {
		if (MonsterRef monster = dynamic_pointer_cast<Monster>((it->second))) {
			monster->executeBehavior();
		}
	}
	DoTimer(0, &Room::UpdateMonsterAI);

}

bool Room::HandleEnterPlayer(PlayerRef player)
{
	UpdateTick();

	bool success = AddPlayer(player);

	// 랜덤 위치
	player->posInfo->set_x(Utils::GetRandom(0.f, 500.f));
	player->posInfo->set_y(Utils::GetRandom(0.f, 500.f));
	player->posInfo->set_z(Utils::GetRandom(100.f, 100.f));
	player->posInfo->set_yaw(Utils::GetRandom(0.f, 500.f));


	/*if (_player1 == nullptr)
	{
		_player1 = player;

		_player1->posInfo->set_x(Utils::GetRandom(0.f, 500.f));
		_player1->posInfo->set_y(Utils::GetRandom(0.f, 500.f));
		_player1->posInfo->set_z(Utils::GetRandom(0.f, 500.f));
		_player1->posInfo->set_yaw(90.f);

		DoTimer(2000, &Room::UpdateMonsterAI);
	}
	else if (_player2 == nullptr)
	{
		_player2 = player;

		_player2->posInfo->set_x(Utils::GetRandom(0.f, 500.f));
		_player2->posInfo->set_y(Utils::GetRandom(0.f, 500.f));
		_player2->posInfo->set_z(Utils::GetRandom(0.f, 500.f));
		_player2->posInfo->set_yaw(90.f);
	}*/

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
		Broadcast(sendBuffer, player->objectInfo->object_id());
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
		Protocol::S_SPAWN_MONSTER spawnKkt;

		for (auto& item : _monsters)
		{
			Protocol::ObjectInfo* objectInfo = spawnKkt.add_monsters();
			objectInfo->CopyFrom(*item.second->objectInfo);
		}

	}

	// 입장한 플레이어 모두에게 기존 몬스터들을 알리자.
	{

		for (auto& item : _monsters)
		{
			Protocol::S_SPAWN_MONSTER spawnPkt;
			Protocol::S_NPCMOVE npcMovePkt;
			
			Protocol::ObjectInfo* objectInfo = spawnPkt.add_monsters();
			objectInfo->CopyFrom(*item.second->objectInfo);
			SendBufferRef sendBuffer = ClientPacketHandler::MakeSendBuffer(spawnPkt);

			if (auto session = player->session.lock()) {
				session->Send(sendBuffer);
				
				if (auto npc = dynamic_pointer_cast<Monster>(item.second))
				{
					{
						Protocol::PosInfo* info = npcMovePkt.mutable_info();

						auto dest = npc->GetDestByIdx(npc->idx);
						info->set_x(dest.x);
						info->set_y(dest.y);
						info->set_z(dest.z);
						info->set_object_id(npc->objectInfo->object_id());
						info->set_state(Protocol::MoveState::MOVE_STATE_RUN);
					}

					SendBufferRef sendBuffer = ClientPacketHandler::MakeSendBuffer(npcMovePkt);
					session->Send(sendBuffer);
				}
			}
		}
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

void Room::HandlePathFinding(Protocol::C_PATHFINDING pkt)
{
	auto arr = pkt.info();
	FVector start = {};
	FVector goal = {};

	for (int i = 0; i < arr.size(); i++)
	{
		if (i == 0) 
			start = { arr[i].x(),arr[i].y(),arr[i].z() };
		else
			goal = { arr[i].x(),arr[i].y(),arr[i].z() };
	}

	auto coroutine = CoroutineJob::CoroutineFunc(GPathFinder, start, goal);

	while (true)
	{
		if (GPathFinder->IsRead)
		{
			coroutine.resume();
			break;
		}
	}

	for (auto p : GPathFinder->path)
		cout << "(" << p.x << "," << p.y << "," << p.z << ") ->";
	cout << endl << endl;
}

void Room::HandleNpcMove(Protocol::C_NPCMOVE pkt)
{
	const uint64 objectId = pkt.info().object_id();

	MonsterRef npc = dynamic_pointer_cast<Monster>(_monsters[objectId]);
	auto info = npc->objectInfo->mutable_pos_info();
	info->CopyFrom(pkt.info());
	npc->posInfo->CopyFrom(pkt.info());
	npc->vector3D.x = npc->posInfo->x();
	npc->vector3D.y = npc->posInfo->y();
	npc->vector3D.z = npc->posInfo->z();

	npc->CheckIdx();

	if (!npc->isArrvied)
		npc->posInfo->set_state(Protocol::MoveState::MOVE_STATE_RUN);

	Protocol::S_NPCMOVE npcMovePkt;
	Protocol::PosInfo* sendInfo = npcMovePkt.mutable_info();

	auto dest = npc->GetDestByIdx(npc->idx);
	sendInfo->set_x(dest.x);
	sendInfo->set_y(dest.y);
	sendInfo->set_z(dest.z);
	sendInfo->set_object_id(npc->objectInfo->object_id());
	sendInfo->set_state(npc->posInfo->state());

	SendBufferRef sendBuffer = ClientPacketHandler::MakeSendBuffer(npcMovePkt);
	Broadcast(sendBuffer);
}

void Room::UpdateTick()
{
	cout << endl << "Update Room" << endl;

	for (auto n : _monsters)
	{
		auto npc = dynamic_pointer_cast<Monster>(n.second);
		npc->DoPathFinding(npc);
	}

	//DoTimer(5000, &Room::UpdateTick);

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