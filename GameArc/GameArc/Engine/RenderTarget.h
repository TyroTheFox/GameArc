#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>

// GL includes
#include "Shader.h"

class RenderTarget {
public:
	unsigned int FBOid;
	unsigned int colourBuffer;
	unsigned int depthBuffer;

	unsigned int ScreenWidth;
	unsigned int ScreenHeight;

	virtual void SetUp() = 0;
	virtual void ClearBuffer() = 0;
	virtual void RenderToBuffer() = 0;
	virtual void UnbindBuffer() = 0;
	virtual void Render() = 0;
};