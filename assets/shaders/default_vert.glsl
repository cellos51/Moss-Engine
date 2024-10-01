#version 460
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoord;
layout (location = 3) in vec4 inColor;

layout (location = 0) out vec4 outColor;



void main ()
{
	gl_Position = vec4 (positions[gl_VertexIndex], 0.0, 1.0);
	outColor = colors[gl_VertexIndex];
}