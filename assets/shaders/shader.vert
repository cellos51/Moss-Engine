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

//uniform vec4 ambientLight;

out vec4 ourColor;
out vec2 TexCoord;
flat out uint texId;
flat out float layerId;

//vec2 testThing = vec2(0f, 0f);

void main()
{
    texId = iTexID;
    layerId = iLayerID;
    gl_Position = iPosOffset * vec4(aPos.xy, 1 - iLayerID, 1.0f); 

    // if (iLayerID > 1)
    // {
    //    float amount = clamp(1 - length((gl_Position.xy - testThing) * 3), 0.1f, 5.0f) * 2; //cool lighting stuff idk
    //     ourColor = vec4(amount, amount, amount, 1.0f); 
    // }
    // else
    // {
    //    ourColor = vec4(aColor - 0.1f, 1.0f); 
    // }  

    //ourColor = vec4(aColor, 1.0f) * ambientLight;

    ourColor = vec4(aColor, 1.0f) * iColor;

    TexCoord = vec2((aTexCoord.x / iTexOffset.z) + iTexOffset.x, (aTexCoord.y / iTexOffset.w) + iTexOffset.y);
}