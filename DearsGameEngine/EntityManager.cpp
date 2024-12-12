#include "EntityManager.h"

EntityManager::EntityManager(entt::registry& _registry)
	: mRegistry(_registry)
{
}

std::shared_ptr<Entity> EntityManager::GetEntity(const UID& _uid)
{
	if (mpEntities.find(_uid) != mpEntities.end())
	{
		return mpEntities[_uid];
	}
	return nullptr;
}

std::shared_ptr<Entity> EntityManager::GetEntity(const entt::entity& _uid)
{
	UID uid = static_cast<UID>(_uid);
	if (mpEntities.find(static_cast<UID>(uid)) != mpEntities.end())
	{
		return mpEntities[static_cast<UID>(uid)];
	}
	return nullptr;
}

std::unordered_map<UID, std::shared_ptr<Entity>> EntityManager::GetEntityMap() const
{
	return mpEntities;
}

std::shared_ptr<Entity> EntityManager::CreateEntity(const std::string& _name)
{
	auto entity = std::make_shared<Entity>(mRegistry);
	entity->Initialize(_name);
	mpEntities[entity->GetUID()] = entity;
	return entity;
}

void EntityManager::AddEntity(std::shared_ptr<Entity> _entity)
{
	mpEntities[_entity->GetUID()] = _entity;
}

void EntityManager::RemoveEntity(const UID& _uid)
{
	if (mpEntities.find(_uid) != mpEntities.end())
	{
		auto& pEntity = mpEntities[_uid];

		if (pEntity->HasComponent<Transform>())
		{
			pEntity->GetComponent<Transform>().mpParent = nullptr;
			pEntity->GetComponent<Transform>().mpOwner = nullptr;
		}

		// Rigidbody가 있으면 PhysX 객체 해제
		if (pEntity->HasComponent<Rigidbody>())
		{
			pEntity->GetComponent<Rigidbody>().mpRigidActor->release();
		}

		pEntity->Destroy();
		mpEntities.erase(_uid);
	}
}

void EntityManager::RemoveAllEntities()
{
	std::vector<UID> keysToRemove;
	for (auto& [key, value] : mpEntities)
	{
		keysToRemove.push_back(key);
	}

	// 미리 저장한 키들을 사용해 삭제
	for (const auto& key : keysToRemove)
	{
		RemoveEntity(key);
	}

	mpEntities.clear();
}