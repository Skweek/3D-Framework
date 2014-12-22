#include "RenderableObject.h"

RenderableObject::RenderableObject()
{
	m_pModel = glm::mat4();
	m_uiShaderProgram = 0;
}

RenderableObject::~RenderableObject()
{

}

void RenderableObject::BindOutputs()
{
	glBindFragDataLocation(m_uiShaderProgram, 0, "Diffuse");
	glBindFragDataLocation(m_uiShaderProgram, 1, "Normal");
	glBindFragDataLocation(m_uiShaderProgram, 2, "Position");

	glBindFragDataLocation(m_uiShaderProgram, 3, "Specular");
	glBindFragDataLocation(m_uiShaderProgram, 4, "Material");
}

void RenderableObject::SetShader(unsigned int a_uiShader)
{
	m_uiShaderProgram = a_uiShader;
}