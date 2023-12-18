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

enum PacketType {
	P_LOGIN,
	P_STATE,

	NONE,
};


struct PacketCordinate {
	int id;
	float x, y, z;
};

std::unordered_map<PacketType, size_t> g_packet_size{
	{P_STATE, sizeof(PacketCordinate)},
	{P_LOGIN, sizeof(PacketCordinate)},
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

void ProcessClient(SOCKET socket)
{
	while (1) {

		PacketType pt;

		PacketCordinate sendCordinate{};
		if (SOCKET_ERROR == recv(socket, reinterpret_cast<char*>(&pt), sizeof(pt), 0)) {
			cout << "Recv 오류! 오류 코드: " << WSAGetLastError() << endl;
			break;
		}

		switch (pt)
		{
		case P_LOGIN:
			PacketCordinate Login;
			if (SOCKET_ERROR == recv(socket, reinterpret_cast<char*>(&Login), sizeof(Login), 0)) {
				cout << "Recv 오류! 오류 코드: " << WSAGetLastError() << endl;
			}
			sendCordinate.id = Login.id;
			sendCordinate.x = Login.x;
			sendCordinate.y = Login.y;
			sendCordinate.z = Login.z;
			players.push_back(Player(Login.id, Login.x, Login.y, Login.z));
			break;
		case P_STATE:
			PacketCordinate cordinate;
			if (SOCKET_ERROR == recv(socket, reinterpret_cast<char*>(&cordinate), sizeof(cordinate), 0)) {
				cout << "Recv 오류! 오류 코드: " << WSAGetLastError() << endl;
			}
			for (Player& player : players) {
				if (player._id == cordinate.id) {
					player._x = cordinate.x;
					player._y = cordinate.y;
					player._z = cordinate.z;

					sendCordinate.id = cordinate.id;
					sendCordinate.x = cordinate.x;
					sendCordinate.y = cordinate.y;
					sendCordinate.z = cordinate.z;
					break;
				}
			}
			break;
		default:
			break;
		}

		for (SOCKET csocket : sockets) {
			struct sockaddr_in clientaddr;
			char addr[INET_ADDRSTRLEN];
			int addrlen = sizeof(clientaddr);
			switch (pt)
			{
			case P_LOGIN:
				Send(csocket, pt, &sendCordinate);
				break;
			case P_STATE:
				Send(csocket, pt, &sendCordinate);
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