#pragma once
#include "framework.h"
#include "Entity.h"
#include "WorldObject.h"

class Component abstract
{
public:
	Component() {}
	virtual ~Component() {}

// 	virtual Entity* GetEntity() abstract;

// 	virtual void Remove(Entity* entity) abstract;
	virtual void Destroy(World* world) abstract;

	uint64_t ID;		// Entity¿« ID // EntityID=30ull; Version=24ull; WorldID=10ull;
};

template <typename T>
class ComponentHandle
{
public:
	ComponentHandle()
		: component()
	{
	}
	ComponentHandle(T* component)
		: component(component)
	{
	}

	T* operator->() const {
		return component;
	}

	operator bool() const 
	{
		return component != nullptr;
	}

	T& Get()
	{
		return *component;
	}

private:
	T* component;
};