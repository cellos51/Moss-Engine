#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

layout (location = 3) in mat4 iPosOffset;
layout (location = 7) in vec4 iTexOffset;
layout (location = 8) in int iTexID;
layout (location = 9) in int iLayerID;

out vec4 ourColor;
out vec2 TexCoord;
flat out uint texId;
flat out float layerId;

void main()
{
    texId = iTexID;
    layerId = iLayerID;
    gl_Position = iPosOffset * vec4(aPos.xy, 1 - iLayerID, 1.0f);
    ourColor = vec4(aColor, 1.0f);
    TexCoord = vec2((aTexCoord.x / iTexOffset.z) + iTexOffset.x, (aTexCoord.y / iTexOffset.w) + iTexOffset.y);
}