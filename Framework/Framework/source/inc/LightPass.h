#ifndef _LIGHT_PASS_H_
#define _LIGHT_PASS_H_

#include "Utility.h"
#include <vector>

struct Light
{
	glm::vec3	position;
	glm::vec3	direction;
	glm::vec3	colour;
	float		intensity;
	int			type;
};

class FBXFile;

class LightPass
{
public:
	enum LigthTypes
	{
		LIGHT_DIRECTIONAL,
		LIGHT_POINT
	};

	static void Create(unsigned int a_uiLightShader);
	static void Destroy();
	static LightPass* Get();

	void					AddDirectional(glm::vec3 a_pDir, glm::vec3 a_pCol);
	void					AddPoint(glm::vec3 a_pPos, glm::vec3 a_pCol, float a_fIntensity);

	unsigned int			Render(glm::ivec4 a_pTextures);
	
protected:
	static LightPass*		m_pInstance;

	LightPass(unsigned int a_uiLightShader);
	~LightPass();

	void					InitFBO();

	void					InitDirLight();
	void					InitPointLight();
	void					InitSpotLight();

	std::vector<Light>		m_pLights;

	FBXFile*				m_pSphere;
	
	/* Light pre pass will render all lights to a texture */
	/* that will be used in the final comp pass */
	unsigned int			m_uiLightPrePassFBO;
	unsigned int			m_uiLightShader;

	unsigned int			m_uiLightPassTexture;
	unsigned int			m_uiDepthTexture;

	// Each type of light will have its own render data
	Utility::RenderData		m_pDirLightData;
	Utility::RenderData		m_pPointLightData;
	Utility::RenderData		m_pSpotLightData;
};

#endif // _LIGHT_PASS_H_