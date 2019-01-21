#pragma once
/**
* \class Model Component
* \file ModelComponent.h
* \author Kieran Clare
* \brief Handles model loading and rendering
*
* Handles loading models, rendering and registering them with the model handler
*/
#include "Component.h"
#include "Model.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class ModelComponent : public Component
{
public:
	Model* model;///Contained model
	ModelComponent() {}///Constructor
	ModelComponent(Model* m) : model(m) {}///Constructor
	void OnSetUp() override {///Called when added to object, sets up debug functions
		debug->AddConsoleCommand("changemodel", TextParser::InterpFunc([this](std::vector<std::string> s) {
			if (s.size() <= 0) { this->debug->WriteToConsole("Missing Values"); return; }
			if (this->parent->name == s.at(0)) {
				if (s.size() > 1) {
					model = new Model(s.at(1));
					this->debug->WriteToConsole("Model " + s.at(0) + " changed to " + s.at(1));
				}
				else {
					if (s.size() <= 0) { this->debug->WriteToConsole("Need a model directory"); }
				}
			}
		}));
	}
	void OnUpdate(float dt) override {}///Called on update tick
	void OnRender(IEngineCore* m_engineInterfacePtr) override {///Called on render call, draws model to screen
		if (parent->getComponent<ModelComponent>() == nullptr && parent->getComponent<TransformComponent>() == nullptr) return;
		if (parent->getComponent<ModelComponent>()->active) {
			m_engineInterfacePtr->drawModel(model, parent->getComponent<TransformComponent>()->getModelMatrix());
		}
	}
	void OnMessage(const std::string m) override {}///Called on event call, doesn't do anything yet
	void BuildFromJson(const Json::Value& componentJSON) override {///Builds component from JSON values
		try {
			if (componentJSON.isMember("fileName")) {
				const Json::Value& fileName = componentJSON["fileName"];
				model = new Model(fileName.asCString());
			}
			/*if (componentJSON.isMember("textureFileName") && componentJSON.isMember("textureDirectory")) {
				const Json::Value& fileName = componentJSON["textureFileName"];
				const Json::Value& directory = componentJSON["textureDirectory"];
				if (model != nullptr) {
					model->TextureFromFile(fileName.asCString(), directory.asCString());
				}
			}*/
			if (componentJSON.isMember("active")) {
				const Json::Value& a = componentJSON["active"];
				active = a.asBool();
			}
		}
		catch (...) {
			std::cout << "Exception thrown in parsing BuildFromJson in ModelComponent." << std::endl;
			throw;
		}
	}
};