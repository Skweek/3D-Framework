#ifndef _UI_SLIDER_H_
#define _UI_SLIDER_H_

#include "Utility.h"
#include "UI/UIBase.h"

class Quad;

class UISlider : public UIBase
{
public:
	UISlider(glm::vec2 a_pPos, glm::vec2 a_pSize, unsigned int a_uiShader, float* a_fValue, glm::vec2 a_pMinMax, float a_fStep);
	~UISlider();

	void		Update(float a_fDelta);
	void		Init();
	void		Draw();

protected:

	glm::vec2	m_pPos;
	glm::vec2	m_pIndicatorPos;
	glm::vec2	m_pSize;

	// We need 3 quads for the slider
	// BG of the slider, FG of the slider and the slider indicator
	Quad*		m_pSliderBG;
	Quad*		m_pSliderFG;
	Quad*		m_pSliderIndicator;

	float		m_fMinValue;
	float		m_fMaxValue;

	float		m_fStep;
	float		m_fStepValue;
	float*		m_fValue;
};

#endif //_UI_SLIDER_H_