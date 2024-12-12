#include "Scene.h"
#include "Entity.h"
#include "EventManager.h"
#include "RenderManager.h"
#include "InputManager.h"
#include "WorldManager.h"
#include "PhysicsManager.h"
#include "UIManager.h"
#include "EntityManager.h"
#include "ResourceManager.h"

Scene::Scene(entt::registry& _registry, const std::string& _name
	, EventManager* _pEventManager, RenderManager* _pRenderManager
	, PhysicsManager* _pPhysicsManager, InputManager* _pInpueManager
	, WorldManager* _pWorldManager, UIManager* _pUIManager
	, EntityManager* _pEntityManager, ResourceManager* _pResourceManager
	, SoundManager* _pSoundManager)
	: mRegistry(_registry)
	, mSceneName(_name)
	, mUid(UIDGenerator::GenerateUID<Scene>())
	, EventListener(_pEventManager)
	, mpRenderManager(_pRenderManager)
	, mpPhysicsManager(_pPhysicsManager)
	, mpInputManager(_pInpueManager)
	, mpWorldManager(_pWorldManager)
	, mpUIManager(_pUIManager)
	, mpEntityManager(_pEntityManager)
	, mpResourceManager(_pResourceManager)
	, mpSoundManager(_pSoundManager)
{
	DLOG(LOG_INFO, "Create Scene: " + _name);
}

Scene::Scene(entt::registry& _registry, const std::string& _name
	, EventManager* _pEventManager, RenderManager* _pRenderManager
	, InputManager* _pInpueManager, WorldManager* _pWorldManager)
	: mRegistry(_registry)
	, mSceneName(_name)
	, mUid(UIDGenerator::GenerateUID<Scene>())
	, EventListener(_pEventManager)
	, mpRenderManager(_pRenderManager)
	, mpPhysicsManager(nullptr)
	, mpInputManager(_pInpueManager)
	, mpWorldManager(_pWorldManager)
{
	DLOG(LOG_INFO, "Create Scene: " + _name);
}

//std::shared_ptr<Entity> Scene::CreateEntity(const std::string& _name)
//{
//	auto entity = std::make_shared<Entity>(m_registry, _name);
//	m_pEntities[entity->GetUID()] = entity;
//	return entity;
//}
//
//void Scene::AddEntity(std::shared_ptr<Entity> _entity)
//{
//	m_pEntities[_entity->GetUID()] = _entity;
//}

bool Scene::Initialize()
{
	return mpRenderManager->InitializeScene();
}

void Scene::FixedUpdate(float _dTime)
{
}

void Scene::Update(float _dTime)
{
}

void Scene::LateUpdate(float _dTime)
{
}

void Scene::Finalize()
{
	mpEntityManager->RemoveAllEntities();

	mpPhysicsManager->ClearScene();
}

//void Scene::RemoveEntity(const UID& _uid)
//{
//	if (m_pEntities.find(_uid) != m_pEntities.end())
//	{
//		if (m_pEntities[_uid]->HasComponent<Rigidbody>())
//		{
//			m_pEntities[_uid]->GetComponent<Rigidbody>().m_pRigidActor->release();
//		}
//		m_pEntities[_uid]->Destroy();
//		m_pEntities.erase(_uid);
//	}
//}
//
//void Scene::RemoveAllEntities()
//{
//	for (auto it = m_pEntities.begin(); it != m_pEntities.end(); ++it)
//	{
//		it->second->Destroy();
//	}
//	m_pEntities.clear();
//}
//
void Scene::AddResource()
{
}
//
//std::shared_ptr<Entity> Scene::GetEntity(const UID& _uid)
//{
//	if (m_pEntities.find(_uid) != m_pEntities.end())
//	{
//		return m_pEntities[_uid];
//	}
//	return nullptr;
//}
//
//std::unordered_map<UID, std::shared_ptr<Entity>> Scene::GetEntityMap() const
//{
//	return m_pEntities;
//}

/*
void Scene::AddSystem(const std::shared_ptr<System>& _pSystem)
{
	m_pSystems.push_back(_pSystem);
}
*/

UID Scene::GetUID() const
{
	return mUid;
}

std::string Scene::GetName() const
{
	return mSceneName;
}

IData* Scene::GetSceneData() const
{
	return mpSceneData;
}

