#pragma once
#include "Component.h"
#include "IEngineCore.h"
#include <unordered_map>
#include <typeindex>

class IEngineCore;

class GameObject
{
public:
	template <typename T>
	T* getComponent()
	{

		auto iter = m_components.find(typeid(T));

		if(iter != std::end(m_components))
		{
			// if found dynamic cast the component pointer and return it
			return dynamic_cast<T*>(iter->second);
		}
		
		// return null if we don't have a component of that type
		return nullptr;
		
	}

	template <typename T>
	void addComponent(T* comp)
	{
		// add the component to unoreder map with hash of its typeid
		comp->parent = this;
		comp->OnSetUp();
		m_components[typeid(T)] = comp;
	}

	void updateAllComponents(float dt) {

		for (auto it : m_components)
		{
			// if found dynamic cast the component pointer and return it
			it.second->OnUpdate(dt);
		}
	}

	void renderAllComponents(IEngineCore* m_engineInterfacePtr) {

		for (auto it : m_components)
		{
			// if found dynamic cast the component pointer and return it
			it.second->OnRender(m_engineInterfacePtr);
		}
	}
	
private:
	std::unordered_map<std::type_index, Component*> m_components;
};
