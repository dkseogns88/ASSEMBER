// Fill out your copyright notice in the Description page of Project Settings.

#include "ClientSocket.h"
#include "LevelUpGameInstance.h"

ClientSocket::ClientSocket(ULevelUpGameInstance* Inst)
	: gameInst(Inst)
{
	if (ConnectServer()) {
		Thread = FRunnableThread::Create(this, TEXT("Network Thread"));
		UE_LOG(LogTemp, Warning, TEXT("Thread has been initialized"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Thread has been initialized fail!!"));
	}
}

ClientSocket::~ClientSocket()
{
	if (Thread)
	{
		Thread->WaitForCompletion();
		Thread->Kill();
		delete Thread;
	}
}

bool ClientSocket::Init()
{
	return true;	
}

uint32 ClientSocket::Run()
{
	while (1) {
		FActorInfo NewActorInfo;
		bool isNewPlayer = true;

		PacketType pt{};
		if (SOCKET_ERROR == recv(Socket, reinterpret_cast<char*>(&pt), sizeof(pt), 0)) {
			return 0;
		}

		switch (pt)
		{
		case P_LOGIN:
			PacketCordinate Login;
			if (SOCKET_ERROR == recv(Socket, reinterpret_cast<char*>(&Login), sizeof(Login), 0)) {
				TEXT("Recv State Fail");
			}
			if (gameInst) {
				NewActorInfo.active = true;
				NewActorInfo.player_id = Login.id;
				NewActorInfo.x = Login.x;
				NewActorInfo.y = Login.y;
				NewActorInfo.z = Login.z;
				for (FActorInfo& actor_info : gameInst->ACTOR_LIST) {
					if (actor_info.player_id == NewActorInfo.player_id) {
						isNewPlayer = false;
					}
				}
				if (isNewPlayer) {
					while(!gameInst->isNewPlayer)
						gameInst->isNewPlayer = true;
					
					gameInst->ACTOR_LIST.Add(NewActorInfo);
					gameInst->isNewPlayerInfo = NewActorInfo;
				}
			}
			break;
		case P_STATE:
			PacketCordinate state;
			if (SOCKET_ERROR == recv(Socket, reinterpret_cast<char*>(&state), sizeof(state), 0)) {
				TEXT("Recv State Fail");
			}
			NewActorInfo.active = true;
			NewActorInfo.player_id = state.id;
			NewActorInfo.x = state.x;
			NewActorInfo.y = state.y;
			NewActorInfo.z = state.z;

			for (FActorInfo& actor_info : gameInst->ACTOR_LIST) {
				if (actor_info.player_id == state.id) {
					actor_info.x = state.x;
					actor_info.y = state.y;
					actor_info.z = state.z;
					isNewPlayer = false;
				}
			}
			if (isNewPlayer) {
				while (!gameInst->isNewPlayer)
					gameInst->isNewPlayer = true;

				gameInst->ACTOR_LIST.Add(NewActorInfo);
				gameInst->isNewPlayerInfo = NewActorInfo;
			}
			break;
		}
	}
	return 0;
}

void ClientSocket::Exit()
{
	if (Socket)
	{
		closesocket(Socket);
		WSACleanup();
	}
}

bool ClientSocket::ConnectServer()
{
	WSADATA wsaData;

	int nRet = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (nRet != 0) {
		return false;
	}

	Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (Socket == INVALID_SOCKET) {
		return false;
	}


	sockaddr_in serveraddr;
	::memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	::inet_pton(AF_INET, "127.0.0.1", &serveraddr.sin_addr);
	serveraddr.sin_port = htons(7777);

	nRet = connect(Socket, (sockaddr*)&serveraddr, sizeof(sockaddr));

	if (nRet == SOCKET_ERROR)
	{
		return false;
	}
	
	return true;

}

void ClientSocket::Send(PacketType pt, void* packet)
{
	send(Socket, (const char*)&pt, sizeof(pt), 0);
	send(Socket, (const char*)packet, g_packet_size[pt], 0);
}
