#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

layout (location = 3) in mat4 iPosOffset;
layout (location = 7) in vec4 iTexOffset;
layout (location = 8) in int iTexID;
layout (location = 9) in int iLayerID;

out vec4 ourColor;
// flat out uint texId;
// flat out uint layerId;

uniform mat4 lightPos;
uniform vec3 lightColor;
varying vec2 v_texcoord;

void main()
{
    //texId = iTexID;
    //layerId = iLayerID;
    gl_Position = lightPos * vec4(aPos, 1.0f);
    //vertexPos = gl_Position;
    ourColor = vec4(lightColor, 0.5f);
    v_texcoord = aTexCoord;

    //TexCoord = vec2((aTexCoord.x / iTexOffset.z) + iTexOffset.x, (aTexCoord.y / iTexOffset.w) + iTexOffset.y);
}