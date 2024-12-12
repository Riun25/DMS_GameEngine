#pragma once
#include "Component.h"
#include "Graphics/BufferData.h"

/// <summary>
/// ���� �ܰ����� �׸��� ���� �ʿ��� ������Ʈ
/// <para >
/// OutlineComponent(float �β�, vector3 ����);
/// </para>
/// </summary>
struct OutlineComponent
	:public Component
{
public:
	OutlineComponent(std::shared_ptr<Entity> _owner, float _thickness = 1.01, const Vector3& _color = Vector3(1.f))
		: Component(_owner), mThickness(_thickness), mColor(_color)
	{}

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<OutlineComponent>(*this);
	}

public:
	float mThickness;	// �β�
	Vector3 mColor;	// ��

	VSEdgeConstantBufferData mVSEdgeCBD;
	PSEdgeConstantBufferData mPSEdgeCBD;

};