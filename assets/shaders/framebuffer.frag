#version 460 core

out vec4 fragColor;

in vec2 TexCoord;

in float zoomFrag;

uniform sampler2D screenTexture;
uniform sampler2D blurTexture;
uniform sampler2D lightTexture;
uniform vec4 unlitColor;
uniform float pass;

const highp float NOISE_GRANULARITY = 2.0 /256.0;

highp float random(highp vec2 coords) 
{
   return fract(sin(dot(coords.xy, vec2(12.9898,78.233))) * 43758.5453);
}

void main() 
{
	if (pass == 1.0) // first pass
	{
		fragColor = texture(blurTexture, TexCoord) * texture(lightTexture, TexCoord);
	}
	if (pass == 2.0) // second pass blooM?
	{
		vec4 bloom = vec4(0.0,0.0,0.0,0.0);
		
		float radius = 6.0;
		float intensity = 5.0;

		for (float i = 1.0; i < radius; i++)
		{
			bloom += textureLod(blurTexture, TexCoord, i) / radius; // this shit is fucking diabolical bro. i am generating mipmaps each frame :[ (and it flickers)
		}
		fragColor = bloom * intensity;
	}
	if (pass == 3.0) // third pass
	{
		fragColor = (texture(screenTexture, TexCoord) * min(unlitColor + texture(lightTexture, TexCoord), 1.0)) + texture(blurTexture, TexCoord);

		
		float radius = 0.7 / zoomFrag;
		float softness = 0.3 / zoomFrag;

		float vignette = (-smoothstep(radius, radius - softness, distance(TexCoord, vec2(0.5))) + 1) / 6;
		vignette -= mix(-NOISE_GRANULARITY, NOISE_GRANULARITY, random(gl_FragCoord.xy)); // goofy ahh vignette 

		if (vignette <= 0.001)
		{
			vignette = 0.0;
		}

		fragColor -= vignette;
	}
} // i be blastin' radiohead when making all this shit :P (in rainbows rn) 8/14/23