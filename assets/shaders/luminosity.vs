#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

layout (location = 3) in mat4 iPosOffset;
layout (location = 7) in vec4 iTexOffset;
layout (location = 8) in int iTexID;
layout (location = 9) in int iLayerID;
layout (location = 10) in vec4 iShadow;

//uniform vec4 ambientLight;

out vec3 ourColor;
out vec2 TexCoord;
flat out uint texId;
flat out float layerId;

//vec2 testThing = vec2(0f, 0f);

void main()
{
    texId = iTexID;
    layerId = iLayerID;
    gl_Position = iPosOffset * vec4(aPos.xy, 1 - iLayerID, 1.0f);

    ourColor = vec3(iShadow.xyz);

    TexCoord = vec2((aTexCoord.x / iTexOffset.z) + iTexOffset.x, (aTexCoord.y / iTexOffset.w) + iTexOffset.y);
}