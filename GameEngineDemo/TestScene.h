#pragma once
#include "pch.h"

class Scene;
class Event;
class EventManager;

/// <summary>
/// ���� ������ ����� �׽�Ʈ�ϱ� ���� �׽�Ʈ ���Դϴ�.
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
	std::shared_ptr<Entity> mpEntity1;
	std::shared_ptr<Entity> mpPlayer1;
	std::shared_ptr<Entity> mpPlayer1_2;
	std::shared_ptr<Entity> mpEntity1_1;
	std::shared_ptr<Entity> mpEntity1_2;
	std::shared_ptr<Entity> mpEntity2;
	std::shared_ptr<Entity> mpEntity3;
	std::shared_ptr<Entity> mpCameraEntity;

	// �ڵ������� ��ȯ�ǰ� �Ϸ��� �ھƳ� ������. �׳� �׽�Ʈ����
	float mTestTimer = 0.0f;
};
