#version 410 core

uniform vec2 screenSize;

uniform vec4 lightColor;

out vec4 FragColor;

in vec2 v_texcoord;

uniform sampler2D NewTexture;

void main()
{
    vec2 st = v_texcoord;
    vec3 canvas = vec3(2);
    float circle = distance(st, vec2(0.5));

    canvas = canvas * circle;

    FragColor = (vec4(-canvas + 1, -canvas + 1) - texture(NewTexture, gl_FragCoord.xy/screenSize).x) * lightColor;
    //FragColor = vec4(0,0,0,1 - texture(NewTexture, v_texcoord).x);
}