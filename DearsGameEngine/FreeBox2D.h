#pragma once
#include "Component.h"

/// <summary>
/// ������ �簢�� ui�� ���� ������Ʈ Ŭ����
/// <para>
/// FreeBox2D( Vector2 ��ġ, );
/// </para>
/// </summary>
struct FreeBox2D
	: public Component
{
public:
	FreeBox2D(std::shared_ptr<Entity> _owner, Vector2 _position = Vector2(), int _layer = 0, const bool& _isVisible = true,
		Vector4 _rgba = Vector4(0.0f, 0.0f, 0.0f, 1.0f))
		: Component(_owner), mPosition(_position), mLayer(_layer),
		mIsVisible(_isVisible), mRgba(_rgba), mHasDepth(false)
	{}

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<FreeBox2D>(*this);
	}

public:

	Vector2 mPosition;
	int mLayer; // ���ڰ� Ŭ���� �ڿ� �׷����ϴ�.
	bool mIsVisible; // ������������ ����� ������, true�� ���� �׸���.
	Vector4 mRgba;
	bool mHasDepth;	// 3D �������� z������ ���̰��� ��� �ϸ� ����Ѵ�.
};