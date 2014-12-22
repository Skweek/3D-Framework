#version 430

in vec4 vPos;
in vec3 normals;
in vec2 TexCoord;

// Noise map
uniform sampler2D textureMap;

// Terrain samples
uniform sampler2D sand;
uniform sampler2D snow;
uniform sampler2D grass;
uniform sampler2D dirt;

out vec4 Diffuse;
out vec4 Normal;
out vec4 Position;

void main()
{
	// Get the noise value to determine what the colour should be
	float height = texture(textureMap, TexCoord).r;

	// We are going to use magic numbers for this because #yolo
	// Could probably come up with an algorithm to determine the height later but oh well.


	if(height < 0.1)
	{
		Diffuse = mix(texture(sand, TexCoord), texture(dirt, TexCoord), height);
	}
	else if(height < 0.3)
	{
		if(height > 0.25)
		{
			Diffuse = mix(texture(dirt, TexCoord), texture(grass, TexCoord), height);
		}
		else
		{
			Diffuse = mix(texture(dirt, TexCoord), texture(sand, TexCoord), height);
		}
	}
	else if(height > 0.9)
	{
		Diffuse = mix(texture(snow, TexCoord), texture(grass, TexCoord), height - 0.5);
	}
	else
	{
		if(height > 0.5)
		{
			Diffuse = mix(texture(grass, TexCoord), texture(snow, TexCoord), height - 0.5);
		}
		else
		{
			Diffuse = mix(texture(grass, TexCoord), texture(dirt, TexCoord), height);
		}
	}


	Normal = vec4(normalize(normals),1);
	Position = vPos;
}