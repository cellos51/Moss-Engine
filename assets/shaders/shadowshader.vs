#version 410 core

layout (location = 0) in vec3 aPos;
// layout (location = 1) in vec3 aColor;
// layout (location = 2) in vec2 aTexCoord;

layout (location = 3) in mat4 iPosOffset;
layout (location = 7) in vec4 iTexOffset;
layout (location = 8) in int iTexID;
layout (location = 9) in int iLayerID;

uniform mat4 lightMatrix;
uniform int currentLayer;

vec2 lightPos = vec2(0,0);

vec2 posR = vec2(0,0);
vec2 posL = vec2(0,0);

void main()
{
     if(iLayerID == currentLayer)
     {

        lightPos = (lightMatrix * vec4(1.0f, 1.0f, 1.0f, 1.0f)).xy;

        gl_Position = vec4((iPosOffset * vec4(aPos, 1.0f)).xy - aPos.z * lightPos, 0, 1.0 - aPos.z);
    }

        // if (gl_VertexID == 0)
        // {
        //     gl_Position = vec4(0, 1, 0, 1.0f);
        // }
        // else if (gl_VertexID == 1)
        // {
        //     gl_Position = vec4(1, 1, 0, 1.0f);
        // }
        // else if (gl_VertexID == 2)
        // {
        //     gl_Position = vec4(0, 0, 0, 1.0f);
        // }
        // else if (gl_VertexID == 3)
        // {

        // }
        
    
}