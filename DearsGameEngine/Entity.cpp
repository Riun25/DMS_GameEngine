#include "Entity.h"
#include "Name.h"

Entity::Entity(entt::registry& _registry)
	:m_registry(_registry)
	, m_entity(m_registry.create())
{
}


Entity::~Entity()
{

}

bool Entity::Initialize(const std::string& _name)
{
	auto sharedThis = shared_from_this();
	m_registry.emplace<Name>(m_entity, sharedThis, _name);
	auto uid = std::to_string(static_cast<int>(m_entity));

	DLOG(LOG_INFO, "Create Entity: " + _name + '(' + uid + ')');
	return true;
}

void Entity::Destroy()
{
	auto& name = m_registry.get<Name>(m_entity).m_name;
	auto uid = std::to_string(static_cast<int>(m_entity));
	DLOG(LOG_INFO, "Remove Entity: " + name + '(' + uid + ')');
	m_registry.destroy(m_entity);
}

std::shared_ptr<Entity> Entity::Clone() const
{
	auto entity = m_registry.create();
	for (auto [id, storage] : m_registry.storage())
	{
		if (storage.contains(m_entity))
		{
			storage.push(entity, storage.value(m_entity));
		}
	}

	std::shared_ptr<Entity> clone = std::make_shared<Entity>(m_registry);
	clone->Initialize(GetName());
	clone->m_owner = m_owner;
	return clone;
}

std::shared_ptr<Scene> Entity::GetOwner() const
{
	return m_owner.lock();
}

UID Entity::GetUID() const
{
	return static_cast<UID>(m_entity);
}

std::string Entity::GetName() const
{
	if (m_registry.all_of<Name>(m_entity))
	{
		return m_registry.get<Name>(m_entity).m_name;
	}
	else
	{
		return "Error";
	}
}

