#include "Entity.h"
#include "Name.h"

Entity::Entity(entt::registry& _registry)
	:mRegistry(_registry)
	, mEntity(mRegistry.create())
{
}


Entity::~Entity()
{

}

bool Entity::Initialize(const std::string& _name)
{
	auto sharedThis = shared_from_this();
	mRegistry.emplace<Name>(mEntity, sharedThis, _name);
	auto uid = std::to_string(static_cast<int>(mEntity));

	DLOG(LOG_INFO, "Create Entity: " + _name + '(' + uid + ')');
	return true;
}

void Entity::Destroy()
{
	auto& name = mRegistry.get<Name>(mEntity).mName;
	auto uid = std::to_string(static_cast<int>(mEntity));
	DLOG(LOG_INFO, "Remove Entity: " + name + '(' + uid + ')');
	mRegistry.destroy(mEntity);
}

std::shared_ptr<Entity> Entity::Clone() const
{
	auto entity = mRegistry.create();
	for (auto [id, storage] : mRegistry.storage())
	{
		if (storage.contains(mEntity))
		{
			storage.push(entity, storage.value(mEntity));
		}
	}

	std::shared_ptr<Entity> clone = std::make_shared<Entity>(mRegistry);
	clone->Initialize(GetName());
	clone->mpOwner = mpOwner;
	return clone;
}

std::shared_ptr<Scene> Entity::GetOwner() const
{
	return mpOwner.lock();
}

UID Entity::GetUID() const
{
	return static_cast<UID>(mEntity);
}

std::string Entity::GetName() const
{
	if (mRegistry.all_of<Name>(mEntity))
	{
		return mRegistry.get<Name>(mEntity).mName;
	}
	else
	{
		return "Error";
	}
}

