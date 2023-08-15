#version 460 core

out vec4 fragColor;

in vec2 TexCoord;

uniform sampler2D screenTexture;
uniform sampler2D blurTexture;
uniform sampler2D lightTexture;
uniform vec4 unlitColor;
uniform vec2 direction;

//credit to lisyarus btw :pray: https://github.com/lisyarus/compute/blob/master/blur/source/separable_linear.cpp

const int M = 8;
const int N = 2 * M + 1;

// sigma = 10
const float coeffs[N] = float[N](
	3.219646771412954e-14,
    4.032574274503986e-11,
    1.899604179511627e-8,
    0.0000033817668214086716,
    0.000229272580848483,
    0.005977006727368395,
    0.0605975508650643,
    0.24173030629950387,
    0.3829249254479876,
    0.24173030629950387,
    0.0605975508650643,
    0.005977006727368395,
    0.000229272580848483,
    0.0000033817668214086716,
    1.899604179511627e-8,
    4.032574274503986e-11,
    3.219646771412954e-14
);

vec4 blurMask(sampler2D image, sampler2D mask)
{
	vec4 sum = vec4(0.0);

	for (int i = 0; i < N; ++i)
	{
		vec2 tc = TexCoord + direction * float(i - M);
		sum += coeffs[i] * texture(image, tc) * vec4(1.0,1.0,1.0, ((texture(mask, tc).r + texture(mask, tc).g + texture(mask, tc).b) / 3) * 2);
	}
	
	return sum;
}

vec4 blur(sampler2D image)
{
	vec4 sum = vec4(0.0);

	for (int i = 0; i < N; ++i)
	{
		vec2 tc = TexCoord + direction * float(i - M);
		sum += coeffs[i] * texture(image, tc);
	}
	
	return sum;
}

void main() 
{
	if (direction.x != 0.0) // first pass
	{
		//fragColor = blurMask(blurTexture, lightTexture);
		fragColor = texture(blurTexture, TexCoord) * texture(lightTexture, TexCoord);
	}
	else // second pass
	{

		//fragColor = textureLod(blurTexture, TexCoord, 2.0);

		vec4 bloom = vec4(0.0,0.0,0.0,0.0);
		
		float radius = 7.0;
		float intensity = 6.0;

		for (float i = 1.0; i < radius; i++)
		{
			bloom += textureLod(blurTexture, TexCoord, i) / radius;
		}

		fragColor = (texture(screenTexture, TexCoord) * min(unlitColor + texture(lightTexture, TexCoord), 1.0)) + (bloom * intensity);
	}
    
	
}