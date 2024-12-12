#include "TimeManager.h"

bool TimeManager::Initialize()
{
	uint64_t countsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec); //시스템의 카운터 빈도.
	mSecPerCount = 1.0 / static_cast<double>(countsPerSec);

	uint64_t curTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&curTime);	// 현재 시스템 시간
	mPrevTime = curTime;

	return true;
}

void TimeManager::Update()
{
	if (mStopped)
	{
		mDeltaTime = 0.f;
		return;
	}

	uint64_t curTime{};
	QueryPerformanceCounter((LARGE_INTEGER*)&curTime);	// 현재 시스템 시간
	mCurTime = curTime;

	// 델타 타임 계산 (현재 시간에서 이전 시간 차이를 초로 변환)
	mDeltaTime = (mCurTime - mPrevTime) * mSecPerCount;

	// 이전 시간을 현재 시간으로 업데이트
	mPrevTime = mCurTime;

	// 너무 큰 델타 타임은 최대값을 넘지 않도록 제한 (프레임 드랍이 심할 때 보정)
	if (mDeltaTime > mMaxDeltaTime)
	{
		mDeltaTime = mMaxDeltaTime;
	}

	// FPS 계산
	mFrameCount++;
	mElapsedTime += mDeltaTime;

	if (mElapsedTime >= 1.0)
	{
		mCurrentFPS = static_cast<float>(mFrameCount) / mElapsedTime;
		mFrameCount = 0;
		mElapsedTime = 0.0;
	}

	// 프레임 제한을 위한 타이밍 조정
	double targetFrameTime = 1.0 / mTargetFPS;
	if (mDeltaTime < targetFrameTime)
	{
		// 대기 시간 계산 (프레임 속도가 너무 빠를 때 제한)
		double timeToWait = targetFrameTime - mDeltaTime;
		// CPU 부하를 줄이기 위해 대기 시간 동안 타이머를 통해 정확한 시간 측정
		uint64_t endWaitTime = curTime + (timeToWait / mSecPerCount);
		while (curTime < endWaitTime)
		{
			QueryPerformanceCounter((LARGE_INTEGER*)&curTime);
		}
		mCurTime = curTime;
		mDeltaTime = targetFrameTime;
	}
}

void TimeManager::Finalize()
{
	Stop();
}

float TimeManager::TotalTime() const
{
	auto totalTime = mStopped ? ((mStopTime - mPausedTime) - mBaseTime) * mSecPerCount
		: ((mCurTime - mPausedTime) - mBaseTime) * mSecPerCount;
	return totalTime;
}

float TimeManager::DeltaTime() const
{
	return static_cast<float>(mDeltaTime);
}

float TimeManager::FixedDeltaTime() const
{
	return static_cast<float>(mFixedDeltaTime);
}

float TimeManager::FPS() const
{
// 	static int frameCount = 0;
// 	static float elapsedTime = 0.f;
// 	static float currentFPS = 0;
// 
// 	frameCount++;
// 	elapsedTime += m_deltaTime;
// 
// 	if (elapsedTime >= 1.f)
// 	{
// 		currentFPS = static_cast<float>(frameCount) / elapsedTime;
// 		frameCount = 0;
// 		elapsedTime = 0.f;
// 	}
// 
// 	return currentFPS;
	return (1.f / mDeltaTime);
// 	return m_currentFPS;
}

void TimeManager::Reset()
{
	LARGE_INTEGER currTime;
	QueryPerformanceCounter(&currTime);

	mBaseTime = currTime.QuadPart;
	mPrevTime = currTime.QuadPart;
	mStopTime = 0;
	mStopped = false;
	mPausedTime = 0;
}

void TimeManager::Start()
{
	uint64_t startTime{};
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

	if (mStopped)
	{
		mPausedTime += startTime - mStopTime;
		mPrevTime = startTime;
		mStopTime = 0;
		mStopped = false;
	}
}

void TimeManager::Stop()
{
	if (!mStopped)
	{
		uint64_t curTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&curTime);
		mStopTime = curTime;
		mStopped = true;
	}
}

void TimeManager::SetTargetFPS(const float _targetFPS)
{
	mTargetFPS = _targetFPS;
	mFixedDeltaTime = 1.0 / static_cast<double>(_targetFPS);
}

void TimeManager::SetFixedDeltaTime(const float _fDTime)
{
	mFixedDeltaTime = _fDTime;
}

void TimeManager::SetDeltaTime(const float _dTime)
{
	mDeltaTime = _dTime;
}

std::chrono::system_clock::time_point TimeManager::GetSystemTime()
{
	return std::chrono::system_clock::now();
}