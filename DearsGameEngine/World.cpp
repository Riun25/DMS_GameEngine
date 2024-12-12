#include "World.h"
#include "Scene.h"
#include "Entity.h"
#include "RenderManager.h"
#include "PhysicsManager.h"

// World::World(entt::registry& _registry)
// 	: mRegistry(_registry)
// 	, m_uid(UIDGenerator::GenerateUID<World>())
// 	, EventListener(nullptr)
// {
// }

World::World(entt::registry& _registry, EventManager* _pEventManager)
	: mRegistry(_registry)
	, mUid(UIDGenerator::GenerateUID<World>())
	, EventListener(_pEventManager)
{

}

bool World::Initialize()
{
	mPreviousScene = 0;
	return true;
}

void World::FixedUpdate(float _dTime)
{
	mpCurrentScene->FixedUpdate(_dTime);
}

void World::Update(float _dTime)
{
	mpCurrentScene->Update(_dTime);
}

void World::LateUpdate(float _dTime)
{
	mpCurrentScene->LateUpdate(_dTime);
}

void World::Finalize()
{
	// 어차피 현재 씬만 메모리 할당 되어있기 때문에 현재 씬만 정리해도 될듯?
// 	for (auto& [uid, scene] : m_pScenes)
// 	{
// 		scene->Finalize();
// 	}
	mpCurrentScene->Finalize();

}

std::shared_ptr<Scene> World::AddScene(std::shared_ptr<Scene> _pScene)
{
	mpScenes[_pScene->GetUID()] = _pScene;
	if (!mpCurrentScene)
	{
		mpCurrentScene = _pScene;
	}

	return _pScene;

}

/*
std::shared_ptr<Scene> World::CreateScene(const std::string& _name)
{
	auto scene = std::make_shared<Scene>(mRegistry, _name);
	m_pScenes[scene->GetUID()] = scene;
	return scene;
}
*/

void World::RemoveScene(std::shared_ptr<Scene> _pScene)
{
	mpScenes.erase(_pScene->GetUID());
}

void World::SetScene(const UID& _uid)
{
	auto it = mpScenes.find(_uid);
	if (it != mpScenes.end())
	{
		if (mpCurrentScene)
		{
			mPreviousScene = mpCurrentScene->GetUID();
			mpCurrentScene->Finalize();
		}

		mpCurrentScene = it->second;
		DLOG(LOG_INFO, "Scene Change: " + mpCurrentScene->GetName() + '(' + std::to_string(_uid) + ')');
		mpCurrentScene->Initialize();
	}
	else
	{
		DLOG(LOG_ERROR, "Scene Change Failed: UID not found.");
	}
}

void World::SetScene(const std::string& _name)
{
	auto it = std::find_if(mpScenes.begin(), mpScenes.end(),
		[&_name](const std::pair<UID, std::shared_ptr<Scene>>& pair)
		{
			return pair.second->GetName() == _name;
		});

	if (it != mpScenes.end())
	{
		if (mpCurrentScene)
		{
			mPreviousScene = mpCurrentScene->GetUID();
			mpCurrentScene->Finalize();
		}

		mpCurrentScene = it->second;
		DLOG(LOG_INFO, "Scene Change: " + _name + '(' + std::to_string(mpCurrentScene->GetUID()) + ')');
		mpCurrentScene->Initialize();
	}
	else
	{
		DLOG(LOG_ERROR, "Scene Change Failed: Name not found.");
	}
}

std::unordered_map<UID, std::shared_ptr<Scene>> World::GetSceneMap() const
{
	return mpScenes;
}

std::shared_ptr<Scene> World::GetCurrentScene()
{
	if (!mpCurrentScene)
	{
		return nullptr;
	}
	return mpCurrentScene;
}

UID World::GetPreviousScene()
{
	return mPreviousScene;
}

UID World::GetUID() const
{
	return mUid;
}

std::shared_ptr<IData> World::GetWorldData(const std::string& _name)
{
	return mpWorldData[_name];
}

