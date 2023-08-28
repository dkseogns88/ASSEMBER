#pragma once

// -------------------
//		1차 시도
// -------------------
/*
struct SListEntry
{
	SListEntry* next;
};

struct SListHeader
{
	SListEntry* next = nullptr;
};

void InitializeHead(SListHeader* header);
void PushEntrySList(SListHeader* header, SListEntry* entry);
SListEntry* PopEntrySList(SListHeader* header);
*/

// -------------------
//		2차 시도
// -------------------

//struct SListEntry
//{
//	SListEntry* next;
//};
//
//struct SListHeader
//{
//	SListEntry* next = nullptr;
//};
//
//void InitializeHead(SListHeader* header);
//void PushEntrySList(SListHeader* header, SListEntry* entry);
//SListEntry* PopEntrySList(SListHeader* header);

// -------------------
//		3차 시도
// -------------------

DECLSPEC_ALIGN(16) // 16바이트로 정렬되게 해줘(무조건!!!! 마소해서 하라고 함)
struct SListEntry
{
	SListEntry* next;
};

DECLSPEC_ALIGN(16)
struct SListHeader
{
	SListHeader()
	{
		alignment = 0;
		region = 0;
	}

	union
	{
		struct
		{
			uint64 alignment;
			uint64 region;
		} DUMMYSTRUCTNAME;

		struct
		{
			uint64 depth : 16; // 비트단위로 쪼개서 사용하는 것, 16비트 사용
			uint64 sequence : 48; // 48비트 사용
			
			// 위에 2개 합쳐서 alignment
			// -----------
			// 아래 2개 합쳐서 region

			uint64 reserved : 4; // 4비트 사용
			uint64 next : 60; // 60비트 사용
		} HeaderX64;
	};
};

void InitializeHead(SListHeader* header);
void PushEntrySList(SListHeader* header, SListEntry* entry);
SListEntry* PopEntrySList(SListHeader* header);