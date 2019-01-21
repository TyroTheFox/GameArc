#pragma once
/**
* \class Model Handler
* \file ModelHandler.h
* \author Kieran Clare
* \brief Handles and checks models that are loaded, preventing reloaded models
*
* Keeps a record of all registers models that have been loaded so far and allows model objects to load their variables from
* this object rather than from file
*/
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
	std::map<std::string, std::unique_ptr<Model>> modelList;///searchable list of all models currently loaded
public:
	ModelHandler();///Constructor
	void registerModel(std::string fileName, Model* m);///Registers new models to the list
	bool checkForModel(std::string fileName);///Checks whether model exists in list
	Model& getModel(std::string fileName);///Returns a given model
};