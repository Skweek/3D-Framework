#ifndef _HEIGHT_MAP_H_
#define _HEIGHT_MAP_H_

#include "Utility.h"

class NavMesh;

class NoiseMap
{
public:
	NoiseMap(unsigned int a_uiShader);
	~NoiseMap();

	void					Init(glm::vec2 a_pSize);
	void					Generate(int a_uiLayers, float a_fAmplitude, float a_fSeed);
	unsigned int			m_uiNoiseTexture;

protected:
	Utility::RenderData*	m_pRenderData;
	unsigned int			m_uiShader;

	unsigned int			m_uiFBO;
	unsigned int			m_uiPBO;

	glm::vec2				m_pSize;
	float*					m_fHeightMap;

};

#endif //_HEIGHT_MAP_H_