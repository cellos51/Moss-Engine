#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

layout (location = 3) in mat4 iPosOffset;
layout (location = 7) in vec4 iTexOffset;
layout (location = 8) in int iTexID;
layout (location = 9) in int iLayerID;
layout (location = 10) in int iShadow;

uniform mat4 lightMatrix;
uniform int currentLayer;

flat out int draw;

vec2 lightPos = vec2(0,0);

vec2 posR = vec2(0,0);
vec2 posL = vec2(0,0);

void main()
{
    if(iLayerID == currentLayer && iShadow == 1)
    {
        draw = 0;

        lightPos = (lightMatrix * vec4(1.0f, 1.0f, 1.0f, 1.0f)).xy;

        if (gl_VertexID == 1 || gl_VertexID == 0) // this shit is slow as hell and needs to be remade 💀
        {
            if ((iPosOffset * vec4(aPos.x, aPos.y, aPos.z,  1.0f)).x >= lightPos.x && (iPosOffset * vec4(aPos.x, 1.0f, aPos.z,  1.0f)).y >= lightPos.y || (iPosOffset * vec4(-aPos.x, aPos.y, aPos.z,  1.0f)).x > lightPos.x)
            {
                posR.y = -1.0f;
            }
            else
            {
                posR.y = 1.0f;
            }

            if ((iPosOffset * vec4(aPos.x, posR.y, aPos.z,  1.0f)).y >= lightPos.y)
            {
                posR.x = 1.0f;
            }
            else
            {
                posR.x = -1.0f;
            }
        }
        else if (gl_VertexID == 2 || gl_VertexID == 3)
        {
            if ((iPosOffset * vec4(aPos.x, aPos.y, aPos.z,  1.0f)).x <= lightPos.x  && (iPosOffset * vec4(aPos.x, 1.0f, aPos.z,  1.0f)).y >= lightPos.y || (iPosOffset * vec4(-aPos.x, aPos.y, aPos.z,  1.0f)).x < lightPos.x)
            {
                posL.y = -1.0f;
            }
            else
            {
                posL.y = 1.0f;
            }

            if ((iPosOffset * vec4(aPos.x, posL.y, aPos.z,  1.0f)).y <= lightPos.y)
            {
                posL.x = 1.0f;
            }
            else
            {
                posL.x = -1.0f;
            }
        }

        if (gl_VertexID == 0)
        {
            gl_Position = vec4((iPosOffset * vec4(posR.x, posR.y, aPos.z,  1.0f)).xy - lightPos, 0, 0);
        }
        else if (gl_VertexID == 3)
        {
            gl_Position = vec4((iPosOffset * vec4(posL.x, posL.y, aPos.z,  1.0f)).xy - lightPos, 0, 0);
        }
        else if (gl_VertexID == 1)
        {
            gl_Position = iPosOffset * vec4(posR.x, posR.y, aPos.z,  1.0f);
        }
        else if (gl_VertexID == 2)
        {
            gl_Position = iPosOffset * vec4(posL.x, posL.y, aPos.z,  1.0f);
        }
    }
    else
    {
        draw = 1;
    }
}