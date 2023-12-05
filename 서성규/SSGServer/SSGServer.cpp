#include <winsock2.h>
#include <ws2tcpip.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <unordered_map>

using namespace std;

#pragma comment(lib, "ws2_32")
#define SERVERPORT 7777
#define BUFSIZE    1024

mutex m;

static int gid = 0;
enum PacketType {
	P_LOGIN,
	P_STATE,

	NONE,
};

struct PacketLogin {
	int id;
};

struct PacketCordinate {
	int id;
	float x, y, z;
};

std::unordered_map<PacketType, size_t> g_packet_size{
	{P_LOGIN, sizeof(PacketLogin)},
	{P_STATE, sizeof(PacketCordinate)},
};

struct Player
{
	Player(int id) {
		_id = id;
	}

	Player(int id, float x, float y, float z) {
		_id = id, _x = x, _y = _y, z = _z;
	}

	int _id{};
	float _x{};
	float _y{};
	float _z{};
};

vector<Player> players;
vector<SOCKET> sockets;

void Send(SOCKET Socket, PacketType pt, void* packet)
{
	send(Socket, (const char*)&pt, sizeof(pt), 0);
	send(Socket, (const char*)packet, g_packet_size[pt], 0);
}

struct location {
	float x{};
	float y{};
	float z{};
};

void ProcessClient(SOCKET socket)
{
	while (1) {
		PacketType pt{NONE};
		int characterid{};
		if (SOCKET_ERROR == recv(socket, reinterpret_cast<char*>(&pt), sizeof(pt), 0)) {
			cout << "Recv 오류! 오류 코드: " << WSAGetLastError() << endl;
			break;
		}

		switch (pt)
		{
		case P_LOGIN:
			PacketLogin Login;
			if (SOCKET_ERROR == recv(socket, reinterpret_cast<char*>(&Login), sizeof(Login), 0)) {
				cout << "Recv 오류!" << endl;
			}
			break;
		case P_STATE:
			PacketCordinate cordinate;
			if (SOCKET_ERROR == recv(socket, reinterpret_cast<char*>(&cordinate), sizeof(cordinate), 0)) {
				cout << "Recv 오류!" << endl;
			}
			characterid = cordinate.id;
			players[cordinate.id]._x = cordinate.x;
			players[cordinate.id]._y = cordinate.y;
			players[cordinate.id]._z = cordinate.z;
			break;
		default:
			break;
		}

		if(pt == P_LOGIN){
			PacketLogin Login;
			Login.id = gid++;
			Send(socket, pt, &Login);
		}

		PacketCordinate cordinate;
		cordinate.id = characterid;
		cordinate.x = players[characterid]._x;
		cordinate.y = players[characterid]._y;
		cordinate.z = players[characterid]._z;

		cout << "id - " << cordinate.id << endl;
		cout << "x - " << cordinate.x << endl;
		cout << "y - " << cordinate.y << endl;
		cout << "z - " << cordinate.z << endl;

		for (SOCKET csocket : sockets) {
			struct sockaddr_in clientaddr;
			char addr[INET_ADDRSTRLEN];
			int addrlen = sizeof(clientaddr);
			switch (pt)
			{
			case P_STATE:
				Send(csocket, pt, &cordinate);
				break;
			case NONE:
				break;
			default:
				break;
			}
		}

	}
	closesocket(socket);
}

int main(int argc, char* argv[])
{
	int retval;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	SOCKET listen_sock = ::socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) {
		cout << "Socket Error!" << endl;
		return 0;
	}

	sockaddr_in serveraddr;
	::memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);

	retval = ::bind(listen_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));

	if (retval == INVALID_SOCKET) {
		cout << "bind Error!" << endl;
		return 0;
	}


	retval = ::listen(listen_sock, SOMAXCONN);
	if (retval == INVALID_SOCKET) {
		cout << "listen Error!" << endl;
		return 0;
	}

	SOCKET client_sock;
	sockaddr_in clientaddr;
	int addrlen = sizeof(clientaddr);


	while (1) {

		cout << "클라 accept 대기중...." << endl;
		client_sock = ::accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			cout << "Accept Socket Error!" << endl;
			return -1;
		}
		sockets.push_back(client_sock);
		players.push_back(Player(gid));
		char addr[INET_ADDRSTRLEN];

		inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
		printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
			addr, ntohs(clientaddr.sin_port));


		thread t(ProcessClient, ref(client_sock));
		t.detach();

	}
	::closesocket(listen_sock);
	::WSACleanup();

	return 0;

}