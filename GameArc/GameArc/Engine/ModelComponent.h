#pragma once
#include "Component.h"
#include "Model.h"
#include "ModelHandler.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

/**
\file ModelComponent.h
\author Kieran Clare
\brief Handles model loading and rendering

Handles loading models, rendering and registering them with the model handler
*/

class ModelComponent : public Component
{
public:
	///Contained model
	Model* model;
	///Model Handler object pointer
	ModelHandler* modelHandler;
	///Constructor
	ModelComponent() {}
	///Constructor
	ModelComponent(ModelHandler* mH) : modelHandler(mH) {}
	///Constructor
	ModelComponent(Model* m, ModelHandler* mH) : model(m), modelHandler(mH) {
		model->modelHandler = mH;
	}
	///Called when added to object, sets up debug functions
	void OnSetUp() override {
		debug->AddConsoleCommand("changeModel", TextParser::InterpFunc([this](std::vector<std::string> s) {
			if (s.size() <= 0) { this->debug->WriteErrorToConsole("Missing Values"); return; }
			if (this->parent->name == s.at(0)) {
				if (s.size() > 1) {
					model = new Model(s.at(1), modelHandler);
					this->debug->WriteToConsole("Model " + s.at(0) + " changed to " + s.at(1));
				}
				else {
					if (s.size() <= 0) { this->debug->WriteErrorToConsole("Need a model directory"); }
				}
			}
		}));
	}
	///Called on update tick
	void OnUpdate(float dt) override {}
	///Called on render call, draws model to screen
	void OnRender(IEngineCore* m_engineInterfacePtr) override {
		if (parent->getComponent<ModelComponent>() == nullptr && parent->getComponent<TransformComponent>() == nullptr) return;
		if (parent->getComponent<ModelComponent>()->active) {
			m_engineInterfacePtr->drawModel(model, parent->getComponent<TransformComponent>()->getModelMatrix());
		}
	}
	///Called on event call, doesn't do anything yet
	void OnMessage(const std::string m) override {}
	///Builds component from JSON values
	void BuildFromJson(const Json::Value& componentJSON) override {
		try {
			string nMFile;
			if (componentJSON.isMember("fileName")) {
				const Json::Value& fileName = componentJSON["fileName"];
				model = new Model(fileName.asCString(), modelHandler);
				if (nMFile != "") {
					unsigned int textureID = model->TextureFromFile(nMFile.c_str(), model->GetDirectory());
				}
			}
			if (componentJSON.isMember("active")) {
				const Json::Value& a = componentJSON["active"];
				active = a.asBool();
			}
			if (componentJSON.isMember("normalMap")) {
				const Json::Value& map = componentJSON["normalMap"];
				nMFile = map.asCString();
				if (model != nullptr) {
					unsigned int textureID = model->TextureFromFile(nMFile.c_str(), model->GetDirectory());

				}
			}
		}
		catch (...) {
			std::cout << "Exception thrown in parsing BuildFromJson in ModelComponent." << std::endl;
			throw;
		}
	}
};