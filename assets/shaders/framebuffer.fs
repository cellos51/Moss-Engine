#version 410 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D screenTexture;
uniform sampler2D lightTexture;

// const float offset = 1.0 / 300.0;  

// void main()
// {
//     vec2 offsets[9] = vec2[](
//         vec2(-offset,  offset), // top-left
//         vec2( 0.0f,    offset), // top-center
//         vec2( offset,  offset), // top-right
//         vec2(-offset,  0.0f),   // center-left
//         vec2( 0.0f,    0.0f),   // center-center
//         vec2( offset,  0.0f),   // center-right
//         vec2(-offset, -offset), // bottom-left
//         vec2( 0.0f,   -offset), // bottom-center
//         vec2( offset, -offset)  // bottom-right    
//     );

//     float kernel[9] = float[](
//     1.0 / 16, 2.0 / 16, 1.0 / 16,
//     2.0 / 16, 4.0 / 16, 2.0 / 16,
//     1.0 / 16, 2.0 / 16, 1.0 / 16  
//     );
    
//     vec3 sampleTex[9];
//     for(int i = 0; i < 9; i++)
//     {
//         sampleTex[i] = vec3(texture(lightTexture, TexCoord.st + offsets[i]));
//     }
//     vec3 col = vec3(0.0);
//     for(int i = 0; i < 9; i++)
//         col += sampleTex[i] * kernel[i];
    
//     FragColor = texture(screenTexture, TexCoord) * vec4(col, 1.0);
// }  

float normpdf(in float x, in float sigma)
{
    return 0.39894*exp(-0.5*x*x/(sigma*sigma))/sigma;
}

vec2 iResolution = vec2(0.9995, 0.9995);

void main()
{
    vec3 c = texture(lightTexture, TexCoord.xy / iResolution.xy).rgb;
  
    //declare stuff
    const int mSize = 11;
    const int kSize = (mSize-1)/2;
    float kernel[mSize];
    vec3 final_colour = vec3(0.0);
    
    //create the 1-D kernel
    float sigma = 7.0;
    float Z = 0.0;
    for (int j = 0; j <= kSize; ++j)
    {
        kernel[kSize+j] = kernel[kSize-j] = normpdf(float(j), sigma);
    }
    
    //get the normalization factor (as the gaussian has been clamped)
    for (int j = 0; j < mSize; ++j)
    {
        Z += kernel[j];
    }
    
    //read out the texels
    for (int i=-kSize; i <= kSize; ++i)
    {
        for (int j=-kSize; j <= kSize; ++j)
        {
            final_colour += kernel[kSize+j]*kernel[kSize+i]*texture(lightTexture, (TexCoord.xy+vec2(float(i),float(j))) / iResolution.xy).rgb;

        }
    }
    
    
    FragColor = texture(screenTexture, TexCoord) * vec4(final_colour/(Z*Z), 1.0);
    
}