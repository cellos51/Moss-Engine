#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;
flat out uint texId;

uniform mat4 transform[1000];

uniform vec4 texOffset[1000];

uniform int textureId[1000];

void main()
{
    texId = textureId[gl_InstanceID];
    gl_Position = transform[gl_InstanceID] * vec4(aPos, 1.0f);
    ourColor = aColor;
    TexCoord = vec2((aTexCoord.x / texOffset[gl_InstanceID].z) + texOffset[gl_InstanceID].x, (aTexCoord.y / texOffset[gl_InstanceID].w) + texOffset[gl_InstanceID].y);
}