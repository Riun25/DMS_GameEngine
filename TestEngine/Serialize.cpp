#include "Reflection.h"
#include "framework.h"

/// <summary>
/// 기본적인 리플렉션 기능과 nlohmann json 라이브러리의 리플렉션 기능을 사용해서
/// 직렬화/역직렬화 테스트하는 코드입니다.
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
	// World객체 생성, 소멸, 오브젝트를 월드에 바인딩, 월드 체인저
};

class ObjectManager
{
	// GameObject 생성, 소멸, 컴포넌트 생성 및 바인딩
};

class Factory
{
	// 객체들 생성
};

struct World
{
	int id;
	std::string name;
	std::unordered_map<int, GameObject*> m_objects;
	REFLECT()
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(World, name, id)
};

// GameObject 구조체 정의
struct GameObject {
	std::string name;
	int id;

	std::unordered_map<std::type_index, Component*> m_pComponents;
	REFLECT()
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(GameObject, name, id)

};

// Component 기본 구조체 정의
struct Component {
	std::string m_typeName;
};

// Transform 구조체 정의, Component를 상속받음
struct Transform : public Component {
	Vector3 m_position;
	Vector3 m_rotation;
	Vector3 m_scale;
	REFLECT()
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Transform, m_typeName, m_position, m_rotation, m_scale)

};

// Render 구조체 정의, Component를 상속받음
struct MeshRenderer : public Component {
	std::string m_fileName;
	REFLECT()
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(MeshRenderer, m_typeName, m_fileName)

};

// BoxCollider 구조체 정의, Component를 상속받음
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

// 리플렉션을 위한 메타데이터 설정
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
	// GameObject 인스턴스 생성
	GameObject go;
	go.name = "Player";
	go.id = 1;

	// Transform 컴포넌트 생성 및 초기화
	auto* transform = new Transform();
	transform->m_typeName = "Transform";
	transform->m_position = Vector3{ 1.0, 0.0, 0.0 };
	transform->m_rotation = Vector3{ 0.0, 1.0, 0.0 };
	transform->m_scale = Vector3{ 1.0, 1.0, 1.0 };

	// Render 컴포넌트 생성 및 초기화
	auto* render = new MeshRenderer();
	render->m_typeName = "Render";
	render->m_fileName = "player_model.obj";

	// BoxCollider 컴포넌트 생성 및 초기화
	auto* boxCollider = new BoxCollider();
	boxCollider->m_typeName = "BoxCollider";
	boxCollider->m_isTrigger = true;
	boxCollider->m_center = Vector3{ 0.0, 0.0, 0.0 };
	boxCollider->m_size = Vector3{ 2.0, 2.0, 2.0 };

	// 컴포넌트를 GameObject에 추가
	go.m_pComponents[type_index(typeid(Transform))] = transform;
	go.m_pComponents[type_index(typeid(MeshRenderer))] = render;
	go.m_pComponents[type_index(typeid(BoxCollider))] = boxCollider;

	// GameObject의 정보 출력
	cout << "GameObject: " << go.name << " (ID: " << go.id << ")" << endl;
	for (auto& pair : go.m_pComponents) {
		Component* comp = pair.second;
		cout << "Component Type: " << comp->m_typeName << endl;
// 		comp->dump;  // dump 함수는 컴포넌트의 상세 정보를 출력하는 리플렉션 함수입니다.
	}

	// 동적으로 할당된 메모리 해제
	delete transform;
	delete render;
	delete boxCollider;

	return 0;
}