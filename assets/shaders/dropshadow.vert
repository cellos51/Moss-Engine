#version 460 core

layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

layout (location = 3) in mat4 iTransform;
layout (location = 7) in vec4 iTextureCoordinates;
//layout (location = 8) in vec4 iLuminosity;
//layout (location = 9) in vec4 iColor;
layout (location = 10) in vec2 iPosition;
layout (location = 11) in vec2 iSize;
layout (location = 12) in int iTextureIndex;
layout (location = 13) in int iLayerIndex;
layout (location = 14) in int iShaderIndex;

out vec2 TexCoord;
flat out uint texId;
flat out float layerId;

uniform float time;
uniform vec2 grassDeform;

void main()
{
    texId = iTextureIndex;
    layerId = iLayerIndex;
    TexCoord = vec2((aTexCoord.x / iTextureCoordinates.z) + iTextureCoordinates.x, (aTexCoord.y / iTextureCoordinates.w) + iTextureCoordinates.y);

    vec2 shadowOffset = aPos.xy - vec2(16.0f, 8.0f) / iSize;

    gl_Position = iTransform * vec4(shadowOffset, 1 - iLayerIndex, 1.0f); 

    vec2 vertexPosition = iPosition + (aPos.xy * 16);

    if (iShaderIndex == 1 && (gl_VertexID == 0 || gl_VertexID == 3))
    {
        float deformPoint = exp(-pow(iPosition.x - grassDeform.x, 2.0) / 512) * exp(-pow(iPosition.y - grassDeform.y, 2.0) / 256);
        float deformDirection = clamp((iPosition.x - grassDeform.x) / 20, -1.0, 1.0);

        float timeOffset = time + ((vertexPosition.x + vertexPosition.y) * 2000);

        vec2 positionOffset = vec2(((sin(timeOffset / 800) / 2) * (1 - deformPoint)) + deformPoint * deformDirection, ((sin(timeOffset / 1000) / 4) * (1 - deformPoint)) - deformPoint); // this is probably not very effecient but whatever.

        gl_Position = iTransform * vec4(shadowOffset + positionOffset, 1 - iLayerIndex, 1.0f);    
    }
    else if (iShaderIndex == 2)
    {
        float timeOffset = time + ((vertexPosition.x * 25000) + (vertexPosition.y * 500));

        gl_Position = iTransform * vec4(shadowOffset + vec2(sin(timeOffset / 500) / 8, sin(timeOffset / 1000) / 8), 1 - iLayerIndex, 1.0f); 
    }
}