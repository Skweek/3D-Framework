#ifndef _3D_PLANE_H_
#define _3D_PLANE_H_

#include "RenderableObject.h"

class Plane3D : public RenderableObject
{
public:
	Plane3D(glm::vec3 a_pPos, glm::vec2 a_pSize, unsigned int a_uiShader, glm::vec4 a_pColour = glm::vec4(1), unsigned int a_uiTexture = 0);
	virtual ~Plane3D();

	void					Init();
	void					Update(float a_fDelta);
	void					Draw();

	void					Displace(unsigned int a_uiDispMap);

protected:
	RenderData*				m_pRenderData;

	// Quad specific variables
	glm::vec4				m_pColour;
	glm::vec2				m_pSize;
	glm::vec3				m_pPos;
	unsigned int			m_uiTexture;

	// just incase we want to displace our plane
	bool					m_bDisplaced;
	unsigned int			m_uiDisplaceMap;
};

#endif // _3D_PLANE_H_