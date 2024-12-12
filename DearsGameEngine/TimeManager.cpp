#include "TimeManager.h"

bool TimeManager::Initialize()
{
	uint64_t countsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec); //�ý����� ī���� ��.
	mSecPerCount = 1.0 / static_cast<double>(countsPerSec);

	uint64_t curTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&curTime);	// ���� �ý��� �ð�
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
	QueryPerformanceCounter((LARGE_INTEGER*)&curTime);	// ���� �ý��� �ð�
	mCurTime = curTime;

	// ��Ÿ Ÿ�� ��� (���� �ð����� ���� �ð� ���̸� �ʷ� ��ȯ)
	mDeltaTime = (mCurTime - mPrevTime) * mSecPerCount;

	// ���� �ð��� ���� �ð����� ������Ʈ
	mPrevTime = mCurTime;

	// �ʹ� ū ��Ÿ Ÿ���� �ִ밪�� ���� �ʵ��� ���� (������ ����� ���� �� ����)
	if (mDeltaTime > mMaxDeltaTime)
	{
		mDeltaTime = mMaxDeltaTime;
	}

	// FPS ���
	mFrameCount++;
	mElapsedTime += mDeltaTime;

	if (mElapsedTime >= 1.0)
	{
		mCurrentFPS = static_cast<float>(mFrameCount) / mElapsedTime;
		mFrameCount = 0;
		mElapsedTime = 0.0;
	}

	// ������ ������ ���� Ÿ�̹� ����
	double targetFrameTime = 1.0 / mTargetFPS;
	if (mDeltaTime < targetFrameTime)
	{
		// ��� �ð� ��� (������ �ӵ��� �ʹ� ���� �� ����)
		double timeToWait = targetFrameTime - mDeltaTime;
		// CPU ���ϸ� ���̱� ���� ��� �ð� ���� Ÿ�̸Ӹ� ���� ��Ȯ�� �ð� ����
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