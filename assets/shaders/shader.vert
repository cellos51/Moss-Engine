#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

layout (location = 3) in mat4 iPosOffset;
layout (location = 7) in vec4 iTexOffset;
layout (location = 8) in int iTexID;
layout (location = 9) in int iLayerID;
//layout (location = 10) in int iShadow;
layout (location = 11) in vec4 iColor;
layout (location = 12) in int iShader;
layout (location = 13) in vec2 iPosition;

uniform float time;

out vec4 color;
out vec2 TexCoord;
flat out uint texId;
flat out float layerId;

uniform vec2 grassDeform;

void main()
{
    texId = iTexID;
    layerId = iLayerID;
    color = vec4(aColor, 1.0f) * iColor;
    TexCoord = vec2((aTexCoord.x / iTexOffset.z) + iTexOffset.x, (aTexCoord.y / iTexOffset.w) + iTexOffset.y);
    gl_Position = iPosOffset * vec4(aPos.xy, 1 - iLayerID, 1.0f); 

    vec2 vertexPosition = iPosition + (aPos.xy * 16);

    if (iShader == 1 && (gl_VertexID == 0 || gl_VertexID == 3))
    {
        float deformPoint = exp(-pow(iPosition.x - grassDeform.x, 2.0) / 512) * exp(-pow(iPosition.y - grassDeform.y, 2.0) / 512);
        float deformDirection = clamp((iPosition.x - grassDeform.x) / 20, -1.0, 1.0);

        float timeOffset = time + ((vertexPosition.x + vertexPosition.y) * 2000);

        vec2 positionOffset = vec2(((sin(timeOffset / 800) / 2) * (1 - deformPoint)) + deformPoint * deformDirection, ((sin(timeOffset / 1000) / 4) * (1 - deformPoint)) - deformPoint); // this is probably not very effecient but whatever.

        gl_Position = iPosOffset * vec4(aPos.xy + positionOffset, 1 - iLayerID, 1.0f);    
        color = vec4(aColor, 1.0f) * vec4(0.5f, 0.5f, 0.0f, 0.0f) * (sin(timeOffset / 800) + 1);
    }
    else if (iShader == 2)
    {
        float timeOffset = time + ((vertexPosition.x * 25000) + (vertexPosition.y * 500));

        gl_Position = iPosOffset * vec4(aPos.xy + vec2(sin(timeOffset / 500) / 8, sin(timeOffset / 1000) / 8), 1 - iLayerID, 1.0f); 
    }
}