#version 460

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

layout(binding = 0) uniform UniformBufferObject 
{
    mat4 model;
} ubo;

vec3 lightDirection = normalize(vec3(0.5, -1.0, -0.5));
float ambient = 0.1;

void main()
{
	gl_Position = pushConstants.projection * pushConstants.view * ubo.model * vec4(inPosition, 1.0);
    
    float intensity = max(ambient, dot(normalize(inNormal), -lightDirection));

    outColor = vec4(inColor.rgb * intensity, inColor.a);
}