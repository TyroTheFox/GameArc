#version 430 core
in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform vec3 spriteColor;
layout(location = 0) out vec3 color;

void main()
{    
    color = spriteColor * vec3(texture(texture_diffuse1, TexCoords));
}  