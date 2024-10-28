// Copyright Epic Games, Inc. All Rights Reserved.

#include "GradGame.h"
#include "GradLogChannels.h"
#include "Modules/ModuleManager.h"

class FGradGame : public FDefaultGameModuleImpl
{
	virtual void StartupModule() override
	{
		FDefaultGameModuleImpl::StartupModule();
	}
	virtual void ShutdownModule() override
	{
		FDefaultGameModuleImpl::ShutdownModule();
	}
};

IMPLEMENT_PRIMARY_GAME_MODULE(FGradGame , GradGame, "GradGame" );

/*----------------
	SendBuffer
-----------------*/

SendBuffer::SendBuffer(int32 bufferSize)
{
	_buffer.SetNum(bufferSize);
}

SendBuffer::~SendBuffer()
{
}

void SendBuffer::CopyData(void* data, int32 len)
{
	::memcpy(_buffer.GetData(), data, len);
	_writeSize = len;
}

void SendBuffer::Close(uint32 writeSize)
{
	_writeSize = writeSize;
}