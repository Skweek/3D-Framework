#version 430

in vec4 vPos;
in vec4 vCol;
in vec3 vLightPos;
in vec3 vLightDir;
in mat4 shadowCoord;

out vec4 Diffuse;

uniform vec2 screenSize;
uniform vec4 camera_pos;

uniform vec3 ambLight;
uniform vec2 Attenuation;

uniform sampler2D WorldText;
uniform sampler2D NormalText;
uniform sampler2D SpecText;
uniform sampler2D DiffuseText;

uniform sampler2D shadowMap;

subroutine vec4 LightType(vec3 LightPos);
subroutine uniform LightType LightPosition;

float shadowFactor = 1.f;
float Att = 1;

// directional
subroutine (LightType) vec4 DirLight(vec3 LightPos)
{
	return normalize(vec4(vLightDir, 0));
}
// point
subroutine (LightType) vec4 PointLight(vec3 LightPos)
{
	vec2 UV = gl_FragCoord.xy / screenSize;
	vec3 World = texture(WorldText, UV).xyz - vLightPos.xyz;
	float len = length(World);
	Att = 1 - smoothstep(Attenuation.x, Attenuation.y, len);

	return vec4(World, 0);
}

void main()
{
	vec2 UV = gl_FragCoord.xy / screenSize;

	vec4 normal = (vec4(texture(NormalText, UV)) * 2) - 1.0;
	vec4 LightDiff = texture(DiffuseText, UV);
	vec3 Spec = texture(SpecText, UV).xyz;

	vec3 finalColour;
	
	vec3 E = normalize(vPos.xyz - camera_pos.xyz);
	vec3 N = normalize(normal.xyz);
	vec3 L = ((LightPosition(vLightPos).xyz));
	vec3 R = reflect(L, N);

	float d = max(0, dot(N,-L));
	float s = pow(max(0, dot(E,R)), 128);
		
	float cosTheta = clamp(dot(N, L), 0, 1);
	float bias = 0.005 * tan(acos(cosTheta));
	bias = clamp(bias, 0.0, 0.01);

	vec4 shadowUV = shadowCoord * texture( WorldText, UV );
	
	vec4 shadow = texture( shadowMap, shadowUV.xy );

	if ( shadow.z < shadowUV.z - bias )
	{
		shadowFactor = 0.1;
	}

	// check to make sur ethe material colour exists
	if(LightDiff == vec4(0)) LightDiff = vec4(1);

	vec3 diff = LightDiff.xyz * vCol.xyz * d * Att;
	vec3 spec = vec3(s * Att);

	finalColour = diff;// * shadowFactor + spec;

	Diffuse = vec4(1,0,0, 1);
}