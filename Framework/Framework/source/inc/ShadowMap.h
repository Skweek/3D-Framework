#ifndef _SHADOW_MAP_H_
#define _SHADOW_MAP_H_

#include "Utility.h"

class ShadowMap
{
public:
	static void Create(unsigned int a_uiShadowShader);
	static void Destroy();
	static ShadowMap* Get();

	ShadowMap(unsigned int a_uiShader);
	~ShadowMap();

	void				DrawShadowMap();
	void				CreateMatrix();

	// Allow access to the FBO and shader of the shadow map
	unsigned int		GetFBO() { return m_uiShadowFBO; }
	unsigned int		GetShader() { return m_uiShader; }
	unsigned int		GetShadowMap() { return m_uiShadowMap; }
	glm::mat4			GetMVP() { return m_uiShadowMVP; }

protected:	
	static ShadowMap*	m_pInstance;

	// Shadow map shader
	unsigned int		m_uiShader;
	// Framebuffer
	void				CreateBuffer();
	unsigned int		m_uiShadowFBO;
	unsigned int		m_uiShadowMap;

	// Create the shadow map MVP matrix
	glm::mat4			m_uiShadowMVP;
};

#endif //_SHADOW_MAP_H_