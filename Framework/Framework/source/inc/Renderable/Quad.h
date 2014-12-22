#ifndef _QUAD_H_
#define _QUAD_H_

#include "RenderableObject.h"

class Quad : public RenderableObject
{
public:
	Quad(glm::vec2 a_pPos, glm::vec2 a_pSize, unsigned int a_uiShader, glm::vec4 a_pColour = glm::vec4(1), unsigned int a_uiTexture = -1);
	virtual ~Quad();

	void					Init();
	void					Update(float a_fDelta);
	void					Draw();

	void					SetSize(glm::vec2 a_pSize);
	void					SetPos(glm::vec2 a_pPos);

protected:
	RenderData*				m_pRenderData;

	// Quad specific variables
	glm::vec4				m_pColour;
	glm::vec2				m_pSize;
	glm::vec2				m_pPos;
	unsigned int			m_uiTexture;
};

#endif // _Quad_H_