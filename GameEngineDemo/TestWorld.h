#pragma once
#include "pch.h"

class TestWorld
	: public World
{
public:
	TestWorld() = delete;
// 	TestWorld(entt::registry& _registry, InputManager* _pInputManager);
	TestWorld(entt::registry& _registry, EventManager* _pEventManager, InputManager* _pInputManager);
	~TestWorld() = default;

	virtual bool Initialize() override;
	virtual void Update(float _dTime) override;


private:
	InputManager* mpInputManager;
};

