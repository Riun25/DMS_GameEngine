#pragma once
#include "framework.h"
#include "IComponent.h"

// class Entity
// {
// public:
// 	Entity() {}
// 	~Entity()
// 	{
// 		RemoveAll();
// 	}
// 
// 	uint64_t ID;		// Entity�� ID // EntityID=30ull; Version=24ull; WorldID=10ull;
// 
// 	template <typename T>
// 	ComponentHandle<T> Get()
// 	{
// 		auto found = components.find(std::type_index(typeid(T)));
// 		if (found != components.end())
// 		{
// 			return ComponentHandle<T>(&reinterpret_cast<T*>(found->second));
// 		}
// 	}
// 
// 	template <typename T>
// 	T* Add()
// 	{
// 		auto comp = components.find(std::type_index(typeid(T)));
// 		if (comp != components.end())
// 		{
// 			return comp->second;
// 		}
// 		else
// 		{
// 			T* component = new T();
// 			components.insert({ std::type_index(typeid(T)), component });
// 			return component;
// 		}
// 
// 	}
// 
// 	// Entity�� Ư�� Component�� ������ �ִ���
// 	template <typename T>
// 	bool Has() const
// 	{
// 		auto index = std::type_index(typeid(T));
// 		return components.find(index) != components.end();
// 	}
// 
// 	// Entity�� Ư�� Component���� ������ �ִ���
// 	template <typename T, typename V, typename... Types>
// 	bool Has() const
// 	{
// 		return Has<T>() && Has<V, Types...>();
// 	}
// 
// 	template <typename T>
// 	void Remove()
// 	{
// 		auto comp = components.find(std::type_index(typeid(T)));
// 		if (comp != components.end())
// 		{
// 			comp->second->Remove(this);
// 			comp->second->Destroy(world);
// 
// 			components.erase(comp);
// 		}
// 	}
// 
// 	void RemoveAll()
// 	{
// 		for (auto pair : components)
// 		{
// 			pair.second->Remove(this);
// 			pair.second->Destroy(world);
// 		}
// 
// 		components.clear();
// 	}
// 
// 	std::unordered_map<std::type_index, Component*> components;
// 	World* world;
// };
// 
