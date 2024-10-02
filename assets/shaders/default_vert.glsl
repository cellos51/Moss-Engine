#version 460
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoord;
layout (location = 3) in vec4 inColor;

layout (location = 0) out vec4 outColor;

layout(push_constant) uniform PushConstants 
{
    mat4 view;
    mat4 projection;
} pushConstants;

void main ()
{
	gl_Position = pushConstants.projection * pushConstants.view * vec4(inPosition, 1.0);
	outColor = inColor;
}