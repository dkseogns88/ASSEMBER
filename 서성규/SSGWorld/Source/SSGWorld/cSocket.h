#pragma once


// winsock2 ����� ���� �Ʒ� �ڸ�Ʈ �߰�
#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <iostream>
#include <map>
#include "Runtime/Core/Public/Math/Vector.h"

using namespace std;

// �÷��̾� ����
class cCharacter {
public:
	cCharacter() {};
	~cCharacter() {};

	// ���� ���̵�
	int SessionId;
	// ��ġ
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

// ��Ŷ ����
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
	// ���� �ʱ�ȭ
	bool InitSocket();
	// ���� ���� ����
	void CloseSocket(SOCKET socketClose, bool bIsForce = false);
	// ������ �ּ������� ���ϰ� �����Ű�� ���� ��û�� �ޱ� ���� �� ������ �����
	bool BindAndListen(int nBindPort);
	// ip, port �� �ش��ϴ� ������ ����
	bool Connect(const char* pszIP, int nPort);
	// ��ġ �۽�
	void SendLocation(float x, float y, float z);
	// ��ġ ��û
	FVector RequestLocation();

	// �÷��̾� ��Ʈ�ѷ� ����
	void SetPlayerController(class ASSGPlayerController* pPlayerController);

	// �ʱ� ĳ���� ���
	void EnrollPlayer(cCharacter& info);

public:
	// Ŭ���̾�Ʈ ������ ������ �����ϱ� ���� ����
	// ���������� Ŭ���̾�Ʈ�� ���ӿ�û�� ��ٸ��� ����
	SOCKET	socket_;
	// ���� ����
	char	sz_socketbuf_[1024];

	class ASSGPlayerController* PlayerController;	// �÷��̾� ��Ʈ�ѷ� ����	
};