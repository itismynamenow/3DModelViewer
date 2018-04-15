#version 420
uniform sampler2DArray textureSampler; 
//uniform int offset;
uniform bool displayTexture;

in vec3 positionToColor; 
in vec2 texCoordsOut;

out vec4 fragColor;

void main()
{

	if(displayTexture)
	{
		fragColor =texture( textureSampler, vec3(texCoordsOut.x,texCoordsOut.y, 0));
	}
	else
	{
		fragColor = vec4(positionToColor,1);
	}

} 