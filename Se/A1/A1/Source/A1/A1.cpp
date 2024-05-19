// Copyright Epic Games, Inc. All Rights Reserved.

#include "A1.h"
#include "Modules/ModuleManager.h"
#include "A1LogChannels.h"

class FA1Module : public FDefaultGameModuleImpl
{
	virtual void StartupModule() override
	{
		UE_LOG(LogA1, Log, TEXT("A1Module Start"));
	}
	
	virtual void ShutdownModule() override
	{

	}

};

IMPLEMENT_PRIMARY_GAME_MODULE(FA1Module, A1, "A1" );

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
