// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// window 기본 타입 Hide
#include "Windows/AllowWindowsPlatformTypes.h"
#include "Windows/prewindowsapi.h"

#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <iostream>

class ULevelUpGameInstance;

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


class LEVELUPDEMO_API ClientSocket : public FRunnable
{
public:
	ClientSocket(ULevelUpGameInstance* Inst);
	virtual ~ClientSocket();

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Exit() override;

	bool ConnectServer();

	SOCKET Socket;

	void Send(PacketType pt, void* packet);

private:
	FRunnableThread* Thread;
	ULevelUpGameInstance* gameInst;
};
