#pragma once
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
#include "2DRect.h"
/// FreeType
#include <ft2build.h>
#include FT_FREETYPE_H

class IEngineCore;///Forward declared
class Rect;

/** \struct Character
* 
* Holds all state information relevant to a character as loaded using FreeType
*/
struct Character {
	/// ID handle of the glyph texture
    GLuint TextureID;   
	/// Size of glyph
    glm::ivec2 Size;   
	/// Offset from baseline to left/top of glyph
    glm::ivec2 Bearing;  
	/// Horizontal offset to advance to next glyph
    GLuint Advance;    
};

/**\class TextWriter
* \file TextWriter.h
* \author Kieran Clare
* \brief Used for writing 2D text to the screen
*
* Based on the Learn Open GL version, draws 2D text to the screen
* Orignal version: https://learnopengl.com/code_viewer.php?code=in-practice/text_rendering
*/
class TextWriter {
public:
	///Engine Core Pointer, used for drawing
	IEngineCore * m_engineInterfacePtr;
	///2D Background Object
	Rect* background;
	///Constructor
	TextWriter();
	///Constructor
	TextWriter(IEngineCore* m_engineInterfacePtr);
	void init();
	///Draws normal, white text to the screen
	void DrawNormalText(std::string text, float x, float y, float scale);
	///Draws red text to the screen
	void DrawErrorText(std::string text, float x, float y, float scale);
	///Sets the engine pointer for the object
	void SetEnginePtr(IEngineCore * enginePtr);
private:
	///A store of all characters 
	std::map<GLchar, Character> Characters;
	///Vertex Array and Buffer Objects
	GLuint VAO, VBO;
	///Draws given text to screen at given position, at a given size, with a given colour
	void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
};
