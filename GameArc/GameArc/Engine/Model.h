#pragma once
#include <string>
#include <vector>
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include "ModelHandler.h"

class ModelHandler;

/** \class Model
\file Model.h
\author Kieran Clare
\brief Contains all model meshes and texture information and loads that information from file

Processes and loads models from file as well as handles and builds the meshes of the model, allowing for easy rendering
*/

class Model
{
public:
	///Model Handler object pointer
	ModelHandler* modelHandler;
	///Flag to show a texture was loaded
	bool textureLoaded = false;
	///Flag to show the model was loaded from the Model Handler
	bool loadedFromHandler = false;
	///Constructor
	Model();
	///Constructor
	Model(ModelHandler* mH);
	///Constructor
	Model(string filepath, ModelHandler* mH);
	///Draws each mesh to the screen
	void render(const unsigned int shaderProgram);
	///Finds texture information from the model file
	static unsigned int TextureFromFile(const char* filepath, const string& directory, bool gamma = false);
	///Loads in model information from file
	void loadModel(string path);
	///Returns all meshes for the file
	std::vector<Mesh> getMeshes();
	///Returns number of textures loaded
	inline int GetTextureSize() { return v_textures.size(); }
private:
	///Stored mesh information
	std::vector<Mesh> v_meshes;
	///Relative file directory of model file
	string directory;
	///Stored texture information
	vector<Texture> v_textures;
	///Turns loded model scenes into meshes
	void processNode(aiNode* node, const aiScene* scene);
	///Turns loaded meshes into stored meshes and textures
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	///Finds and loads material textures
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName); 
};
