#include "UI\UISlider.h"
#include "Renderable\Quad.h"

UISlider::UISlider(glm::vec2 a_pPos, glm::vec2 a_pSize, unsigned int a_uiShader, float* a_fValue, glm::vec2 a_pMinMax, float a_fStep)
{
	m_pPos = a_pPos;
	m_pIndicatorPos = a_pPos;
	m_pSize = a_pSize;
	m_uiShaderProgram = a_uiShader;

	m_fMinValue = a_pMinMax.x;
	m_fMaxValue = a_pMinMax.y;
	m_fStepValue = (m_fMaxValue - m_fMinValue) / (m_pSize.x - 15);
	m_fStep = a_fStep;

	m_fValue = a_fValue;
	*m_fValue = m_fMinValue;

	Init();
}

UISlider::~UISlider()
{
	delete m_pSliderBG;
	delete m_pSliderFG;
	delete m_pSliderIndicator;
}

void UISlider::Init()
{
	m_pSliderBG = new Quad(m_pPos, m_pSize, m_uiShaderProgram, glm::vec4(0.9,0.9,0.9,1));
	m_pSliderFG = new Quad(m_pPos, glm::vec2(0, m_pSize.y), m_uiShaderProgram, glm::vec4(0.74, 0.74, 0.74, 1));

	m_pSliderIndicator = new Quad(m_pIndicatorPos, glm::vec2(15, m_pSize.y), m_uiShaderProgram, glm::vec4(0.62,0.62,0.62,1));
}

void UISlider::Draw()
{
	m_pSliderIndicator->Draw();
	m_pSliderFG->Draw();
	m_pSliderBG->Draw();
}

void UISlider::Update(float a_fDelta)
{
	a_fDelta;

	m_pSliderBG->Update(a_fDelta);
	m_pSliderFG->Update(a_fDelta);
	m_pSliderIndicator->Update(a_fDelta);

	if(glfwGetMouseButton(Utility::GetWindow(), GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
	{

		if(Utility::MouseInBouds(m_pPos - 2.f, m_pSize + 4.f))
		{
			double x,y;
			glfwGetCursorPos(Utility::GetWindow(), &x, &y);

			m_pIndicatorPos.x = (float)x;

			if(m_pIndicatorPos.x > m_pPos.x + m_pSize.x - 15)
			{
				m_pIndicatorPos.x = m_pPos.x + m_pSize.x - 15;
			}
			if(m_pIndicatorPos.x < m_pPos.x - 15)
			{
				m_pIndicatorPos.x = m_pPos.x - 15;
			}
						
			*m_fValue = (m_fStepValue * ((float)x - m_pPos.x));

			*m_fValue = floor(*m_fValue * 10) / 10.f + m_fMinValue;

			// make sure the value isn't outside the range
			if(*m_fValue > m_fMaxValue)
			{
				*m_fValue = m_fMaxValue;
			}
			if(*m_fValue < m_fMinValue)
			{
				*m_fValue = m_fMinValue;
			}
			
			printf("Value: %f\n", *m_fValue);

			m_pSliderIndicator->SetPos(m_pIndicatorPos);
			
			// Change the width of the FG
			m_pSliderFG->SetSize(glm::vec2(m_pIndicatorPos.x - m_pPos.x - 1.f, m_pSize.y));
			m_pSliderFG->SetPos(m_pPos);
		}
	}
}