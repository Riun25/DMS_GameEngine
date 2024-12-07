#include "WorldManager.h"

WorldManager::WorldManager(entt::registry& _registry)
	: m_registry(_registry)
	, m_pCurrentWorld(nullptr)
{

}

bool WorldManager::Initialize()
{
	m_pCurrentWorld = nullptr;
	return true;
}

void WorldManager::FixedUpdate(float _dTime)
{
	if (m_pCurrentWorld)
	{
		m_pCurrentWorld->FixedUpdate(_dTime);
	}
}

void WorldManager::Update(float _dTime)
{
	if (m_pCurrentWorld)
	{
		m_pCurrentWorld->Update(_dTime);
	}
}

void WorldManager::LateUpdate(float _dTime)
{
	if (m_pCurrentWorld)
	{
		m_pCurrentWorld->LateUpdate(_dTime);
	}
}

void WorldManager::Finalize()
{
	if (m_pCurrentWorld)
	{
		m_pCurrentWorld->Finalize();
	}
}

/*
std::shared_ptr<World> WorldManager::CreateWorld()
{
	auto world = std::make_shared<World>(m_registry);
	m_pWorld[world->GetUID()] = world;
	m_pCurrentWorld = world;
	return world;
}
*/

std::shared_ptr<World> WorldManager::AddWorld(std::shared_ptr<World> _world)
{
	m_pWorld[_world->GetUID()] = _world;
	if (!m_pCurrentWorld)
	{
		m_pCurrentWorld = _world;
	}

	return _world;
}

std::shared_ptr<World> WorldManager::GetCurrentWorld()
{
	if (!m_pCurrentWorld)
	{
		return nullptr;
	}
	return m_pCurrentWorld;
}

bool WorldManager::ShouldQuit() const
{
	return m_shouldQuit;
}

void WorldManager::RequestQuit()
{
	m_shouldQuit = true;
}

std::shared_ptr<IData> WorldManager::GetWorldData(const std::string& _name)
{
	return m_pCurrentWorld->GetWorldData(_name);
}

IData* WorldManager::GetSceneData()
{
	return m_pCurrentWorld->GetCurrentScene()->GetSceneData();
}

