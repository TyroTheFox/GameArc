#pragma once

#include <glad/glad.h> 

#include <assimp/Importer.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>

using namespace std;
/** \struct Vertex
Used for mesh geometry data
*/
struct Vertex 
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 textureCoords;
};
/** \struct Texture
Used for texture information
*/
struct Texture {
	unsigned int id;
	string type;
	aiString filepath;
};
/** \class Mesh
\file Mesh.h
\author Kieran Clare
\brief Contains information for one part of a model

Contains the information for one part of a full model, including texture information and GPU-Needed objects
*/
class Mesh 
{
public:
	///Stored verticies
	vector<Vertex> vertices;
	///Order object's verticies are drawn in
	vector<unsigned int> indices;
	///Mesh's texture information
	vector<Texture> textures;
	///Vertex Array Object
	unsigned int VAO;
	///Constructor
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
	///Draws mesh to screen
	void render(const unsigned int shaderProgram);
	void renderWithoutTextures(const unsigned int shaderProgram);
private:
	///Buffer objects
	unsigned int VBO, EBO;
	///Initialises all the buffer arrays
	void setupMesh();
};