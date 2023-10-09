#include <winsock2.h>
#include <ws2tcpip.h>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

#pragma comment(lib, "ws2_32")
#define SERVERPORT 7777
#define BUFSIZE    1024

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

	char buf[BUFSIZE];
	int len;

	struct location
	{
		location() {}

		location(float x, float y, float z) {
			_x = x, _y = _y, z = _z;
		}

		float _x{};
		float _y{};
		float _z{};
	};

	static int numbers = 0;

	while (1) {

		cout << "클라 accept 대기중...." << endl;
		client_sock = ::accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			cout << "Accept Socket Error!" << endl;
			break;
		}
		else {
			
			cout << "\nClient Accept!!" << endl;
			cout << "client number: " << numbers << endl;
		}

		char addr[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
		printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
			addr, ntohs(clientaddr.sin_port));


		while (1) {

			location lct;

			retval = ::recv(client_sock, (char*)&lct, sizeof(lct), 0);
			if (retval == SOCKET_ERROR) {
				cout << "recv Error!" << endl;
				break;
			}
			else if (retval == 0) {
				cout << "캐릭터 제자리" << endl;
			}

			cout << "=============================================\n";
			printf_s("캐릭터 수신 위치 X = %f, Y = %f, Z = %f\n", lct._x, lct._y, lct._z);

			retval = ::send(client_sock, (char*)&lct, sizeof(location), 0);
			if (retval == SOCKET_ERROR) {
				cout << "recv Error!" << endl;
				break;
			}
			else if (retval == 0) {
				cout << "캐릭터 제자리" << endl;
			}

			printf_s("캐릭터 송신 위치 X = %f, Y = %f, Z = %f\n", lct._x, lct._y, lct._z);
			cout << "=============================================\n";
		}



		::closesocket(client_sock);
		printf("\n[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n\n",
			addr, ntohs(clientaddr.sin_port));

	}

	::closesocket(listen_sock);
	::WSACleanup();

	return 0;

}