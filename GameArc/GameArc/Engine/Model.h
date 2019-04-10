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

struct ModelColour {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
	float mapHeight;
	ModelColour() : ambient(glm::vec3(0.3f)), diffuse(glm::vec3(0.1f)), specular(glm::vec3(0.8f)), shininess(32), mapHeight(0.01f) {};
	ModelColour(glm::vec3 a, glm::vec3 d, glm::vec3 s, float sh) : ambient(a), diffuse(d), specular(s), shininess(sh), mapHeight(0.01f) {};
	ModelColour(glm::vec3 a, glm::vec3 d, glm::vec3 s, float sh, float mH) : ambient(a), diffuse(d), specular(s), shininess(sh), mapHeight(mH) {};
};

class Model
{
public:
	///Model Handler object pointer
	ModelHandler* modelHandler;
	ModelColour modelColour;
	///Flag to show a texture was loaded
	bool textureLoaded = false;
	///Flag to show the model was loaded from the Model Handler
	bool loadedFromHandler = false;
	///Constructor
	Model();
	///Constructor
	Model(ModelHandler* mH);
	///Constructor
	Model(std::string filepath, ModelHandler* mH);
	///Draws each mesh to the screen
	void render(const unsigned int shaderProgram);
	void renderWithoutTextures(const unsigned int shaderProgram);
	///Finds texture information from the model file
	static unsigned int TextureFromFile(const char* filepath, const string& directory, bool gamma = false);
	///Loads in model information from file
	void loadModel(string path);
	///Returns all meshes for the file
	std::vector<Mesh> getMeshes();
	///Returns number of textures loaded
	inline int GetTextureSize() { return v_textures.size(); }
	inline string GetDirectory() { return directory; }
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
