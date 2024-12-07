#include "framework.h"

struct Component
{
	virtual ~Component() = default;

	virtual void DoSomething() = 0;
	virtual std::shared_ptr<Component> Clone() const = 0;
};

struct Transform : public Component
{
	Vector3 pos;
	virtual void DoSomething() override
	{
		std::cout << "Transform" << std::endl;
	}

	virtual std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<Transform>(*this);
	}

};

struct Object
{
	std::shared_ptr<Object> Clone() const
	{
		return std::make_shared<Object>(*this);
	}

	void AddComponent(std::shared_ptr<Component> _component)
	{
		if (m_components.find(typeid(_component)) == m_components.end())
		{
			m_components[typeid(_component)] = (_component);
		}
	}

	template <typename T>
	std::shared_ptr<T> GetComponent() const
	{
		auto it = m_components.find(typeid(T));
		if (it != m_components.end())
		{
			return std::dynamic_pointer_cast<T>(it->second);
		}
		return nullptr;
	}

	std::unordered_map<std::type_index, std::shared_ptr<Component>> m_components;
};



int main()
{
	auto trs = std::make_shared<Transform>();
	trs->pos = Vector3(1, 2, 3);

	auto trs2 = std::dynamic_pointer_cast<Transform>(trs->Clone());
	trs2->pos = Vector3(4, 5, 6);

	auto obj = std::make_shared<Object>();
	obj->AddComponent(trs2);

	auto objtrs = obj->GetComponent<Transform>();
	objtrs->pos = Vector3(7, 8, 9);

	auto obj2 = obj->Clone();
	obj2->AddComponent(trs);


	return 0;
}