#include "World.h"
#include "Scene.h"
#include "Entity.h"
#include "RenderManager.h"
#include "PhysicsManager.h"

// World::World(entt::registry& _registry)
// 	: m_registry(_registry)
// 	, m_uid(UIDGenerator::GenerateUID<World>())
// 	, EventListener(nullptr)
// {
// }

World::World(entt::registry& _registry, EventManager* _pEventManager)
	: m_registry(_registry)
	, m_uid(UIDGenerator::GenerateUID<World>())
	, EventListener(_pEventManager)
{

}

bool World::Initialize()
{
	m_previousScene = 0;
	return true;
}

void World::FixedUpdate(float _dTime)
{
	m_pCurrentScene->FixedUpdate(_dTime);
}

void World::Update(float _dTime)
{
	m_pCurrentScene->Update(_dTime);
}

void World::LateUpdate(float _dTime)
{
	m_pCurrentScene->LateUpdate(_dTime);
}

void World::Finalize()
{
	// 어차피 현재 씬만 메모리 할당 되어있기 때문에 현재 씬만 정리해도 될듯?
// 	for (auto& [uid, scene] : m_pScenes)
// 	{
// 		scene->Finalize();
// 	}
	m_pCurrentScene->Finalize();

}

std::shared_ptr<Scene> World::AddScene(std::shared_ptr<Scene> _pScene)
{
	m_pScenes[_pScene->GetUID()] = _pScene;
	if (!m_pCurrentScene)
	{
		m_pCurrentScene = _pScene;
	}

	return _pScene;

}

/*
std::shared_ptr<Scene> World::CreateScene(const std::string& _name)
{
	auto scene = std::make_shared<Scene>(m_registry, _name);
	m_pScenes[scene->GetUID()] = scene;
	return scene;
}
*/

void World::RemoveScene(std::shared_ptr<Scene> _pScene)
{
	m_pScenes.erase(_pScene->GetUID());
}

void World::SetScene(const UID& _uid)
{
	auto it = m_pScenes.find(_uid);
	if (it != m_pScenes.end())
	{
		if (m_pCurrentScene)
		{
			m_previousScene = m_pCurrentScene->GetUID();
			m_pCurrentScene->Finalize();
		}

		m_pCurrentScene = it->second;
		DLOG(LOG_INFO, "Scene Change: " + m_pCurrentScene->GetName() + '(' + std::to_string(_uid) + ')');
		m_pCurrentScene->Initialize();
	}
	else
	{
		DLOG(LOG_ERROR, "Scene Change Failed: UID not found.");
	}
}

void World::SetScene(const std::string& _name)
{
	auto it = std::find_if(m_pScenes.begin(), m_pScenes.end(),
		[&_name](const std::pair<UID, std::shared_ptr<Scene>>& pair)
		{
			return pair.second->GetName() == _name;
		});

	if (it != m_pScenes.end())
	{
		if (m_pCurrentScene)
		{
			m_previousScene = m_pCurrentScene->GetUID();
			m_pCurrentScene->Finalize();
		}

		m_pCurrentScene = it->second;
		DLOG(LOG_INFO, "Scene Change: " + _name + '(' + std::to_string(m_pCurrentScene->GetUID()) + ')');
		m_pCurrentScene->Initialize();
	}
	else
	{
		DLOG(LOG_ERROR, "Scene Change Failed: Name not found.");
	}
}

std::unordered_map<UID, std::shared_ptr<Scene>> World::GetSceneMap() const
{
	return m_pScenes;
}

std::shared_ptr<Scene> World::GetCurrentScene()
{
	if (!m_pCurrentScene)
	{
		return nullptr;
	}
	return m_pCurrentScene;
}

UID World::GetPreviousScene()
{
	return m_previousScene;
}

UID World::GetUID() const
{
	return m_uid;
}

std::shared_ptr<IData> World::GetWorldData(const std::string& _name)
{
	return m_pWorldData[_name];
}

