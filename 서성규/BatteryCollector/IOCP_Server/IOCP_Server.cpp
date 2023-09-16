// iocp-sample.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include "pch.h"
#include "IOCompletionPort.h"

int main()
{
	IOCompletionPort iocp_server;
	if (iocp_server.Initialize())
	{
		iocp_server.StartServer();
	}
	return 0;
}

