#include "WorldManager.h"

WorldManager::WorldManager(entt::registry& _registry)
	: mRegistry(_registry)
	, mpCurrentWorld(nullptr)
{

}

bool WorldManager::Initialize()
{
	mpCurrentWorld = nullptr;
	return true;
}

void WorldManager::FixedUpdate(float _dTime)
{
	if (mpCurrentWorld)
	{
		mpCurrentWorld->FixedUpdate(_dTime);
	}
}

void WorldManager::Update(float _dTime)
{
	if (mpCurrentWorld)
	{
		mpCurrentWorld->Update(_dTime);
	}
}

void WorldManager::LateUpdate(float _dTime)
{
	if (mpCurrentWorld)
	{
		mpCurrentWorld->LateUpdate(_dTime);
	}
}

void WorldManager::Finalize()
{
	if (mpCurrentWorld)
	{
		mpCurrentWorld->Finalize();
	}
}

/*
std::shared_ptr<World> WorldManager::CreateWorld()
{
	auto world = std::make_shared<World>(mRegistry);
	m_pWorld[world->GetUID()] = world;
	m_pCurrentWorld = world;
	return world;
}
*/

std::shared_ptr<World> WorldManager::AddWorld(std::shared_ptr<World> _world)
{
	mpWorld[_world->GetUID()] = _world;
	if (!mpCurrentWorld)
	{
		mpCurrentWorld = _world;
	}

	return _world;
}

std::shared_ptr<World> WorldManager::GetCurrentWorld()
{
	if (!mpCurrentWorld)
	{
		return nullptr;
	}
	return mpCurrentWorld;
}

bool WorldManager::ShouldQuit() const
{
	return mShouldQuit;
}

void WorldManager::RequestQuit()
{
	mShouldQuit = true;
}

std::shared_ptr<IData> WorldManager::GetWorldData(const std::string& _name)
{
	return mpCurrentWorld->GetWorldData(_name);
}

IData* WorldManager::GetSceneData()
{
	return mpCurrentWorld->GetCurrentScene()->GetSceneData();
}

