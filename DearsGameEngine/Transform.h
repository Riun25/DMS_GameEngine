#pragma once
#include "Component.h"

/// <summary>
/// ������Ʈ�� ��ġ, ȸ��, ũ�⸦ ������ �� �ִ� ������Ʈ�Դϴ�.
/// <para>
/// Transform(Vector3 ��ġ, Vector3 ȸ����, Vector3 ũ��, Vector3 �θ� ��ġ, Quaternion �θ� ȸ��, Vector3 �θ� ũ��);
/// </para>
/// </summary>
struct Transform
	: public Component
{
public:
	Transform(std::shared_ptr<Entity> _owner, Vector3 _localPosition = Vector3(), Vector3 _localRotation = Vector3(), Vector3 _localScale = Vector3(1.f)
		, Vector3 _parentPosition = Vector3(), Vector3 _parentRotation = Vector3(), Vector3 _parentScale = Vector3(1.f))
		: Component(_owner), mLocalPosition(_localPosition), mLocalRotation(_localRotation), mLocalScale(_localScale)
		, mWorldPosition(_parentPosition), mWorldRotation(_parentRotation), mWorldScale(_parentScale)
		, mpParent(nullptr)
	{
	}

	virtual std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<Transform>(*this);
	}

	Matrix GetTransformMatrix();

public:
	Transform* mpParent;

	// ���� ��ü �������� ��ġ, ȸ��, ũ�⸦ ��Ÿ���ϴ�.
	Vector3 mLocalPosition;
	Vector3 mLocalRotation;
	Vector3 mLocalScale;

	// �θ� ��ü�� ��ġ, ȸ��, ũ�⸦ ��Ÿ���ϴ�.
	Vector3 mWorldPosition;
	Vector3 mWorldRotation;
	Vector3 mWorldScale;

	Matrix mLocalMatrix;
};
