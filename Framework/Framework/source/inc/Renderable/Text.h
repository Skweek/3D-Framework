#ifndef _TEXT_H_
#define _TEXT_H_

#include "Utility.h"
#include "RenderableObject.h"

class BitmapFont;

class Text : public RenderableObject
{
public:
	Text(const char* a_ccText, glm::vec4 a_pPos, glm::vec4 a_pCol, unsigned int a_uiShader, BitmapFont* a_pFont, float a_fScale = 1.f);
	~Text();

	void Update(float a_fDelta);
	void Draw();

protected:
	void Init();

	Utility::RenderData*	m_pRenderData;

	BitmapFont*				m_pFont;
	const char*				m_ccTextString;
	glm::vec4				m_pPos;
	glm::vec4				m_pCol;
	float					m_fScale;

};

#endif //_TEXT_H_