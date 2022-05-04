#version 410 core

const int maxTextures = 16;

out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;
flat in uint texId;

uniform sampler2D ourTexture[maxTextures];

void main()
{
    for (int i = 1; i <= maxTextures; i++)
    {
        if (texId == i)
        {
            FragColor = texture(ourTexture[i], TexCoord) * vec4(ourColor, 1.0);
        }
    }
}