#version 460 core

uniform vec4 lightColor;

uniform int layerId;

out vec4 FragColor;

in vec2 v_texcoord;

const highp float NOISE_GRANULARITY = 2.0 /255.0;

highp float random(highp vec2 coords) 
{
   return fract(sin(dot(coords.xy, vec2(12.9898,78.233))) * 43758.5453);
}

 void main()
 {
	gl_FragDepth = (1.0f - (layerId / 100.0f));

	highp float fragmentColor = distance( v_texcoord, vec2(0.5));
	fragmentColor += mix(-NOISE_GRANULARITY, NOISE_GRANULARITY, random(v_texcoord));
	fragmentColor = 2.0 * fragmentColor;
	FragColor = vec4(-fragmentColor + 1.0) * lightColor;
 }