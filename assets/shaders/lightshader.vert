#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 lightPos;
out vec2 v_texcoord;

void main()
{
    gl_Position = lightPos * vec4(aPos, 1.0f);
    v_texcoord = aTexCoord;
}