#include "pch.h"
#include "TimeManager.h"

void TimeManager::Init()
{
	QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&_frequency));
	// - 시스템 부팅 시 고정되는 고성능 타이머의 주파수를 반환한다.

	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&_prevCount));
	// - 고성능 타이머(<1us)의 현재 카운터 값을 반환한다.

}

void TimeManager::Update()
{
	uint64 currentCount;

	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currentCount));

	_deltaTime = (currentCount - _prevCount) / static_cast<float>(_frequency);
	_prevCount = currentCount;

	_frameCount++;
	_frameTime += _deltaTime;

	if (_frameTime >= 1.f) {
		_fps = static_cast<uint32>(_frameCount / _frameTime);

		_frameTime = 0.f;
		_frameCount = 0;
	}
}
