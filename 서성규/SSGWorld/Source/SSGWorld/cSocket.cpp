#include "cSocket.h"
#include <iostream>
#include <string>
#include "SSGPlayerController.h"
#include <sstream>

cSocket::cSocket()
{
	socket_ = INVALID_SOCKET;
	ZeroMemory(sz_socketbuf_, 1024);
}

cSocket::~cSocket()
{
	// winsock ����� ����
	WSACleanup();
}

bool cSocket::InitSocket()
{
	WSADATA wsaData;
	// ���� ������ 2.2�� �ʱ�ȭ
	int nRet = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (nRet != 0) {
		std::cout << "Error : " << WSAGetLastError() << std::endl;
		return false;
	}

	// TCP ���� ����
	socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socket_ == INVALID_SOCKET) {
		std::cout << "Error : " << WSAGetLastError() << std::endl;
		return false;
	}

	std::cout << "socket initialize success." << std::endl;
	return true;
}

void cSocket::CloseSocket(SOCKET socketClose, bool bIsForce)
{
	struct linger stLinger = { 0, 0 };	// SO_DONTLINGER �� ����
	// bIsFoce �� true �̸� SO_LINGER, timeout = 0 ���� �����Ͽ�
	// ���� ���� ��Ų��. ���� : ������ �ս��� ���� �� ����
	if (bIsForce == true) {
		stLinger.l_onoff = true;
		// socketClose ������ ������ �ۼ����� ��� �ߴ�
		shutdown(socketClose, SD_BOTH);
		// ���� �ɼ��� ����
		setsockopt(socketClose, SOL_SOCKET, SO_LINGER, (char*)&stLinger, sizeof(stLinger));
		// ���� ������ ����
		closesocket(socketClose);

		socketClose = INVALID_SOCKET;
	}
}

bool cSocket::BindAndListen(int nBindPort)
{
	SOCKADDR_IN		stServerAddr;
	stServerAddr.sin_family = AF_INET;
	// ���� ��Ʈ�� ����
	stServerAddr.sin_port = htons(nBindPort);
	stServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	// ������ ������ ���� �ּ� ������ ������ ����
	int nRet = ::bind(socket_, (SOCKADDR*)&stServerAddr, sizeof(SOCKADDR_IN));
	if (nRet != 0) {
		std::cout << "Error : " << WSAGetLastError() << std::endl;
		return false;
	}

	// ���� ��û�� �޾Ƶ��̱� ���� ������ ����ϰ� ���� ���ť�� 5�� ����
	nRet = listen(socket_, 5);
	if (nRet != 0) {
		std::cout << "Error : " << WSAGetLastError() << std::endl;
		return false;
	}

	std::cout << "server enroll success." << std::endl;
	return true;
}

bool cSocket::Connect(const char* pszIP, int nPort)
{
	// ������ ���� ������ ������ ����ü
	SOCKADDR_IN stServerAddr;

	stServerAddr.sin_family = AF_INET;
	// ������ ���� ��Ʈ �� IP
	stServerAddr.sin_port = htons(nPort);
	stServerAddr.sin_addr.s_addr = inet_addr(pszIP);

	int nRet = connect(socket_, (sockaddr*)&stServerAddr, sizeof(sockaddr));
	if (nRet == SOCKET_ERROR) {
		std::cout << "Error : " << WSAGetLastError() << std::endl;
		return false;
	}

	std::cout << "Connection success..." << std::endl;

	return true;
}

struct location
{
	location(){}

	location(float x, float y, float z) {
		_x = x, _y = y, _z = z;
	}

	float _x{};
	float _y{};
	float _z{};
};


void cSocket::SendLocation(float x, float y, float z)
{

	location lct(x, y, z);
	// UE_LOG(LogClass, Log, TEXT("Character Location x - %f, y - %f, z - %f\n"), lct._x, lct._y, lct._z);

	int nSendLen = send(socket_, (char*)&lct, sizeof(location), 0);

	if (nSendLen == -1) {
		std::cout << "Error : " << WSAGetLastError() << std::endl;
	}

	int nRecvLen = ::recv(socket_, (char*)&lct, sizeof(lct), 0);

	if (nRecvLen == -1) {
		std::cout << "Error : " << WSAGetLastError() << std::endl;
	}
}


FVector cSocket::RequestLocation()
{
	char		szOutMsg[1024];
	location loc;
	strcpy_s(szOutMsg, "req_location");

	int nSendLen = send(socket_, szOutMsg, strlen(szOutMsg), 0);

	if (nSendLen == -1) {
		std::cout << "Error : " << WSAGetLastError() << std::endl;
		return FVector();
	}

	int nRecvLen = recv(socket_, (char*)&loc, 1024, 0);

	FVector result(loc._x, loc._y, loc._z);

	return result;
}

void cSocket::SetPlayerController(ASSGPlayerController* pPlayerController)
{
	// �÷��̾� ��Ʈ�ѷ� ����
	if (pPlayerController)
	{
		PlayerController = pPlayerController;
	}
}

void cSocket::EnrollPlayer(cCharacter& info)
{


}
