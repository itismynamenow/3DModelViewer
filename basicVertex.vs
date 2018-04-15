#version 420
layout (location = 0) in vec3 position;
layout (location = 1) in vec4 weights;
layout (location = 2) in ivec4 bonesIDs;
layout (location = 3) in vec3 texCoords;


const int MAX_BONES = 200;

uniform mat4 transform;
uniform mat4 bones[MAX_BONES];
uniform int shift;
uniform bool displayTexture;
uniform bool playAnimation;


out vec3 positionToColor;
out vec2 texCoordsOut;

bool compare (int num, vec4 vec)
{
	if(vec[0]==num||vec[1]==num||vec[2]==num||vec[3]==num)
		return true;
	else 
		return false;
}

void main()
{
	vec4 temp;	
    vec4 tempPosition=vec4(position.x,position.y,position.z, 1.0);
	
	if(playAnimation)
	{
		mat4 boneTransform=bones[bonesIDs[0]]* weights[0]+bones[bonesIDs[1]] * weights[1]+bones[bonesIDs[2]] * weights[2]+bones[bonesIDs[3]] * weights[3];
		temp=transform*boneTransform*tempPosition;
		
			
		if(bonesIDs[0]==shift||bonesIDs[1]==shift||bonesIDs[2]==shift||bonesIDs[3]==shift)
		{
			positionToColor=vec3(1,0,0);	
		}

		else
		{
			positionToColor=position;
		}

		if(weights[0]<0||weights[1]<0||weights[2]<0||weights[3]<0)
		{
			positionToColor=vec3(1,1,0);
		}

			
		gl_Position = temp;		
	}
	else
	{
		gl_Position = transform*tempPosition;		
	}

		
	
	if(displayTexture)
	{
		texCoordsOut=vec2(texCoords.x, texCoords.y);
	}
	else if (!playAnimation)
	{
		positionToColor=position;
	}


}