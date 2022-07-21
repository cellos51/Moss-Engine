#version 410 core

uniform vec4 lightColor;

out vec4 FragColor;

varying vec2 v_texcoord;

void main()
{
    vec2 st = v_texcoord;
    vec3 canvas = vec3(2);
    float circle = distance(st, vec2(0.5));

    canvas = canvas * circle;

    FragColor = vec4(-canvas + 1, -canvas + 1) * lightColor;
}