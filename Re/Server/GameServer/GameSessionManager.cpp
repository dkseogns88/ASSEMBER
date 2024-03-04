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
	// 전송하고자 하는 패킷을 1000개의 Session에서 각자의 wsaBuf에 복사하여 Send하는 것 보다 
	// SendBuffer를 하나 만들고 그 SendBuffer의 스마트 포인터를 건네주도록 하여 복사비용을 줄이는 것이 중요한 목적이다.
	// 결국 복사비용 때문에 스마트 포인터를 사용하게 되어 Reference Counting을 사용

	WRITE_LOCK;
	for (GameSessionRef session : _sessions)
	{
		session->Send(sendBuffer);
	}
}