#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

uniform mat4 transform;

uniform vec4 texOffset;

void main()
{
    gl_Position = transform * vec4(aPos, 1.0f);
    ourColor = aColor;
    TexCoord = vec2((aTexCoord.x / texOffset.z) + texOffset.x, (aTexCoord.y / texOffset.w) + texOffset.y);
}