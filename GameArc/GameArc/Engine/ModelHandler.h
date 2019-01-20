#pragma once
#include "Model.h"
#include <map>
#include <string>
#include <utility>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>  
#include <vector>
#include <memory>

class Model;

class ModelHandler
{
private:
	std::map<std::string, std::unique_ptr<Model>> modelList;
public:
	ModelHandler();
	void registerModel(std::string fileName, Model* m);
	bool checkForModel(std::string fileName);
	Model& getModel(std::string fileName);
};