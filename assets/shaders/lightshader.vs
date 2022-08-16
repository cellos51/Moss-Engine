#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

layout (location = 3) in mat4 iPosOffset;
layout (location = 7) in vec4 iTexOffset;
layout (location = 8) in int iTexID;
layout (location = 9) in int iLayerID;

uniform mat4 lightPos;
out vec2 v_texcoord;

void main()
{
    gl_Position = lightPos * vec4(aPos, 1.0f);
    v_texcoord = aTexCoord;
}