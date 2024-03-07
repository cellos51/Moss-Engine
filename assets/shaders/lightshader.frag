#version 460 core

uniform vec4 lightColor;

uniform vec2 shape;

uniform float width;

uniform int layerId;

out vec4 FragColor;

in vec2 v_texcoord;

const highp float NOISE_GRANULARITY = 2.0 /256.0;

highp float random(highp vec2 coords) 
{
   return fract(sin(dot(coords.xy, vec2(12.9898,78.233))) * 43758.5453);
}

 void main()
 {
	gl_FragDepth = (1.0f - (layerId / 100.0f));

	highp float fragmentColor = distance( v_texcoord, vec2(0.5));
	fragmentColor *= 2.0;

	if (shape.x < 1.0 || shape.y < 1.0)
	{
		if (v_texcoord.y < 0.5f && -((v_texcoord.y - 0.5) * 2) > abs((v_texcoord.x  - 0.5) * shape.y * 2.0) - shape.x)
		{
			FragColor = vec4(-fragmentColor + 1.0) * lightColor;
			FragColor += mix(-NOISE_GRANULARITY, NOISE_GRANULARITY, random(v_texcoord));
		}
		else
		{
			FragColor = vec4(0.0);
		}
	}
	else
	{
		FragColor = vec4(-fragmentColor + 1.0) * lightColor;
		FragColor += mix(-NOISE_GRANULARITY, NOISE_GRANULARITY, random(v_texcoord));
	}
 }