#version 410 core

flat in int draw;

out vec4 FragColor;

void main()
{
	if (draw == 1)
	{
		discard;
	}
	

	FragColor = vec4(0,0,0,1);
}