#version 460 core

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 LumenColor;

in vec4 color;
in vec3 lumen;
in vec2 TexCoord;
flat in uint texId;
flat in float layerId;

uniform sampler2D ourTexture[16];

void main()
{
    vec4 texColor = vec4(1.0);

    if (layerId > 3)
    {
        discard;
    }

    gl_FragDepth = 1.0f - ((layerId) / 100.0f);


    uint index = -1;
    switch(texId)
    {
        case 0: index = 0; break;
        case 1: index = 1; break;
        case 2: index = 2; break;
        case 3: index = 3; break;
        case 4: index = 4; break;
        case 5: index = 5; break;
        case 6: index = 6; break;
        case 7: index = 7; break;
        case 8: index = 8; break;
        case 9: index = 9; break;
        case 10: index = 10; break;
        case 11: index = 11; break;
        case 12: index = 12; break;
        case 13: index = 13; break;
        case 14: index = 14; break;
        case 15: index = 15; break;
    }

    if (index != -1)
    {
        texColor = texture(ourTexture[index], TexCoord);
    }
    
    if(texColor.a < 0.1)
    {
        discard;
    }

    FragColor = vec4(vec3(0.0f), 1.0f);
}