// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <iostream>

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
	Player() {};
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

struct location {
	float x{};
	float y{};
	float z{};
};

class SSG_API ClientSocket
{
public:
	ClientSocket();
	~ClientSocket();

	bool InitSocket();

	// ip, port 에 해당하는 서버로 접속
	bool ConnectServer();

	void Send(PacketType pt, void* packet);
	void LoginRecv(int& id);
	int StateRecv();

	SOCKET GetSock() { return Socket; }
private:
	SOCKET	Socket;
};
