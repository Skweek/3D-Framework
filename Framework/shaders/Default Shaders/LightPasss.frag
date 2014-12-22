#version 330

in vec4 vPosition;

uniform sampler2D colour_tex;
uniform sampler2D normal_tex;
uniform sampler2D worldPos_tex;
uniform sampler2D shadow_tex;

uniform vec4 camera_pos;
uniform vec2 screenSize;

uniform vec4 lightDir;
uniform vec4 lightPos;
uniform vec4 lightColour;
uniform vec4 lightSpecular;
//Lights attenuation x = min | y = max
uniform vec2 lightAtt;

uniform int lightType;
//Type:
//DIRECTIONAL	= 0
//POINT			= 1
//SPOT			= 2

uniform mat4 Model;
uniform mat4 lightProjView;

in vec4 shadowCoord;

out vec4 Light;

float Shadow()
{
	vec2 UV = gl_FragCoord.xy / screenSize;

	vec4 position = texture(worldPos_tex, UV);
	vec4 q = lightProjView * position;
	vec3 t = (q.xyz / q.w + 1) / 2;
	float depth = texture(shadow_tex, t.xy).r;

	//return depth;

	if( t.z + 0.0001 <= depth)
	{
		return 1.0;
	}
	else
	{
		return 0.1;
	}


}

float Shadow2()
{
	if( texture(shadow_tex, shadowCoord.xy ).z < shadowCoord.z)
	{
		return 0.0;
	}

	return 1.0;
}

void main()
{

	vec2 UV = gl_FragCoord.xy / screenSize;

	vec4 DiffuseColour = texture(colour_tex, UV);
	vec4 Normal = texture(normal_tex, UV);
	Normal = Normal * 2 - 1;
	//vec4 Spec = texture(spec_tex, UV);
	vec4 WorldPosition = texture(worldPos_tex, UV);
	vec4 Eye = normalize(camera_pos - vPosition);
	vec4 LightDir = normalize(lightDir);

	//Init some variables
	vec4 lightSpecColour = vec4(0,0,0,1);
	vec4 lightDiffColour = vec4(0,0,0,1);
	vec4 L = vec4(0,0,0,1);

	float shadows = Shadow();

	if( lightType == 0) //Directional
	{
		lightSpecColour = lightSpecular;
		lightDiffColour = lightColour;
	}
	else if( lightType == 1) //Point
	{
		vec4 LD = WorldPosition - lightPos;
		float len = length(LD);
		float attenuation = 1 - smoothstep(lightAtt.x, lightAtt.y, len);

		lightSpecColour = lightSpecular;
		lightDiffColour = lightColour;

		lightSpecColour *= attenuation;
		lightDiffColour *= attenuation;

		LightDir = vec4( normalize( LD.xyz), 0);
	}
	else if(lightType == 2) //Spot
	{
	
	}


	vec4 R = vec4(reflect(LightDir.xyz, Normal.xyz), 0);

	float d = max(0, dot(Normal, -LightDir));
	vec4 diffuse = /*DiffuseColour */ lightDiffColour * d * shadows;

	float s = pow( max(0, dot(Eye, R) ), 128);
	vec4 specular = lightSpecColour * s * shadows;

	
	Light += diffuse + specular;
	//Light += vec4(1,1,1,1) * shadows;
}