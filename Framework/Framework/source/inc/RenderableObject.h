#ifndef _REND_OBJ_H_
#define _REND_OBJ_H_

#include "Utility.h"
#include "Camera.h"

class RenderableObject
{
public:
	RenderableObject();
	~RenderableObject();

	void				SetShader(unsigned int a_uiShader);

	virtual void		Init() = 0;
	virtual void		Update(float a_fDelta) = 0;
	virtual void		Draw() = 0;

	void				SetModel(glm::mat4 a_pModel) { m_pModel = a_pModel; }

protected:
	void				BindOutputs();

	glm::mat4			m_pModel;

	// Struct for all buffers
	struct RenderData
	{
		unsigned int	m_uiVAO;
		unsigned int	m_uiVBO;
		unsigned int	m_uiIBO;
	};

	// Shader to use for the rendering
	unsigned int	m_uiShaderProgram;
};

#endif // _REND_OBJ_H_