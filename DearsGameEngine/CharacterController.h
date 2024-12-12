#pragma once
#include "Component.h"

namespace physx
{
	class PxCapsuleController;
}

struct CharacterController
	: public Component
{
public:
	CharacterController(std::shared_ptr<Entity> _owner, float _contactOffset = 0.1f, float _stepOffset = 0.5f
		, float _slopeLimit = 0.f, float _density = 10.f)
		: Component(_owner)
		, mContactOffset(_contactOffset)
		, mStepOffset(_stepOffset)
		, mSlopeLimit(_slopeLimit)
		, mDensity(_density)
		, mpController(nullptr)
	{}

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<CharacterController>(*this);
	}

public:
	float mContactOffset;   // ���� ������
	float mStepOffset;      // ��� ������ ������
	float mSlopeLimit;      // �ִ� ���� ����
	float mDensity;         // �е�

	physx::PxCapsuleController* mpController;
};