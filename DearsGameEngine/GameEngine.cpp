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
	: mHWnd(_hWnd), mWidth(_width), mHeight(_height), mRegistry()
	, mpTimeManager(nullptr)
	, mpInputManager(nullptr)
	, mpResourceManager(nullptr)
	, mpSoundManager(nullptr)
	, mpEventManager(nullptr)
	, mpWorldManager(nullptr)
	, mpPhysicsManager(nullptr)
	, mpRenderManager(nullptr)
	, mpUIManager(nullptr)
	, mpEntityManager(nullptr)
	, mpInfo(nullptr)
{
}

bool GameEngine::Initialize()
{
	InitializeLogger();
	SET_LOG_LEVEL(LOG_INFO);
	PrintLoggerInfo();

	/// 그래픽스 엔진 생성
	mpGraphicsEngine = std::make_shared<DearsGraphicsEngine>(mHWnd, mWidth, mHeight);


	/// 매니저 클래스 생성 및 초기화
	mpTimeManager = new TimeManager();
	mpInfo = new GameInfo(mpTimeManager);
	mpInputManager = new InputManager();
	mpSoundManager = new SoundManager(/*mpEventManager*/);
	mpResourceManager = new ResourceManager(mpGraphicsEngine,mpSoundManager);
	mpEventManager = new EventManager();
	mpWorldManager = new WorldManager(mRegistry);
	mpPhysicsManager = new PhysicsManager(mRegistry, this);
	mpRenderManager = new RenderManager(mRegistry, mpGraphicsEngine, mpWorldManager, mpTimeManager);
	mpUIManager = new UIManager(mRegistry, mpInputManager, mpGraphicsEngine, mpInfo);
	mpEntityManager = new EntityManager(mRegistry);


	/// 그래픽스 엔진 초기화
	mpGraphicsEngine->Initialize();

	if (!mpTimeManager->Initialize()
		|| !mpInputManager->Initialize()
		|| !mpResourceManager->Initialize()
		|| !mpEventManager->Initialize()
		|| !mpWorldManager->Initialize()
		|| !mpPhysicsManager->Initialize()
		|| !mpRenderManager->Initialize()
		|| !mpSoundManager->Initialize()
		|| !mpUIManager->Initialize())
	{
		return false;
	}
	
	return true;
}

void GameEngine::Run()
{	
	mpTimeManager->Update();

	mFixedDeltaTime = mpTimeManager->FixedDeltaTime();

	mDeltaTime = mpTimeManager->DeltaTime();
	mAccumulator += mDeltaTime;

	while (mAccumulator >= mFixedDeltaTime)
	{
		FixedUpdate(mFixedDeltaTime);
		mAccumulator -= mFixedDeltaTime; // 누적값에서 고정된 프레임 시간만큼 뺌
	}
	Update(mDeltaTime);
	LateUpdate(mDeltaTime);
	BeginRender();
	Render(mDeltaTime);
	EndRender();
}

void GameEngine::FixedUpdate(float _fixedDeltaTime)
{
	mpPhysicsManager->FixedUpdate(_fixedDeltaTime);
	mpWorldManager->FixedUpdate(_fixedDeltaTime);
}

// 매니저들을 업데이트합니다.
void GameEngine::Update(float _dTime)
{
	mpInputManager->Update(_dTime);
	mpEventManager->Update(_dTime);
	mpSoundManager->Update(_dTime);
	mpPhysicsManager->Update(_dTime);
	mpWorldManager->Update(_dTime);
	mpUIManager->Update(_dTime);
	mpRenderManager->Update(_dTime);
}

void GameEngine::Render(float _dTime)
{
	mpUIManager->Render(_dTime);
	mpRenderManager->Render(_dTime);
}

void GameEngine::LateUpdate(float _dTime)
{
	mpEventManager->LateUpdate(_dTime);
	mpRenderManager->LateUpdate(_dTime);
	mpWorldManager->LateUpdate(_dTime);
}

// 관리하고 있는 매니저들의 메모리할당을 해제합니다.
void GameEngine::Finalize()
{
	mpEventManager->Finalize();
	mpSoundManager->Finalize();
	mpInputManager->Finalize();
	mpTimeManager->Finalize();
	mpUIManager->Finalize();
	mpWorldManager->Finalize();
	mpResourceManager->Finalize();

	delete mpTimeManager;
	delete mpInputManager;
	delete mpResourceManager;
	delete mpEventManager;
	delete mpWorldManager;
	delete mpPhysicsManager;
	delete mpRenderManager;
	delete mpUIManager;
	delete mpSoundManager;
	delete mpEntityManager;

	delete mpInfo;

	CleanupLogger();
}

void GameEngine::BeginRender()
{
	mpRenderManager->BeginRender();
}

void GameEngine::EndRender()
{
	mpRenderManager->EndRender();
}

PhysicsManager* GameEngine::GetPhysicsManager() const
{
	return mpPhysicsManager;
}

RenderManager* GameEngine::GetRenderManager() const
{
	return mpRenderManager;
}

UIManager* GameEngine::GetUIManager() const
{
	return mpUIManager;
}

EntityManager* GameEngine::GetEntityManager() const
{
	return mpEntityManager;
}

TimeManager* GameEngine::GetTimeManager() const
{
	return mpTimeManager;
}

InputManager* GameEngine::GetInputManager() const
{
	return mpInputManager;
}

ResourceManager* GameEngine::GetResourceManager() const
{
	return mpResourceManager;
}

SoundManager* GameEngine::GetSoundManager() const
{
	return mpSoundManager;
}

EventManager* GameEngine::GetEventManager() const
{
	return mpEventManager;
}

WorldManager* GameEngine::GetWorldManager() const
{
	return mpWorldManager;
}