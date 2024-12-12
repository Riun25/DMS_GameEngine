#pragma once
#include "Component.h"
#include "Collider.h"

/// <summary>
/// ĸ�� ����� �����ϴ� ������Ʈ�Դϴ�.
/// <para>
/// CapsuleCollider(bool Ʈ���� ����, Vector3 ����, float ������, float ����, ĸ�� ����);
/// </para>
/// </summary>
struct CapsuleCollider
	:public Collider
{
public:
	CapsuleCollider(std::shared_ptr<Entity> _owner, bool _isTrigger = false, const Vector3& _center = Vector3()
		, float _radius = 1.f, float _height = 1.f, int _direction = 1)
		: Collider(_owner, _isTrigger, _center, Vector3(1.f))
		, mRadius(_radius), mHeight(_height), mDirection(_direction)
	{}

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<CapsuleCollider>(*this);
	}

public:
	float mRadius;        // ĸ���� ������
	float mHeight;        // ĸ���� ����
	int mDirection;       // ĸ���� ���� (0: X��, 1: Y��, 2: Z��)	// Vector3�� �����Ұ�
};