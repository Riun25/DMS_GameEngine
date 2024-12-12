#pragma once
#include "Component.h"

/// <summary>
/// ����� ��Ÿ���� ������Ʈ�Դϴ�.
/// <para>
/// PlaneComponent(Vector3 ��, float �Ÿ�);
/// </para>
/// </summary>
struct PlaneComponent
	: public Component
{
public:
	PlaneComponent(std::shared_ptr<Entity> _owner, const Vector3& _axes = Vector3(0.f, 1.f, 0.f), float _distance = 0.f)
		:Component(_owner), mAxes(_axes), mDistance(_distance)
	{}

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<PlaneComponent>(*this);
	}

public:
	Vector3 mAxes;
	float mDistance;

};

