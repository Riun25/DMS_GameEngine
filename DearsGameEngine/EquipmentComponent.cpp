#include "EquipmentComponent.h"

EquipmentComponent::~EquipmentComponent()
{
	m_pEquipments.clear();
}


void EquipmentComponent::AddEquipment(const std::string& _fileName /*= ""*/
	, const std::string& _meshName /*= "" */, const std::string& _targetName /*= ""*/
	, const std::string& _boneName /*= "" */, const std::string& _diffuse /*= ""*/
	, const std::string& _normal /*= ""*/)
{
	std::shared_ptr<Equipment> equipment = std::make_shared<Equipment>();
	equipment->m_pModelBuffer = std::make_shared<ModelBuffer>();
	equipment->m_modelFileName = _fileName;
	equipment->m_meshName = _meshName;
	equipment->m_targetModelName = _targetName;
	equipment->m_boneName = _boneName;
	equipment->m_diffuseTexture = _diffuse;
	equipment->m_normalTexture = _normal;

	m_pEquipments.push_back(equipment);
}
