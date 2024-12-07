#include "framework.h"

/// <summary>
/// nlohmann json ���̺귯���� ���÷��� ����� ����ؼ� ����ȭ/������ȭ �׽�Ʈ�ϴ� �ڵ��Դϴ�.
/// </summary>

using json = nlohmann::json;

class Component;
class World;
class Object;

namespace nlohmann 
{
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

class Component
{
public:
	std::string type;
};

class Transform : Component
{
public:
	Vector3 pos;
	Vector3 rot;
	Vector3 scl;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Transform, pos, rot, scl)
};

class Collider : Component
{
public:
	Vector3 center;
	Vector3 size;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Collider, center, size)
};

class Object
{
public:
	int id;
	std::string name;
	std::unordered_map<std::string, std::shared_ptr<Component>> components;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Object, id, name)
};

class World
{
public:
	int id;
	std::string name;
	std::unordered_map<int, std::shared_ptr<Object>> objects;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(World, id, name)
};


// ����ȭ �Լ�
template <typename T>
json serialize(const T& obj) 
{
	json j;
	nlohmann::to_json(j, obj);
	return j;
}

// ������ȭ �Լ�
template <typename T>
T deserialize(const json& j) 
{
	T obj;
	nlohmann::from_json(j, obj);
	return obj;
}

int main() {
	// Object Ŭ������ ���� �ν��Ͻ� ���� �� ����ȭ
	Object myObject;
	myObject.id = 1;
	myObject.name = "Test Object";
	json objectJson = serialize(myObject);
	std::cout << "Serialized Object: " << objectJson << std::endl;

	// JSON���� Object Ŭ������ ������ȭ
	Object newObj = deserialize<Object>(objectJson);
	std::cout << "Deserialized Object ID: " << newObj.id << ", Name: " << newObj.name << std::endl;

	// World Ŭ������ ���� �ν��Ͻ� ���� �� ����ȭ
	World myWorld;
	myWorld.id = 2;
	myWorld.name = "Test World";
	json worldJson = serialize(myWorld);
	std::cout << "Serialized World: " << worldJson << std::endl;

	// JSON���� World Ŭ������ ������ȭ
	World newWorld = deserialize<World>(worldJson);
	std::cout << "Deserialized World ID: " << newWorld.id << ", Name: " << newWorld.name << std::endl;

	return 0;
}