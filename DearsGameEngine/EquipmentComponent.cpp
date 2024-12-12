#include "EquipmentComponent.h"

EquipmentComponent::~EquipmentComponent()
{
	mpEquipments.clear();
}


void EquipmentComponent::AddEquipment(const std::string& _fileName /*= ""*/
	, const std::string& _meshName /*= "" */, const std::string& _targetName /*= ""*/
	, const std::string& _boneName /*= "" */, const std::string& _diffuse /*= ""*/
	, const std::string& _normal /*= ""*/)
{
	std::shared_ptr<Equipment> equipment = std::make_shared<Equipment>();
	equipment->mpModelBuffer = std::make_shared<ModelBuffer>();
	equipment->mModelFileName = _fileName;
	equipment->mMeshName = _meshName;
	equipment->mTargetModelName = _targetName;
	equipment->mBoneName = _boneName;
	equipment->mDiffuseTexture = _diffuse;
	equipment->mNormalTexture = _normal;

	mpEquipments.push_back(equipment);
}
