#include "GameEngine.h"
#include "TimeManager.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "UIManager.h"
#include "EntityManager.h"
#include "SoundManager.h"
#include "EventManager.h"
#include "WorldManager.h"
#include "PhysicsManager.h"
#include "RenderManager.h"
#include "FileManager.h"
#include "GameInfo.h"

class DearsGraphicsEngine;

GameEngine::GameEngine(HWND _hWnd, int _width, int _height)
	: m_hWnd(_hWnd), m_width(_width), m_height(_height), m_registry()
	, m_pTimeManager(nullptr)
	, m_pInputManager(nullptr)
	, m_pResourceManager(nullptr)
	, m_pSoundManager(nullptr)
	, m_pEventManager(nullptr)
	, m_pWorldManager(nullptr)
	, m_pPhysicsManager(nullptr)
	, m_pRenderManager(nullptr)
	, m_pUIManager(nullptr)
	, m_pEntityManager(nullptr)
	, m_pInfo(nullptr)
{
}

bool GameEngine::Initialize()
{
	InitializeLogger();
	SET_LOG_LEVEL(LOG_INFO);
	PrintLoggerInfo();

	/// 그래픽스 엔진 생성
	m_pGraphicsEngine = std::make_shared<DearsGraphicsEngine>(m_hWnd, m_width, m_height);


	/// 매니저 클래스 생성 및 초기화
	m_pTimeManager = new TimeManager();
	m_pInfo = new GameInfo(m_pTimeManager);
	m_pInputManager = new InputManager();
	m_pSoundManager = new SoundManager(/*m_pEventManager*/);
	m_pResourceManager = new ResourceManager(m_pGraphicsEngine,m_pSoundManager);
	m_pEventManager = new EventManager();
	m_pWorldManager = new WorldManager(m_registry);
	m_pPhysicsManager = new PhysicsManager(m_registry, this);
	m_pRenderManager = new RenderManager(m_registry, m_pGraphicsEngine, m_pWorldManager, m_pTimeManager);
	m_pUIManager = new UIManager(m_registry, m_pInputManager, m_pGraphicsEngine, m_pInfo);
	m_pEntityManager = new EntityManager(m_registry);


	/// 그래픽스 엔진 초기화
	m_pGraphicsEngine->Initialize();

	if (!m_pTimeManager->Initialize()
		|| !m_pInputManager->Initialize()
		|| !m_pResourceManager->Initialize()
		|| !m_pEventManager->Initialize()
		|| !m_pWorldManager->Initialize()
		|| !m_pPhysicsManager->Initialize()
		|| !m_pRenderManager->Initialize()
		|| !m_pSoundManager->Initialize()
		|| !m_pUIManager->Initialize())
	{
		return false;
	}
	
	return true;
}

void GameEngine::Run()
{	
	m_pTimeManager->Update();

	m_fixedDeltaTime = m_pTimeManager->FixedDeltaTime();

	m_deltaTime = m_pTimeManager->DeltaTime();
	m_accumulator += m_deltaTime;

	while (m_accumulator >= m_fixedDeltaTime)
	{
		FixedUpdate(m_fixedDeltaTime);
		m_accumulator -= m_fixedDeltaTime; // 누적값에서 고정된 프레임 시간만큼 뺌
	}
	Update(m_deltaTime);
	LateUpdate(m_deltaTime);
	BeginRender();
	Render(m_deltaTime);
	EndRender();
}

void GameEngine::FixedUpdate(float _fixedDeltaTime)
{
	m_pPhysicsManager->FixedUpdate(_fixedDeltaTime);
	m_pWorldManager->FixedUpdate(_fixedDeltaTime);
}

// 매니저들을 업데이트합니다.
void GameEngine::Update(float _dTime)
{
	m_pInputManager->Update(_dTime);
	m_pEventManager->Update(_dTime);
	m_pSoundManager->Update(_dTime);
	m_pPhysicsManager->Update(_dTime);
	m_pWorldManager->Update(_dTime);
	m_pUIManager->Update(_dTime);
	m_pRenderManager->Update(_dTime);
}

void GameEngine::Render(float _dTime)
{
	m_pUIManager->Render(_dTime);
	m_pRenderManager->Render(_dTime);
}

void GameEngine::LateUpdate(float _dTime)
{
	m_pEventManager->LateUpdate(_dTime);
	m_pRenderManager->LateUpdate(_dTime);
	m_pWorldManager->LateUpdate(_dTime);
}

// 관리하고 있는 매니저들의 메모리할당을 해제합니다.
void GameEngine::Finalize()
{
	m_pEventManager->Finalize();
	m_pSoundManager->Finalize();
	m_pInputManager->Finalize();
	m_pTimeManager->Finalize();
	m_pUIManager->Finalize();
	m_pWorldManager->Finalize();
	m_pResourceManager->Finalize();

	delete m_pTimeManager;
	delete m_pInputManager;
	delete m_pResourceManager;
	delete m_pEventManager;
	delete m_pWorldManager;
	delete m_pPhysicsManager;
	delete m_pRenderManager;
	delete m_pUIManager;
	delete m_pSoundManager;
	delete m_pEntityManager;

	delete m_pInfo;

	CleanupLogger();
}

void GameEngine::BeginRender()
{
	m_pRenderManager->BeginRender();
}

void GameEngine::EndRender()
{
	m_pRenderManager->EndRender();
}

PhysicsManager* GameEngine::GetPhysicsManager() const
{
	return m_pPhysicsManager;
}

RenderManager* GameEngine::GetRenderManager() const
{
	return m_pRenderManager;
}

UIManager* GameEngine::GetUIManager() const
{
	return m_pUIManager;
}

EntityManager* GameEngine::GetEntityManager() const
{
	return m_pEntityManager;
}

TimeManager* GameEngine::GetTimeManager() const
{
	return m_pTimeManager;
}

InputManager* GameEngine::GetInputManager() const
{
	return m_pInputManager;
}

ResourceManager* GameEngine::GetResourceManager() const
{
	return m_pResourceManager;
}

SoundManager* GameEngine::GetSoundManager() const
{
	return m_pSoundManager;
}

EventManager* GameEngine::GetEventManager() const
{
	return m_pEventManager;
}

WorldManager* GameEngine::GetWorldManager() const
{
	return m_pWorldManager;
}
