#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>
#include "ThreadManager.h"

// ======================================================================================
// 소켓을 만들면 커널에서 send와 recv 버퍼가 생성된다
// 소켓을 우리가 원하는대로 설정이 가능하다
// setsockopt: 소켓 설정
// getsockopt: 설정한 거를 확인
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

	// 옵션을 해석하고 처리할 주체?
	// 소켓 코드 -> SOL_SOCKET
	// IPv4 -> IPPROTO_IP
	// TCP 프로토콜 -> IPPROTO_TCP
	// SO_KEEPALIVE <- 주기적으로 연결 상태 확인 여부(TCP에서만)
	// 상대방이 소리 소문없이 연결을 끊는다면?
	// 주기적으로 TCP 프로토콜 연결 상태 확인 -> 귾어진 연결을 감지
	bool enable = true; // 홈페이지
	::setsockopt(serverSocket, SOL_SOCKET, SO_KEEPALIVE, (char*)&enable, sizeof(enable));





	// 윈속 종료
	::WSACleanup();

}