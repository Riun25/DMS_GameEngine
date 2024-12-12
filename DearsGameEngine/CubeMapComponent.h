#pragma once
#include "Component.h"
#include "Graphics/BufferData.h"

/// <summary>
/// ť����� �׷��ֱ� ���� �ʿ��� ������Ʈ
/// </summary>
struct CubeMapComponent
	: public Component
{
public:
	CubeMapComponent(std::shared_ptr<Entity> _owner, const std::string& _diffuse = "", const std::string& _specular = "")
		: Component(_owner), mDiffuse(_diffuse), mSpecular(_specular)
	{
		mpModel = std::make_shared<ModelBuffer>();
	}

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<CubeMapComponent>(*this);
	}

public:
	std::shared_ptr<ModelBuffer> mpModel;
	std::string mDiffuse;
	std::string mSpecular;

	VSConstantBufferData mVSCBD;
};