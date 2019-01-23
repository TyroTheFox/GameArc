#pragma once

#include "Component.h"
#include "IEngineCore.h"
#include <unordered_map>
#include <typeindex>
///Forward declared
class IEngineCore;
/** \class GameObject
\file Game Object.h
\brief A container for components that represents an object within the game

An amorpheous blob that components are attached to in order to grant it function
*/
class GameObject
{
public:
	///Object name
	std::string name = "object";
	///Template
	template <typename T>
	///Returns object of desired type
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
	///Adds component of desired type
	void addComponent(T* comp)
	{
		// add the component to unoreder map with hash of its typeid
		comp->parent = this;
		comp->OnSetUp();
		m_components[typeid(T)] = comp;
	}
	///Fires update for all components
	void updateAllComponents(float dt) {

		for (auto it : m_components)
		{
			// if found dynamic cast the component pointer and return it
			it.second->OnUpdate(dt);
		}
	}
	///Fires render for all components
	void renderAllComponents(IEngineCore* m_engineInterfacePtr) {

		for (auto it : m_components)
		{
			// if found dynamic cast the component pointer and return it
			it.second->OnRender(m_engineInterfacePtr);
		}
	}
	
private:
	///List of attached components
	std::unordered_map<std::type_index, Component*> m_components;
};
