#include "pch.h"
#include "Object.h"

Object::Object()
{
	objectInfo = new Protocol::ObjectInfo();
	posInfo = new Protocol::PosInfo();
	statInfo = new Protocol::StatInfo();

	objectInfo->set_allocated_pos_info(posInfo);
	objectInfo->set_allocated_stat_info(statInfo);
}

Object::~Object()
{
	delete objectInfo;
}
