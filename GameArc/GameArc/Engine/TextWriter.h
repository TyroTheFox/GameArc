#pragma once
/**
* \class Text Writer
* \file TextWriter.h
* \author Kieran Clare
* \brief Used for writing 2D text to the screen
*
* Based on the Learn Open GL version, draws 2D text to the screen
* Orignal version: https://learnopengl.com/code_viewer.php?code=in-practice/text_rendering
*/
/// Std. Includes
#include <iostream>
#include <map>
#include <string>
/// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
/// GL includes
#include "Shader.h"
#include "IEngineCore.h"
/// FreeType
#include <ft2build.h>
#include FT_FREETYPE_H

class IEngineCore;///Forward declared

/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
    GLuint TextureID;   /// ID handle of the glyph texture
    glm::ivec2 Size;    /// Size of glyph
    glm::ivec2 Bearing;  /// Offset from baseline to left/top of glyph
    GLuint Advance;    /// Horizontal offset to advance to next glyph
};

class TextWriter {
public:
	IEngineCore * m_engineInterfacePtr;///Engine Core Pointer, used for drawing
	TextWriter();///Constructor
	void DrawNormalText(std::string text, float x, float y, float scale);///Draws normal, white text to the screen
	void DrawErrorText(std::string text, float x, float y, float scale);///Draws red text to the screen
	void SetEnginePtr(IEngineCore * enginePtr);///Sets the engine pointer for the object
private:
	std::map<GLchar, Character> Characters;///A store of all characters 
	GLuint VAO, VBO;///Vertex Array and Buffer Objects
	void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);///Draws given text to screen at given position, at a given size, with a given colour
};
