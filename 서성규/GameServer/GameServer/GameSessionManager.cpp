#include "pch.h"
#include "GameSessionManager.h"
#include "GameSession.h"

GameSessionManager GSessionManager;

void GameSessionManager::Add(GameSessionRef session)
{
	WRITE_LOCK;
	_sessions.insert(session);
}

void GameSessionManager::Remove(GameSessionRef session)
{
	WRITE_LOCK;
	_sessions.erase(session);
}

void GameSessionManager::Broadcast(SendBufferRef sendBuffer)
{
	// 모든 유저에게 데이터를 전송
	// ex) 몬스터가 생기면 모든 유저에게 보이게
	
	WRITE_LOCK;
	for (GameSessionRef session : _sessions)
	{
		session->Send(sendBuffer);
	}
}