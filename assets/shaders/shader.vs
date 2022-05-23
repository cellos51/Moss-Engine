#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

layout (location = 3) in mat4 iPosOffset;
layout (location = 7) in vec4 iTexOffset;
layout (location = 8) in int iTexID;

out vec3 ourColor;
out vec2 TexCoord;
flat out uint texId;

void main()
{
    texId = iTexID;
    gl_Position = iPosOffset * vec4(aPos, 1.0f);
    ourColor = aColor;
    TexCoord = vec2((aTexCoord.x / iTexOffset.z) + iTexOffset.x, (aTexCoord.y / iTexOffset.w) + iTexOffset.y);
}