#include "TestWorld.h"
#include "TestScene.h"

TestWorld::TestWorld(entt::registry& _registry, EventManager* _pEventManager, InputManager* _pInputManager)
	: World(_registry, _pEventManager)
	, mpInputManager(_pInputManager)
{

}

bool TestWorld::Initialize()
{

	return true;
}

void TestWorld::Update(float _dTime)
{
	if (mpInputManager->GetKeyState(KEY::D1) == KEY_STATE::TAP)
	{
		// 		if (GetCurrentScene()->GetUID() == 0)
		// 		{
		// 		SetScene(1);
		// 		}
		if (GetCurrentScene()->GetUID() == 1)
		{
			SetScene(0);
		}
	}

	__super::Update(_dTime);
}
