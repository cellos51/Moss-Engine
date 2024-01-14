#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

layout (location = 3) in mat4 iTransform;
layout (location = 7) in vec4 iTextureCoordinates;
layout (location = 8) in vec4 iLuminosity;
layout (location = 9) in vec4 iColor;
layout (location = 10) in vec2 iPosition;
layout (location = 11) in int iTextureIndex;
layout (location = 12) in int iLayerIndex;
layout (location = 13) in int iShaderIndex;

//uniform vec4 ambientLight;

out vec3 ourColor;
out vec2 TexCoord;
flat out uint texId;
flat out float layerId;

//vec2 testThing = vec2(0f, 0f);

void main()
{
    texId = iTextureIndex;
    layerId = iLayerIndex;
    gl_Position = iTransform * vec4(aPos.xy, 1 - iLayerIndex, 1.0f);

    ourColor = vec3(iLuminosity.xyz);

    TexCoord = vec2((aTexCoord.x / iTextureCoordinates.z) + iTextureCoordinates.x, (aTexCoord.y / iTextureCoordinates.w) + iTextureCoordinates.y);
}