// dllmain.cpp : DLL 애플리케이션의 진입점을 정의합니다.
#include "pch.h"
#include "SoundSystem.h"

SoundSystem* g_pSoundSystem = nullptr;
std::mutex g_mutex;

extern "C" SOUNDSYSTEM_API SoundSystem* GetSoundSystem()
{
	std::lock_guard<std::mutex> lock(g_mutex);
	if (!g_pSoundSystem)
	{
		g_pSoundSystem = new SoundSystem();
		if (!g_pSoundSystem->Initialize())
		{
			delete g_pSoundSystem;
			g_pSoundSystem = nullptr;
		}
	}
	return g_pSoundSystem;
}

extern "C" SOUNDSYSTEM_API void ReleaseSoundSystem()
{
	std::lock_guard<std::mutex> lock(g_mutex);
	if (g_pSoundSystem)
	{
		g_pSoundSystem->Finalize();
		delete g_pSoundSystem;
		g_pSoundSystem = nullptr;
	}
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			DisableThreadLibraryCalls(hModule);
			break;
		case DLL_PROCESS_DETACH:
			ReleaseSoundSystem();
			break;
	}
	return TRUE;
}

