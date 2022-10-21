#version 410 core

out vec4 FragColor;

in vec4 ourColor;
in vec2 TexCoord;
flat in uint texId;
flat in float layerId;

uniform sampler2D ourTexture[16];

uniform int currentTexture;
uniform int currentLayer;

void main()
{
    vec4 texColor;

    gl_FragDepth = 1 - (layerId / 100);

    switch(texId)
    {
        case 0: texColor = texture(ourTexture[0], TexCoord); break;
        case 1: texColor = texture(ourTexture[1], TexCoord); break;
        case 2: texColor = texture(ourTexture[2], TexCoord); break;
        case 3: texColor = texture(ourTexture[3], TexCoord); break;
        case 5: texColor = texture(ourTexture[5], TexCoord); break;
        case 6: texColor = texture(ourTexture[6], TexCoord); break;
        case 7: texColor = texture(ourTexture[7], TexCoord); break;
        case 8: texColor = texture(ourTexture[8], TexCoord); break;
        case 9: texColor = texture(ourTexture[9], TexCoord); break;
        case 10: texColor = texture(ourTexture[10], TexCoord); break;
        case 11: texColor = texture(ourTexture[11], TexCoord); break;
        case 12: texColor = texture(ourTexture[12], TexCoord); break;
        case 13: texColor = texture(ourTexture[13], TexCoord); break;
        case 14: texColor = texture(ourTexture[14], TexCoord); break;
        case 15: texColor = texture(ourTexture[15], TexCoord); break;
    }
    
    if(texColor.a < 0.1)
    {
        discard;
    }

    FragColor = texColor * ourColor;
}