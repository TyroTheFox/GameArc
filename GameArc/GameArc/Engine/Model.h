#pragma once
/**
* \class Model
* \file Model.h
* \author Kieran Clare
* \brief Contains all model meshes and texture information and loads that information from file
*
* Processes and loads models from file as well as handles and builds the meshes of the model, allowing for easy rendering
*/
#include <string>
#include <vector>
#include "Mesh.h"
#include "global.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

class ModelHandler;///Forward declared
extern ModelHandler* modelHandler;///External Model Handler object

class Model
{
public:
	bool textureLoaded = false;///Flag to show a texture was loaded
	bool loadedFromHandler = false;///Flag to show the model was loaded from the Model Handler
	Model();///Constructor
	Model(string filepath);///Constructor

	void render(const unsigned int shaderProgram);///Draws each mesh to the screen

	static unsigned int TextureFromFile(const char* filepath, const string& directory, bool gamma = false);///Finds texture information from the model file
	void loadModel(string path);///Loads in model information from file
	std::vector<Mesh> getMeshes();///Returns all meshes for the file
	inline int GetTextureSize() { return v_textures.size(); }///Returns number of textures loaded
private:

	std::vector<Mesh> v_meshes;///Stored mesh information
	string directory;///Relative file directory of model file
	vector<Texture> v_textures;///Stored texture information

	void processNode(aiNode* node, const aiScene* scene);///Turns loded model scenes into meshes
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);///Turns loaded meshes into stored meshes and textures
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName); ///Finds and loads material textures
};
