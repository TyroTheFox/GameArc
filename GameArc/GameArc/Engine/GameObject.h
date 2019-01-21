#pragma once
/**
* \class Game Object
* \file Game Object.h
* \brief A container for components that represents an object within the game
*
* An amorpheous blob that components are attached to in order to grant it function
*/
#include "Component.h"
#include "IEngineCore.h"
#include <unordered_map>
#include <typeindex>

class IEngineCore;///Forward declared

class GameObject
{
public:
	std::string name = "object";///Object name
	template <typename T>///Template
	T* getComponent()///Returns object of desired type
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
	void addComponent(T* comp)///Adds component of desired type
	{
		// add the component to unoreder map with hash of its typeid
		comp->parent = this;
		comp->OnSetUp();
		m_components[typeid(T)] = comp;
	}

	void updateAllComponents(float dt) {///Fires update for all components

		for (auto it : m_components)
		{
			// if found dynamic cast the component pointer and return it
			it.second->OnUpdate(dt);
		}
	}

	void renderAllComponents(IEngineCore* m_engineInterfacePtr) {///Fires render for all components

		for (auto it : m_components)
		{
			// if found dynamic cast the component pointer and return it
			it.second->OnRender(m_engineInterfacePtr);
		}
	}
	
private:
	std::unordered_map<std::type_index, Component*> m_components;///List of attached components
};
