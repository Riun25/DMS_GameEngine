#pragma once
#include "Component.h"

/// <summary>
/// 3D �ؽ��� ������Ʈ�Դϴ�.
/// <para >
/// Texture2D(string "��ǻ�� �ؽ���", string "�븻 �ؽ���");
/// </para>
/// </summary>
struct Texture3D
	:public Component
{
public:
	Texture3D(std::shared_ptr<Entity> _owner, const std::string& _diffuse = "", const std::string& _normal = "")
		: Component(_owner), mDiffuse(_diffuse), mNormal(_normal)
	{}

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<Texture3D>(*this);
	}

public:
	std::string mDiffuse;
	std::string mSpecular;
	std::string mNormal;

};