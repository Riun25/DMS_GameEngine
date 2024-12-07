#include "DemoProcessor.h"
#include <memory>
#include "../DearsGameEngine/DearsGameEngineAPI.h"
#include "TestScene.h"
#include "TestWorld.h"

DemoProcessor::DemoProcessor(HINSTANCE _hInstance, const WCHAR* _szTitle, int _iconResourceId)
	:BaseWindowsProcessor(_hInstance, _szTitle, _iconResourceId)
	, m_pGameEngine(nullptr)
{
}

bool DemoProcessor::Initialize()
{
	if (!__super::Initialize())
	{
		return false;
	}

	m_pGameEngine = new GameEngine(m_hWnd, m_screenWidth, m_screenHeight);
	m_pGameEngine->Initialize();

	// Create World Test
	m_pWorld = std::make_shared<TestWorld>(m_pGameEngine->m_registry,m_pGameEngine->GetEventManager(), m_pGameEngine->GetInputManager());
	m_pGameEngine->GetWorldManager()->AddWorld(m_pWorld);

	m_pScene1 = std::make_shared<TestScene>(m_pGameEngine->m_registry, "TestScene"
		, m_pGameEngine->GetEventManager(), m_pGameEngine->GetRenderManager()
		, m_pGameEngine->GetPhysicsManager(), m_pGameEngine->GetInputManager()
		, m_pGameEngine->GetWorldManager(), m_pGameEngine->GetUIManager()
		, m_pGameEngine->GetEntityManager(), m_pGameEngine->GetResourceManager()
	,m_pGameEngine->GetSoundManager());

	m_pScene2 = std::make_shared<TestScene>(m_pGameEngine->m_registry, "TestScene"
		, m_pGameEngine->GetEventManager(), m_pGameEngine->GetRenderManager()
		, m_pGameEngine->GetPhysicsManager(), m_pGameEngine->GetInputManager()
		, m_pGameEngine->GetWorldManager(), m_pGameEngine->GetUIManager()
		, m_pGameEngine->GetEntityManager(), m_pGameEngine->GetResourceManager()
		, m_pGameEngine->GetSoundManager());

	m_pGameEngine->GetWorldManager()->GetCurrentWorld()->AddScene(m_pScene1);
	m_pGameEngine->GetWorldManager()->GetCurrentWorld()->AddScene(m_pScene2);

	m_pGameEngine->GetSoundManager()->LoadBGM("../../Resources/Sound/Snd_bgm_Battle.wav", "Snd_bgm_Battle");
	m_pGameEngine->GetSoundManager()->PlayBGM("Snd_bgm_Battle");
	m_pGameEngine->GetWorldManager()->GetCurrentWorld()->SetScene(m_pScene1->GetUID());
	return true;
}

void DemoProcessor::Run()
{
	while (true)
	{
		if (PeekMessage(&m_msg, NULL, 0, 0, PM_REMOVE))
		{
			if (m_msg.message == WM_QUIT)
			{
				break;
			}

			if (!TranslateAccelerator(m_msg.hwnd, m_hAccelTable, &m_msg))
			{
				TranslateMessage(&m_msg);
				DispatchMessage(&m_msg);
			}
		}
		else
		{
			if (m_pGameEngine->GetWorldManager()->ShouldQuit())
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
	m_pGameEngine->Finalize();
}

LRESULT CALLBACK DemoProcessor::WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam)
{
	return __super::WndProc(_hWnd, _message, _wParam, _lParam);
}

void DemoProcessor::Update()
{
	__super::Update();
	m_pGameEngine->Run();
}
