#include "Reflection.h"
#include "framework.h"

/// <summary>
/// �⺻���� ���÷��� ��ɰ� nlohmann json ���̺귯���� ���÷��� ����� ����ؼ�
/// ����ȭ/������ȭ �׽�Ʈ�ϴ� �ڵ��Դϴ�.
/// </summary>

using json = nlohmann::json;

namespace nlohmann {
	template <>
	struct adl_serializer<DirectX::SimpleMath::Vector3>
	{
		static void to_json(json& j, const DirectX::SimpleMath::Vector3& v)
		{
			j = json{ {"x", v.x}, {"y", v.y}, {"z", v.z} };
		}

		static void from_json(const json& j, DirectX::SimpleMath::Vector3& v)
		{
			j.at("x").get_to(v.x);
			j.at("y").get_to(v.y);
			j.at("z").get_to(v.z);
		}
	};
}

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(DirectX::SimpleMath::Vector3, x, y, z)



struct Component;
struct GameObject;
struct World;
class Factory;

class WorldManager
{
	// World��ü ����, �Ҹ�, ������Ʈ�� ���忡 ���ε�, ���� ü����
};

class ObjectManager
{
	// GameObject ����, �Ҹ�, ������Ʈ ���� �� ���ε�
};

class Factory
{
	// ��ü�� ����
};

struct World
{
	int id;
	std::string name;
	std::unordered_map<int, GameObject*> m_objects;
	REFLECT()
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(World, name, id)
};

// GameObject ����ü ����
struct GameObject {
	std::string name;
	int id;

	std::unordered_map<std::type_index, Component*> m_pComponents;
	REFLECT()
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(GameObject, name, id)

};

// Component �⺻ ����ü ����
struct Component {
	std::string m_typeName;
};

// Transform ����ü ����, Component�� ��ӹ���
struct Transform : public Component {
	Vector3 m_position;
	Vector3 m_rotation;
	Vector3 m_scale;
	REFLECT()
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Transform, m_typeName, m_position, m_rotation, m_scale)

};

// Render ����ü ����, Component�� ��ӹ���
struct MeshRenderer : public Component {
	std::string m_fileName;
	REFLECT()
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(MeshRenderer, m_typeName, m_fileName)

};

// BoxCollider ����ü ����, Component�� ��ӹ���
struct BoxCollider : public Component {
	bool m_isTrigger;
	Vector3 m_center;
	Vector3 m_size;
	REFLECT()
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(BoxCollider, m_typeName, m_isTrigger, m_center, m_size)

};


REFLECT_STRUCT_BEGIN(World)
REFLECT_STRUCT_MEMBER(name)
REFLECT_STRUCT_MEMBER(id)
REFLECT_STRUCT_END()

// ���÷����� ���� ��Ÿ������ ����
REFLECT_STRUCT_BEGIN(GameObject)
REFLECT_STRUCT_MEMBER(name)
REFLECT_STRUCT_MEMBER(id)
REFLECT_STRUCT_END()

REFLECT_STRUCT_BEGIN(Transform)
REFLECT_STRUCT_MEMBER(m_typeName)
REFLECT_STRUCT_MEMBER(m_position)
REFLECT_STRUCT_MEMBER(m_rotation)
REFLECT_STRUCT_MEMBER(m_scale)
REFLECT_STRUCT_END()

REFLECT_STRUCT_BEGIN(MeshRenderer)
REFLECT_STRUCT_MEMBER(m_typeName)
REFLECT_STRUCT_MEMBER(m_fileName)
REFLECT_STRUCT_END()

REFLECT_STRUCT_BEGIN(BoxCollider)
REFLECT_STRUCT_MEMBER(m_typeName)
REFLECT_STRUCT_MEMBER(m_isTrigger)
REFLECT_STRUCT_MEMBER(m_center)
REFLECT_STRUCT_MEMBER(m_size)
REFLECT_STRUCT_END()


class ObjectFactory
{
public:
	ObjectFactory()
	{
		RegisterComponent<Transform>("Transform");
		RegisterComponent<BoxCollider>("BoxCollider");
		RegisterComponent<MeshRenderer>("Render");
	}

	template <typename T>
	void RegisterComponent(const std::string& _name)
	{
		m_builders[_name] = []() -> std::shared_ptr<Component> {return std::make_shared<T>(); };
	}

	std::shared_ptr<Component> CreateComponent(const std::string& _name)
	{
		auto it = m_builders.find(_name);
		if (it != m_builders.end())
		{
			return it->second();
		}
		std::cerr << "Component type not registered: " << _name << std::endl;
		return nullptr;
	}

private:
	std::unordered_map<std::string, std::function<std::shared_ptr<Component>()>> m_builders;

};


int main()
{
	// GameObject �ν��Ͻ� ����
	GameObject go;
	go.name = "Player";
	go.id = 1;

	// Transform ������Ʈ ���� �� �ʱ�ȭ
	auto* transform = new Transform();
	transform->m_typeName = "Transform";
	transform->m_position = Vector3{ 1.0, 0.0, 0.0 };
	transform->m_rotation = Vector3{ 0.0, 1.0, 0.0 };
	transform->m_scale = Vector3{ 1.0, 1.0, 1.0 };

	// Render ������Ʈ ���� �� �ʱ�ȭ
	auto* render = new MeshRenderer();
	render->m_typeName = "Render";
	render->m_fileName = "player_model.obj";

	// BoxCollider ������Ʈ ���� �� �ʱ�ȭ
	auto* boxCollider = new BoxCollider();
	boxCollider->m_typeName = "BoxCollider";
	boxCollider->m_isTrigger = true;
	boxCollider->m_center = Vector3{ 0.0, 0.0, 0.0 };
	boxCollider->m_size = Vector3{ 2.0, 2.0, 2.0 };

	// ������Ʈ�� GameObject�� �߰�
	go.m_pComponents[type_index(typeid(Transform))] = transform;
	go.m_pComponents[type_index(typeid(MeshRenderer))] = render;
	go.m_pComponents[type_index(typeid(BoxCollider))] = boxCollider;

	// GameObject�� ���� ���
	cout << "GameObject: " << go.name << " (ID: " << go.id << ")" << endl;
	for (auto& pair : go.m_pComponents) {
		Component* comp = pair.second;
		cout << "Component Type: " << comp->m_typeName << endl;
// 		comp->dump;  // dump �Լ��� ������Ʈ�� �� ������ ����ϴ� ���÷��� �Լ��Դϴ�.
	}

	// �������� �Ҵ�� �޸� ����
	delete transform;
	delete render;
	delete boxCollider;

	return 0;
}