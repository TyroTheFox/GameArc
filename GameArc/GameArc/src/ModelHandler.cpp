#include "../Engine/ModelHandler.h"
ModelHandler::ModelHandler() {}

ModelHandler::~ModelHandler() {
	modelList.clear();
}

void ModelHandler::registerModel(std::string fileName, Model* m) {
	modelList[fileName] = std::unique_ptr<Model>(m);
}
bool ModelHandler::checkForModel(std::string fileName) {
	return modelList[fileName] != nullptr;
}
Model& ModelHandler::getModel(std::string fileName) {
	return *modelList[fileName];
}