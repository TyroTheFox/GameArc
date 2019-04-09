#pragma once
#include "Mesh.h"

using namespace std;

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	// initialise the mesh data within vertex buffers
	computeTangents();
	setupMesh();
}

// render the mesh with a given shader program
void Mesh::render(const unsigned int shaderProgram)
{
	// bind the appropriate textures
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glUniform1i(glGetUniformLocation(shaderProgram, "normalMapped"), 0);
		glUniform1i(glGetUniformLocation(shaderProgram, "heightMapped"), 0);
		string number;
		string name = textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);
		else if (name == "texture_normal") {
			number = std::to_string(normalNr++);
			glUniform1i(glGetUniformLocation(shaderProgram, "normalMapped"), 1);
		}
		else if (name == "texture_height") {
			number = std::to_string(heightNr++);
			glUniform1i(glGetUniformLocation(shaderProgram, "heightMapped"), 1);
		}

		// now set the sampler to the correct texture unit
		glUniform1i(glGetUniformLocation(shaderProgram, (name + number).c_str()), i);
		// and finally bind the texture
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}


	// set the model component of our shader to the cube model
	//glUniformMatrix4fv(glGetUniformLocation(m_defaultShaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));


	// draw mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	for (unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	// return to default texture
	//glActiveTexture(GL_TEXTURE0);
}

void Mesh::renderWithoutTextures(const unsigned int shaderProgram)
{
	// draw mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

// initialises all the buffer arrays
void Mesh::setupMesh()
{
	// create buffers/arrays
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	// load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// set the memory address as the first vertex struct
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// set the vertex attribute pointers
	// positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	// texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureCoords));
	// texture coords
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
	// texture coords
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));
	glBindVertexArray(0);

}

void Mesh::computeTangents() {
	for (unsigned int i = 0; i < indices.size(); i +=3) {
		// Shortcuts for vertices
		int index0 = indices[i];
		int index1 = indices[i + 1];
		int index2 = indices[i + 2];

		Vertex& v0 = vertices[index0];
		Vertex& v1 = vertices[index1];
		Vertex& v2 = vertices[index2];

		// Edges of the triangle : postion delta
		glm::vec3 deltaPos1 = v1.position - v0.position;
		glm::vec3 deltaPos2 = v2.position - v0.position;

		// UV delta
		glm::vec2 deltaUV1 = v1.textureCoords - v0.textureCoords;
		glm::vec2 deltaUV2 = v2.textureCoords - v0.textureCoords;

		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
		glm::vec3 tangent; 
		tangent.x = (deltaUV2.y * deltaPos1.x - deltaUV1.y * deltaPos2.x)*r;
		tangent.y = (deltaUV2.y * deltaPos1.y - deltaUV1.y * deltaPos2.y)*r;
		tangent.z = (deltaUV2.y * deltaPos1.z - deltaUV1.y * deltaPos2.z)*r;
		v0.tangent = glm::normalize(tangent);
		v1.tangent = glm::normalize(tangent);
		v2.tangent = glm::normalize(tangent);
		glm::vec3 bitangent;
		bitangent.x = (-deltaUV2.x * deltaPos1.x - deltaUV1.x * deltaPos2.x)*r;
		bitangent.y = (-deltaUV2.x * deltaPos1.y - deltaUV1.x * deltaPos2.y)*r;
		bitangent.z = (-deltaUV2.x * deltaPos1.z - deltaUV1.x * deltaPos2.z)*r;
		v0.bitangent = glm::normalize(bitangent);
		v1.bitangent = glm::normalize(bitangent);
		v2.bitangent = glm::normalize(bitangent);
	}
}
