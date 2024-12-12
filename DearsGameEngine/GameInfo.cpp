#include "GameInfo.h"
#include "TimeManager.h"
#include <windows.h>
#include <dxgi1_4.h>
#include <psapi.h>

GameInfo::GameInfo(TimeManager* _pTimeManager)
	: m_pTimeManager(_pTimeManager)
{
	SYSTEM_INFO sysInfo;
	FILETIME ftime, fsys, fuser;

	GetSystemInfo(&sysInfo);
	mNumProcessors = sysInfo.dwNumberOfProcessors;

	GetSystemTimeAsFileTime(&ftime);
	memcpy(&mLastCPU, &ftime, sizeof(FILETIME));

	mSelf = GetCurrentProcess();
	GetProcessTimes(mSelf, &ftime, &ftime, &fsys, &fuser);
	memcpy(&mLastSysCPU, &fsys, sizeof(FILETIME));
	memcpy(&mLastUserCPU, &fuser, sizeof(FILETIME));
}

GameInfo::~GameInfo()
{

}

float GameInfo::GetFPS() const
{
	return m_pTimeManager->FPS();
}

float GameInfo::GetFrameTime() const
{
	return m_pTimeManager->DeltaTime();
}

float GameInfo::GetCPUInfo()
{
	FILETIME ftime, fsys, fuser;
	ULARGE_INTEGER now, sys, user;
	double percent;

	GetSystemTimeAsFileTime(&ftime);
	memcpy(&now, &ftime, sizeof(FILETIME));

	GetProcessTimes(mSelf, &ftime, &ftime, &fsys, &fuser);
	memcpy(&sys, &fsys, sizeof(FILETIME));
	memcpy(&user, &fuser, sizeof(FILETIME));

	percent = (sys.QuadPart - mLastSysCPU.QuadPart) +
		(user.QuadPart - mLastUserCPU.QuadPart);
	percent /= (now.QuadPart - mLastCPU.QuadPart);
	percent /= mNumProcessors;
	mLastCPU = now;
	mLastUserCPU = user;
	mLastSysCPU = sys;

	return percent * 100;
}

double GameInfo::GetVRAMInfo() const
{
	IDXGIAdapter3* adapter{};
	DXGI_QUERY_VIDEO_MEMORY_INFO info = {};
	adapter->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &info);

	// info.CurrentUsage�� ����Ͽ� ���� VRAM ��뷮(byte ����) Ȯ�� ����
	return static_cast<double>(info.CurrentUsage) / (1024 * 1024);
}

unsigned int GameInfo::GetRAMInfo() const
{
	PROCESS_MEMORY_COUNTERS_EX pmc{};
	GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
	return static_cast<unsigned int>(pmc.WorkingSetSize); // ����Ʈ ������ ��� �޸�
}

