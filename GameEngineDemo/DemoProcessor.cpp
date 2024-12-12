#include "DemoProcessor.h"
#include <memory>
#include "../DearsGameEngine/DearsGameEngineAPI.h"
#include "TestScene.h"
#include "TestWorld.h"

DemoProcessor::DemoProcessor(HINSTANCE _hInstance, const WCHAR* _szTitle, int _iconResourceId)
	:BaseWindowsProcessor(_hInstance, _szTitle, _iconResourceId)
	, mpGameEngine(nullptr)
{
}

bool DemoProcessor::Initialize()
{
	if (!__super::Initialize())
	{
		return false;
	}

	mpGameEngine = new GameEngine(mHWnd, mScreenWidth, mScreenHeight);
	mpGameEngine->Initialize();

	// Create World Test
	mpWorld = std::make_shared<TestWorld>(mpGameEngine->mRegistry,mpGameEngine->GetEventManager(), mpGameEngine->GetInputManager());
	mpGameEngine->GetWorldManager()->AddWorld(mpWorld);

	mpScene1 = std::make_shared<TestScene>(mpGameEngine->mRegistry, "TestScene"
		, mpGameEngine->GetEventManager(), mpGameEngine->GetRenderManager()
		, mpGameEngine->GetPhysicsManager(), mpGameEngine->GetInputManager()
		, mpGameEngine->GetWorldManager(), mpGameEngine->GetUIManager()
		, mpGameEngine->GetEntityManager(), mpGameEngine->GetResourceManager()
	,mpGameEngine->GetSoundManager());

	mpScene2 = std::make_shared<TestScene>(mpGameEngine->mRegistry, "TestScene"
		, mpGameEngine->GetEventManager(), mpGameEngine->GetRenderManager()
		, mpGameEngine->GetPhysicsManager(), mpGameEngine->GetInputManager()
		, mpGameEngine->GetWorldManager(), mpGameEngine->GetUIManager()
		, mpGameEngine->GetEntityManager(), mpGameEngine->GetResourceManager()
		, mpGameEngine->GetSoundManager());

	mpGameEngine->GetWorldManager()->GetCurrentWorld()->AddScene(mpScene1);
	mpGameEngine->GetWorldManager()->GetCurrentWorld()->AddScene(mpScene2);

	mpGameEngine->GetSoundManager()->LoadBGM("../../Resources/Sound/Snd_bgm_Battle.wav", "Snd_bgm_Battle");
	mpGameEngine->GetSoundManager()->PlayBGM("Snd_bgm_Battle");
	mpGameEngine->GetWorldManager()->GetCurrentWorld()->SetScene(mpScene1->GetUID());
	return true;
}

void DemoProcessor::Run()
{
	while (true)
	{
		if (PeekMessage(&mMsg, NULL, 0, 0, PM_REMOVE))
		{
			if (mMsg.message == WM_QUIT)
			{
				break;
			}

			if (!TranslateAccelerator(mMsg.hwnd, mHAccelTable, &mMsg))
			{
				TranslateMessage(&mMsg);
				DispatchMessage(&mMsg);
			}
		}
		else
		{
			if (mpGameEngine->GetWorldManager()->ShouldQuit())
			{
				Finalize();
				PostQuitMessage(0);
				break;
			}
			Update();
		}
	}
}

void DemoProcessor::Finalize()
{
	__super::Finalize();
	mpGameEngine->Finalize();
}

LRESULT CALLBACK DemoProcessor::WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam)
{
	return __super::WndProc(_hWnd, _message, _wParam, _lParam);
}

void DemoProcessor::Update()
{
	__super::Update();
	mpGameEngine->Run();
}
