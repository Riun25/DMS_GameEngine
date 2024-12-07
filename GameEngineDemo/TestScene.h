#pragma once
#include "pch.h"

class Scene;
class Event;
class EventManager;

/// <summary>
/// 게임 엔진의 기능을 테스트하기 위한 테스트 씬입니다.
/// 2024.06.26 _Doyo;
/// </summary>
class TestScene
	: public Scene
{
public:
	TestScene(entt::registry& _registry, const std::string& _name
		, EventManager* _pEventManager, RenderManager* _pRenderManager
		, PhysicsManager* _pPhysicsManager, InputManager* _pInpueManager
		, WorldManager* _pWorldManager, UIManager* _pUIManager
		, EntityManager* _pEntityManager, ResourceManager* _pResourceManager
	,SoundManager* _pSoundManager);

	virtual bool Initialize() override;

	virtual void Update(float _dTime) override;

	virtual void LateUpdate(float _dTime) override;

	virtual void Finalize() override;

	virtual void AddResource() override;

	void OnCharacterMove(const Event& _event);

private:
	std::shared_ptr<Entity> entity1;
	std::shared_ptr<Entity> player1;
	std::shared_ptr<Entity> player1_2;
	std::shared_ptr<Entity> entity1_1;
	std::shared_ptr<Entity> entity1_2;
	std::shared_ptr<Entity> entity2;
	std::shared_ptr<Entity> entity3;
	std::shared_ptr<Entity> cameraEntity;

	// 자동적으로 변환되게 하려고 박아논 변수다. 그냥 테스트용임
	float testTimer = 0.0f;
};
