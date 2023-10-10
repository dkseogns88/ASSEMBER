#pragma once
#include "Allocator.h"


class MemoryPool;

/*-------------
	Memory
---------------*/

class Memory
{
	enum
	{
		// ~1024Byte까지 32Byte단위, ~2048Byte까지 128Byte단위, ~4096Byte까지 256Byte단위
		POOL_COUNT = (1024 / 32) + (1024 / 128) + (2048 / 256),
		MAX_ALLOC_SIZE = 4096
	};

public:
	Memory();
	~Memory();

	void*	Allocate(int32 size);
	void	Release(void* ptr);

private:
	vector<MemoryPool*> _pools;

	// 메모리 크기 <-> 메모리 풀
	// O(1) 빠르게 찾기 위한 테이블
	MemoryPool* _poolTable[MAX_ALLOC_SIZE + 1];
};


template<typename Type, typename... Args>
Type* xnew(Args&&... args)
{
	Type* memory = static_cast<Type*>(PoolAllocator::Alloc(sizeof(Type)));
	new(memory)Type(forward<Args>(args)...); // placement new
	// placement new(메모리를 미리 위에서 할당하고 메모리 위에다가 객체 생성자 호출)
	// 우리가 사용하던 일반 new는 메모리를 생성하자마자 바로 메모리 위에다가 생성자 호출
	// 그런데, 생성자를 어떤 거를 호출해야할까?
	// 생성자 인자가 하나, 둘 혹은 그 이상이 있을 경우에는 어떡하지?
	// 그런 상황에 대비해서 나온게 가변인자
	// 그런데, 인자가 왼값 인지 오른값인지 모르기 때문에
	// 보편참조로 넘겨준다!
	return memory;
}

template<typename Type>
void xdelete(Type* obj)
{
	obj->~Type();
	PoolAllocator::Release(obj);
}

template<typename Type, typename... Args>
shared_ptr<Type> MakeShared(Args&&... args)
{
	return shared_ptr<Type>{ xnew<Type>(forward<Args>(args)...), xdelete<Type> };
}