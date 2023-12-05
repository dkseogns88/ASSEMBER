// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientSocket.h"

ClientSocket::ClientSocket()
{
	Socket = INVALID_SOCKET;
}

ClientSocket::~ClientSocket()
{
	WSACleanup();
}

bool ClientSocket::InitSocket()
{
	WSADATA wsaData;
	// 윈속 버전을 2.2로 초기화
	int nRet = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (nRet != 0) {
		std::cout << "Error : " << WSAGetLastError() << std::endl;
		return false;
	}

	// TCP 소켓 생성
	Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (Socket == INVALID_SOCKET) {
		std::cout << "Error : " << WSAGetLastError() << std::endl;
		return false;
	}

	std::cout << "socket initialize success." << std::endl;
	return true;
}


bool ClientSocket::ConnectServer()
{
	sockaddr_in serveraddr;
	::memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	::inet_pton(AF_INET, "127.0.0.1", &serveraddr.sin_addr);
	serveraddr.sin_port = htons(7777);

	int nRet;

	nRet = connect(Socket, (sockaddr*)&serveraddr, sizeof(sockaddr));

	if (nRet == SOCKET_ERROR)
	{
		return false;
	}
	else
	{
		TEXT("Socket Connect Success");
		return true;
	}
}

void ClientSocket::Send(PacketType pt, void* packet)
{
	send(Socket, (const char*)&pt, sizeof(pt), 0);
	send(Socket, (const char*)packet, g_packet_size[pt], 0);
}

void ClientSocket::LoginRecv(int& id)
{
	PacketType pt{};

	if (SOCKET_ERROR == recv(Socket, reinterpret_cast<char*>(&pt), sizeof(pt), 0)) {
		TEXT("Recv Fail");
	}
	switch (pt)
	{
	case P_LOGIN:
		PacketLogin Login;
		if (SOCKET_ERROR == recv(Socket, reinterpret_cast<char*>(&Login), sizeof(Login), 0)) {
			TEXT("Recv Fail");
		}
		id = Login.id;
		break;
	default:
		break;
	}

}

int ClientSocket::StateRecv()
{
	PacketType pt{};
	int id = -1;
	if (SOCKET_ERROR == recv(Socket, reinterpret_cast<char*>(&pt), sizeof(pt), 0)) {
		TEXT("Recv Fail");
	}
	switch (pt)
	{
	case P_STATE:
		PacketCordinate state;
		if (SOCKET_ERROR == recv(Socket, reinterpret_cast<char*>(&state), sizeof(state), 0)) {
			TEXT("Recv Fail");
		}
		id = state.id;
		break;
	default:
		break;
	}

	return id;
}

