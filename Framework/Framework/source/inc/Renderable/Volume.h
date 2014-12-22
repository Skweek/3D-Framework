#ifndef _VOLUME_H_
#define _VOLUME_H_

#include "RenderableObject.h"
#include "MarchingCube.h"

class Volume : public RenderableObject
{
public:
	Volume(unsigned int a_uiShader);
	virtual ~Volume();

	

protected:
	void					Init();
	void					Update(float a_fDelta);
	void					Draw();

	RenderData*				m_pRenderData;

	MarchingCubes			m_pCubes;

	// Volume data
	float***				m_fVolume;
	glm::ivec3				m_pGridSize;
	glm::vec3				m_pCubeSize;
	float					m_fVolumeThreshold;

	// blob particles
	unsigned int			m_uiBlobCount;
	glm::vec3*				m_pBlobPoints;
	glm::vec3*				m_pBlobVelocity;

	// vertices
	glm::vec3*				m_pVerts;
	unsigned int			m_uiMaxVerts;

	// fluid colour/pos
	glm::vec4				m_pFluidColour;
	glm::mat4				m_pFluidTransform;





};

#endif // _VOLUME_H_