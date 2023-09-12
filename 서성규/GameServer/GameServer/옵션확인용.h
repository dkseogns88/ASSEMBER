#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>
#include "ThreadManager.h"

// ======================================================================================
// ������ ����� Ŀ�ο��� send�� recv ���۰� �����ȴ�
// ������ �츮�� ���ϴ´�� ������ �����ϴ�
// setsockopt: ���� ����
// getsockopt: ������ �Ÿ� Ȯ��
// ======================================================================================

#include <winsock2.h>
#include <mswsock.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

void HandleError(const char* cause) {
	int32 errCode = ::WSAGetLastError();
	cout << cause << " ErrorCode : " << errCode << endl;

}

int main()
{
	WSAData wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return 0;

	SOCKET serverSocket = ::socket(AF_INET, SOCK_DGRAM, 0);
	if (serverSocket == INVALID_SOCKET)
	{
		HandleError("Socket");
		return 0;
	}

	// �ɼ��� �ؼ��ϰ� ó���� ��ü?
	// ���� �ڵ� -> SOL_SOCKET
	// IPv4 -> IPPROTO_IP
	// TCP �������� -> IPPROTO_TCP
	// SO_KEEPALIVE <- �ֱ������� ���� ���� Ȯ�� ����(TCP������)
	// ������ �Ҹ� �ҹ����� ������ ���´ٸ�?
	// �ֱ������� TCP �������� ���� ���� Ȯ�� -> �D���� ������ ����
	bool enable = true; // Ȩ������
	::setsockopt(serverSocket, SOL_SOCKET, SO_KEEPALIVE, (char*)&enable, sizeof(enable));





	// ���� ����
	::WSACleanup();

}