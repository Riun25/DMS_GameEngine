#pragma once
#include "Component.h"
#include "GraphicsDefine.h"

/// <summary>
/// ĳ���Ͱ� �����ϴ� ��� �ϳ��� �ش��ϴ� ��ü
/// </summary>
struct Equipment
{
	std::string mModelFileName;	// �� ���� �̸�
	std::string mMeshName;			// �޽� �̸�
	std::string mDiffuseTexture;	// ��ǻ�� �ؽ��� ���ϸ�
	std::string mNormalTexture;	// �븻 �ؽ��� ���ϸ�
	std::string mTargetModelName;	// ������ų ���� �̸�
	std::string mBoneName;			// ���� �̸�
	std::shared_ptr<ModelBuffer> mpModelBuffer;	// �� ����
	int mTargetBoneIndex;			// �� �ε���
	VSTargetBoneConstantBufferData mTargetBoneConstantBufferData;	// �� ��Ʈ����
};

/// <summary>
/// ������ ���͸� �������ִ� ������Ʈ
/// ��� �������� �Ҷ� ����
/// �浹ü�� �ʿ��� ���� ������ ��ƼƼ�� ������Ʈ�� �̿��� ����
/// </summary>
class EquipmentComponent :
	public Component
{
public:
	EquipmentComponent(std::shared_ptr<Entity> _owner)
		: Component(_owner)
	{}

	~EquipmentComponent();

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<EquipmentComponent>(*this);
	}

	void AddEquipment(const std::string& _fileName = "", const std::string& _meshName = ""
		, const std::string& _targetName = "", const std::string& _boneName = ""
		, const std::string& _diffuse = "", const std::string& _normal = "");

public:
	std::vector<std::shared_ptr<Equipment>> mpEquipments;	// ������ ����

};

// ĳ������ meshrenderer������Ʈ�� ��ü�� ������ ���� ��
// ����, ���� ������� ������ �ִ� ��ü
// VSConstantBufferData 

// ĳ������ bonegroup������Ʈ�� ��ü�� ������ ���� ��
// �� ��Ʈ�������� ���
// VSBoneConstantBufferData* 
