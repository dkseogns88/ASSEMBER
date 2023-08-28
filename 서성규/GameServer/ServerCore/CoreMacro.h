#pragma once

#define OUT // OUT은 값이 바뀔 수 있는 힌트를 준다

/* ----------------------
		  Lock
---------------------- */

#define USE_MANY_LOCKS(count)	Lock _locks[count];
#define USE_LOCK				USE_MANY_LOCKS(1)
#define READ_LOCK_IDX(idx)		ReadLockGuard readLockGuard_##idx(_locks[idx], typeid(this).name()); // ##idx가 저절로 1로 바뀜
#define READ_LOCK				READ_LOCK_IDX(0)
#define WRITE_LOCK_IDX(idx)		WriteLockGuard writeLockGuard_##idx(_locks[idx], typeid(this).name()); // ##idx가 저절로 1로 바뀜
#define WRITE_LOCK				WRITE_LOCK_IDX(0)



/* ----------------------
		  Crash
---------------------- */

// 우리가 인위적으로 CRASH를 내려고 만든 것이다
// - 그 이유는 컴파일러가 알아서 잡아주는 부분이 생기기 때문에
#define CRASH(cause)						\
{											\
	uint32* crash = nullptr;				\
	__analysis_assume(crash != nullptr);	\
	*crash = 0xDEADBEFF;					\
}

// 조건부 Crash
#define ASSERT_CRASH(expr)					\
{											\
	if(!(expr))								\
	{										\
		CRASH("ASSERT_CRASH");				\
		__analysis_assume(expr);			\
	}										\
}