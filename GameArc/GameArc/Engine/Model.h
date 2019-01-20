#pragma once
#include <string>
#include <vector>
#include "Mesh.h"
#include "global.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

class ModelHandler;
extern ModelHandler* modelHandler;

class Model
{
public:
	bool textureLoaded = false;
	bool loadedFromHandler = false;
	Model();
	Model(string filepath);

	void render(const unsigned int shaderProgram);

	static unsigned int TextureFromFile(const char* filepath, const string& directory, bool gamma = false);
	void loadModel(string path);
	std::vector<Mesh> getMeshes();
	inline int GetTextureSize() { return v_textures.size(); }
private:

	std::vector<Mesh> v_meshes;
	string directory;
	vector<Texture> v_textures;

	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
	                                     std::string typeName);
};
