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
		mLastEntityID++;
		void* entity = new World();
	}

private:
	std::vector<World*> mEentities;
	
	uint64_t mLastEntityID = 0;
};

