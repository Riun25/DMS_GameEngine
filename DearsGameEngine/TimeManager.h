#pragma once
#include "pch.h"

/// <summary>
/// ���� �������� �ð��� �����ϴ� �Ŵ��� Ŭ�����Դϴ�.
/// QueryPerformance�� ����� Windows ȯ�濡�� ����ϵ��� �ۼ��Ǿ����ϴ�.
/// ���� chrono ���̺귯���� ����ؼ� �÷��� Ȯ���� ����Ҽ� �ֽ��ϴ�.
/// 2024.04.22 _Doyo;
/// </summary>
class TimeManager
{
public:
	TimeManager() = default;
	~TimeManager() = default;

	bool Initialize();
	void Update();
	void Finalize();

	float TotalTime() const;
	float DeltaTime() const;
	float FixedDeltaTime() const;
	float FPS() const;

	void Reset();
	void Start();
	void Stop();

	// 30, 60, 120, 144
	void SetTargetFPS(const float _targetFPS);

	void SetFixedDeltaTime(const float _fDTime);
	void SetDeltaTime(const float _dTime);

	std::chrono::system_clock::time_point GetSystemTime();

	float GetTargetFPS() const { return mTargetFPS; }
private:
	double mSecPerCount = 0;
	double mDeltaTime = 0;
	double mFixedDeltaTime = 1.0 / 60.0;
	float mMaxDeltaTime = 0.1f;  // ��Ÿ Ÿ���� �ִ� 0.1�ʷ� ����

	uint64_t mBaseTime = 0;
	uint64_t mPausedTime = 0;
	uint64_t mStopTime = 0;
	uint64_t mPrevTime = 0;
	uint64_t mCurTime = 0;

	bool mStopped = false;
	float mTargetFPS = 144.f;	// ��ǥ fps
	float mCurrentFPS = 0.f;     // ���� FPS�� ����
	int mFrameCount = 0;
	double mElapsedTime = 0.0;
};
