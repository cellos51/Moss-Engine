#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

layout (location = 3) in mat4 iPosOffset;
layout (location = 7) in vec4 iTexOffset;
layout (location = 8) in int iTexID;
layout (location = 9) in int iLayerID;

out vec3 ourColor;
out vec2 TexCoord;
flat out uint texId;
flat out uint layerId;

vec2 lightPos = vec2(0,0);

void main()
{
    texId = iTexID;
    layerId = iLayerID;
    //gl_Position = iPosOffset * vec4(aPos, 1.0f);
    ourColor = aColor;
    TexCoord = vec2((aTexCoord.x / iTexOffset.z) + iTexOffset.x, (aTexCoord.y / iTexOffset.w) + iTexOffset.y);
    //highp vec2 position = (u_matrix*vec4(a_vertex.xy, 0.0, 1.0)).xy;

    if (iLayerID == 1)
    {
        gl_Position = vec4((iPosOffset * vec4(aPos, 1.0f)).xy - aPos.z * lightPos, 0, 1.0); 
    }
    else if (iLayerID == 3)
    {
        gl_Position = vec4((iPosOffset * vec4(aPos, 1.0f)).xy - aPos.z * lightPos, 0, 0.9); 
    }
    else
    {
        gl_Position = vec4((iPosOffset * vec4(aPos, 1.0f)).xy - aPos.z * lightPos, 0, 1.0 - aPos.z); 
    }
}