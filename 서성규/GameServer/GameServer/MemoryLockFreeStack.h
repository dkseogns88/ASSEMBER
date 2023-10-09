#pragma once

// -------------------
//		1�� �õ�
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
//		2�� �õ�
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
//		3�� �õ�
// -------------------

DECLSPEC_ALIGN(16) // 16����Ʈ�� ���ĵǰ� ����(������!!!! �����ؼ� �϶�� ��)
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
			uint64 depth : 16; // ��Ʈ������ �ɰ��� ����ϴ� ��, 16��Ʈ ���
			uint64 sequence : 48; // 48��Ʈ ���
			
			// ���� 2�� ���ļ� alignment
			// -----------
			// �Ʒ� 2�� ���ļ� region

			uint64 reserved : 4; // 4��Ʈ ���
			uint64 next : 60; // 60��Ʈ ���
		} HeaderX64;
	};
};

void InitializeHead(SListHeader* header);
void PushEntrySList(SListHeader* header, SListEntry* entry);
SListEntry* PopEntrySList(SListHeader* header);