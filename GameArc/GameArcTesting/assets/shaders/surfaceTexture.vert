#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUV;

out vec3 FragPos;
out vec3 Normal;
out vec2 textureCoords; // texture coordinates passed through from from vertex shader

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	textureCoords = aUV;
	FragPos = vec3(model * vec4(aPos, 1.0));
	Normal = mat3(transpose(inverse(model))) * aNormal;
	gl_Position = projection * view * vec4(FragPos, 1.0f);
}