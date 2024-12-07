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
	: m_registry(_registry)
	, m_sceneName(_name)
	, m_uid(UIDGenerator::GenerateUID<Scene>())
	, EventListener(_pEventManager)
	, m_pRenderManager(_pRenderManager)
	, m_pPhysicsManager(_pPhysicsManager)
	, m_pInputManager(_pInpueManager)
	, m_pWorldManager(_pWorldManager)
	, m_pUIManager(_pUIManager)
	, m_pEntityManager(_pEntityManager)
	, m_pResourceManager(_pResourceManager)
	, m_pSoundManager(_pSoundManager)
{
	DLOG(LOG_INFO, "Create Scene: " + _name);
}

Scene::Scene(entt::registry& _registry, const std::string& _name
	, EventManager* _pEventManager, RenderManager* _pRenderManager
	, InputManager* _pInpueManager, WorldManager* _pWorldManager)
	: m_registry(_registry)
	, m_sceneName(_name)
	, m_uid(UIDGenerator::GenerateUID<Scene>())
	, EventListener(_pEventManager)
	, m_pRenderManager(_pRenderManager)
	, m_pPhysicsManager(nullptr)
	, m_pInputManager(_pInpueManager)
	, m_pWorldManager(_pWorldManager)
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
	return m_pRenderManager->InitializeScene();
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
	m_pEntityManager->RemoveAllEntities();

	m_pPhysicsManager->ClearScene();
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
	return m_uid;
}

std::string Scene::GetName() const
{
	return m_sceneName;
}

IData* Scene::GetSceneData() const
{
	return m_pSceneData;
}

