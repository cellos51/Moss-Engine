#version 410 core

out vec4 FragColor;

in vec4 ourColor;
in vec2 TexCoord;
flat in uint texId;
flat in uint layerId;

uniform sampler2D ourTexture;
uniform int currentTexture;
uniform int currentLayer;

void main()
{
    if (texId == currentTexture && layerId == currentLayer)
    {
        FragColor = texture(ourTexture, TexCoord) * ourColor;
    }

}