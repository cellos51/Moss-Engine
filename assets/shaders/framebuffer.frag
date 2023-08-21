#version 460 core

out vec4 fragColor;

in vec2 TexCoord;

uniform sampler2D screenTexture;
uniform sampler2D blurTexture;
uniform sampler2D lightTexture;
uniform vec4 unlitColor;
uniform float pass;


void main() 
{
	if (pass == 1.0) // first pass
	{
		//fragColor = blur(blurTexture);
		
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
		//fragColor = texture(blurTexture, TexCoord);
	}
    
	
} // i be blastin' radiohead when making all this shit :P (in rainbows rn) 8/14/23