#pragma once
// Std. Includes
#include <iostream>
#include <map>
#include <string>
// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// GL includes
#include "Shader.h"
// FreeType
#include <ft2build.h>
#include FT_FREETYPE_H

/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
    GLuint TextureID;   // ID handle of the glyph texture
    glm::ivec2 Size;    // Size of glyph
    glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
    GLuint Advance;    // Horizontal offset to advance to next glyph
};

class TextWriter {
public:
	Shader* shader;
	TextWriter(GLfloat WIDTH, GLfloat HEIGHT);
	void SetWindowSize(float WIDTH, float HEIGHT);
	void DrawNormalText(std::string text, float x, float y, float scale);
	void DrawErrorText(std::string text, float x, float y, float scale);
private:
	glm::mat4 projection;
	std::map<GLchar, Character> Characters;
	GLuint VAO, VBO;
	void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
};
