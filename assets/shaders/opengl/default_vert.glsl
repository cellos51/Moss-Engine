#version 450 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoord;
layout (location = 3) in vec4 inColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 outColor;

vec3 lightDirection = normalize(vec3(0.5, -1.0, -0.5));
float ambient = 0.1;

void main()
{
    gl_Position = projection * view * model * vec4(inPosition, 1.0);

    float intensity = max(ambient, dot(normalize(inNormal), -lightDirection));

    outColor = vec4(inColor.rgb * intensity, inColor.a);
}