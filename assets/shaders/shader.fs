#version 410 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;
flat in uint texId;

uniform sampler2D ourTexture;
uniform int currentTexture;

void main()
{

    if (texId == currentTexture)
    {
        FragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0);
    }

}