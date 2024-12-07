#include "GameManager.h"
#include "TimeManager.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "UIManager.h"
#include "SoundManager.h"
#include "EventManager.h"
#include "WorldManager.h"

bool GameManager::Initialize()
{
	m_pTimeManager = new TimeManager();
	m_pInputManager = new InputManager();
	m_pResourceManager = new ResourceManager();
	m_pWorldManager = new WorldManager();
	m_pSoundManager = SoundManager::GetInstance();
	m_pEventManager = new EventManager();

	if (!m_pTimeManager->Initialize()
		|| !m_pInputManager->Initialize()
		|| !m_pResourceManager->Initialize()
		|| !m_pSoundManager->Initialize()
		|| !m_pEventManager->Initialize())
	{
		return false;
	}

	// Sound Test
	m_pSoundManager->LoadBGM("../../Resources/Sound/Seven Nation Army.mp3", "SNA");
	m_pSoundManager->LoadBGM("../../Resources/Sound/rather be.mp3", "RB");
	m_pSoundManager->PlayBGM("RB");

	return true;
}

// 고정된 프레임으로 물리연산을 업데이트 합니다.
void GameManager::FixedUpdate()
{
}

// 물리 관련 시스템을 제외한 모든 매니저들을 업데이트합니다.
void GameManager::Update()
{
	m_pTimeManager->Update();
	m_pInputManager->Update();
	m_pSoundManager->Update();
	m_pResourceManager->Update();
	m_pEventManager->Update();
}

// ?
void GameManager::LateUpdate()
{
}

// 관리하고 있는 매니저들의 메모리할당을 해제합니다.
void GameManager::Finalize()
{
	m_pEventManager->Finalize();
	m_pSoundManager->Finalize();
	m_pResourceManager->Finalize();
	m_pInputManager->Finalize();
	m_pTimeManager->Finalize();

}
