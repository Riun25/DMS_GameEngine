#pragma once
#include "Component.h"
#include "Graphics/ModelInfo.h"

struct WeaponComponent
	: public Component
{
public:
	WeaponComponent(std::shared_ptr<Entity> _owner
	/*, const std::string& _targetModel = "", const std::string& _targetBone = "", float _scale = 1.f*/)
		:Component(_owner), mpAttachedEntity(nullptr)/*, m_targetModel(_targetModel), m_targetBone(_targetBone), m_scale(_scale)*/
	{
		mpModel = std::make_shared<ModelBuffer>();
	}

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<WeaponComponent>(*this);
	}

public:
	Entity* mpAttachedEntity;	// ������ ��ƼƼ
	std::shared_ptr<ModelBuffer> mpModel; // ������ �� ����
	AABB mWeaponAABB;
	//std::string m_targetModel;
	//std::string m_targetBone;
	std::string mFileName;
	std::string mMeshName;
	float mScale = 1.f;
	int mBoneIndex;	//���� �ε����� ������ �ְԲ�.
	Matrix mWeaponBoneMatrix;				// �ݶ��̴��� ��Ʈ����
	VSConstantBufferData mWeaponConstantBufferData;	// ���� �޽��� ������Ʈ ���� ������
};