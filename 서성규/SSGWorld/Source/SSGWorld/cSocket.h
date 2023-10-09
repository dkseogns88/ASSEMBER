#pragma once


// winsock2 사용을 위해 아래 코멘트 추가
#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <iostream>
#include <map>
#include "Runtime/Core/Public/Math/Vector.h"

using namespace std;

// 플레이어 정보
class cCharacter {
public:
	cCharacter() {};
	~cCharacter() {};

	// 세션 아이디
	int SessionId;
	// 위치
	float X;
	float Y;
	float Z;


	friend ostream& operator<<(ostream& stream, cCharacter& info)
	{
		stream << info.SessionId << endl;
		stream << info.X << endl;
		stream << info.Y << endl;
		stream << info.Z << endl;


		return stream;
	}

	friend istream& operator>>(istream& stream, cCharacter& info)
	{
		stream >> info.SessionId;
		stream >> info.X;
		stream >> info.Y;
		stream >> info.Z;

		return stream;
	}
};

class cCharactersInfo
{
public:
	cCharactersInfo() {};
	~cCharactersInfo() {};

	map<int, cCharacter> players;

	friend ostream& operator<<(ostream& stream, cCharactersInfo& info)
	{
		stream << info.players.size() << endl;
		for (auto& kvp : info.players)
		{
			stream << kvp.first << endl;
			stream << kvp.second << endl;
		}

		return stream;
	}

	friend istream& operator>>(istream& stream, cCharactersInfo& info)
	{
		int nPlayers = 0;
		int SessionId = 0;
		cCharacter Player;
		info.players.clear();

		stream >> nPlayers;
		for (int i = 0; i < nPlayers; i++)
		{
			stream >> SessionId;
			stream >> Player;
			info.players[SessionId] = Player;
		}

		return stream;
	}
};

// 패킷 정보
enum EPacketType
{
	LOGIN,
	ENROLL_PLAYER,
	SEND_PLAYER,
	RECV_PLAYER,
	LOGOUT_PLAYER,
	HIT_PLAYER,
	DAMAGED_PLAYER,
	CHAT,
	ENTER_NEW_PLAYER,
	SIGNUP,
	HIT_MONSTER,
	SYNC_MONSTER,
	SPAWN_MONSTER,
	DESTROY_MONSTER
};

class cSocket
{
public:
	cSocket();
	virtual ~cSocket();
	// 소켓 초기화
	bool InitSocket();
	// 소켓 연결 종료
	void CloseSocket(SOCKET socketClose, bool bIsForce = false);
	// 서버의 주소정보를 소켓과 연결시키고 접속 요청을 받기 위해 그 소켓을 등록함
	bool BindAndListen(int nBindPort);
	// ip, port 에 해당하는 서버로 접속
	bool Connect(const char* pszIP, int nPort);
	// 위치 송신
	void SendLocation(float x, float y, float z);
	// 위치 요청
	FVector RequestLocation();

	// 플레이어 컨트롤러 세팅
	void SetPlayerController(class ASSGPlayerController* pPlayerController);

	// 초기 캐릭터 등록
	void EnrollPlayer(cCharacter& info);

public:
	// 클라이언트 에서는 서버에 연결하기 위한 소켓
	// 서버에서는 클라이언트의 접속요청을 기다리는 소켓
	SOCKET	socket_;
	// 소켓 버퍼
	char	sz_socketbuf_[1024];

	class ASSGPlayerController* PlayerController;	// 플레이어 컨트롤러 정보	
};