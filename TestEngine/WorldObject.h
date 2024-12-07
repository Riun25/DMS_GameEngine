#pragma once
#include "framework.h"
#include "Entity.h"
#include "IComponent.h"

class World
{
public:
	World() {}
	~World() {}

	void* CreateObject()
	{
		lastEntityID++;
		void* entity = new World();
	}

private:
	std::vector<World*> entities;
	
	uint64_t lastEntityID = 0;
};

