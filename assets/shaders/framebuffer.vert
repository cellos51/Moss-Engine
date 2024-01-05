#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoord;

uniform float zoom;

out vec2 TexCoord;
out float zoomFrag;

void main()
{
    gl_Position = vec4(aPos.xy * zoom, 0, 1.0f);
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
    zoomFrag = zoom;
}