#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "Utility.h"
#include "Renderable/Quad.h"
#include <functional>

class UIButton : public Quad
{
public:
	UIButton(glm::vec2 a_pPos, glm::vec2 a_pSize, unsigned int a_uiShader, std::function<void()> a_pFunc, unsigned int a_uiTexture = 0);
	~UIButton();

	void SetFunction(std::function<void()> p);
	void Update(float a_fDelta);

protected:
	std::function<void()> m_pFuncPointer;


};

#endif //_BUTTON_H_