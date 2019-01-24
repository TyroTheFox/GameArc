#include "TextWriter.h"

TextWriter::TextWriter() {
	init();
}

TextWriter::TextWriter(IEngineCore* engineInterfacePtr) {
	m_engineInterfacePtr = engineInterfacePtr;
	init();
	background = new Rect(m_engineInterfacePtr);
}

void TextWriter::init() {
	// FreeType
	FT_Library ft;
	// All functions return a value different than 0 whenever an error occurred
	if (FT_Init_FreeType(&ft))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	// Load font as face
	FT_Face face;
	if (FT_New_Face(ft, "assets/fonts/GenericMobileSystem.ttf", 0, &face))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

	// Set size to load glyphs as
	FT_Set_Pixel_Sizes(face, 0, 48);

	// Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Load first 128 characters of ASCII set
	for (GLubyte c = 0; c < 128; c++)
	{
		// Load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		Characters.insert(std::pair<GLchar, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	// Destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);


	// Configure VAO/VBO for texture quads
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void TextWriter::DrawNormalText(std::string text, float x, float y, float scale) {
	RenderText(text, x, y, scale, glm::vec3(1.0f, 1.0f, 1.0f));
}

void TextWriter::DrawErrorText(std::string text, float x, float y, float scale) {
	RenderText(text, x, y, scale , glm::vec3(1.0f, 0.0f, 0.0f));
}

void TextWriter::SetEnginePtr(IEngineCore * enginePtr) {
	m_engineInterfacePtr = enginePtr;
	background = new Rect(m_engineInterfacePtr);
}

void TextWriter::RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color) {
	glm::vec3 invertedColor = glm::vec3(0.0f);
	m_engineInterfacePtr->drawText(text, x, y, scale, color, Characters, VAO, VBO);
	if (background != nullptr) {
		Character c = Characters[(int)'Q'];
		float glyphHeight = c.Size.y * scale;
		float glyphWidth = c.Size.x * scale;
		float fullHeight = glyphHeight + (glyphHeight * 0.30f);
		background->DrawRect(glm::vec2(x, y - (fullHeight - glyphHeight)/2), glm::vec2(glyphWidth * text.size(), fullHeight), 0, invertedColor);
	}
}