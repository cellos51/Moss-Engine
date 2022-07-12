#version 410 core

out vec4 FragColor;

in vec4 ourColor;
varying vec2 v_texcoord;
// flat in uint texId;
// flat in uint layerId;
vec2 res = vec2(1280 / 4, 1280 / 4);

void main()
{
    vec2 st = v_texcoord;
    vec3 canvas = vec3(2);
    float circle = 1.0;

    circle = distance(st, vec2(0.5));

    canvas = canvas * circle;

    FragColor = vec4(-canvas + 1, -canvas + 1);
    //if (texId == currentTexture && layerId == currentLayer)
    //{
        //FragColor = ourColor;
    //}
}