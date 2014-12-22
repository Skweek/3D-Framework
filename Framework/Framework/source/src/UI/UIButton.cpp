#include "UI/UIButton.h"

UIButton::UIButton(glm::vec2 a_pPos, glm::vec2 a_pSize, unsigned int a_uiShader, std::function<void()> a_pFunc, unsigned int a_uiTexture) : Quad(a_pPos, a_pSize, a_uiShader, glm::vec4(1), a_uiTexture)
{
	SetFunction(a_pFunc);
}

UIButton::~UIButton()
{

}

void UIButton::SetFunction(std::function<void()> p)
{
	m_pFuncPointer = p;
}

void UIButton::Update(float a_fDelta)
{
	a_fDelta;
	m_pModel = glm::mat4(	m_pSize.x,	0,			0,	0,
							0,			m_pSize.y,	0,	0,
							0,			0,			1,	0,
							m_pPos.x,	SCREENHEIGHT - m_pPos.y - m_pSize.y,	0,	1); // TO_DO: Fix this issue
	// Drawing UI in Ortho to ensure they are 2D causes them to be flipped.
	// This is a dirty fix, need to find a better way around it

	if(glfwGetMouseButton(Utility::GetWindow(), GLFW_MOUSE_BUTTON_1))
		if(Utility::MouseInBouds(m_pPos, m_pSize))
		{
			m_pFuncPointer();
		}
}	