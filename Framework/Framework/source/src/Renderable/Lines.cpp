#include "Renderable/Lines.h"
#include "Camera.h"


const int	m_ciMaxLines = 350000;

Lines::Lines(unsigned int a_uiShader)
{
	m_uiShaderProgram = a_uiShader;
	Init();
}

Lines::~Lines()
{
	glDeleteBuffers(1, &m_pRenderData->m_uiVBO);
	glDeleteBuffers(1, &m_pRenderData->m_uiIBO);
	glDeleteVertexArrays(1, &m_pRenderData->m_uiVAO);

	glDeleteProgram(m_uiShaderProgram);

	delete m_pLines;
	delete m_pRenderData;
}

void Lines::AddLines(glm::vec3 a_pStart, glm::vec3 a_pEnd)
{
	m_pLines[m_iNumberOfLines].m_pStart = a_pStart;
	m_pLines[m_iNumberOfLines].m_pEnd = a_pEnd;
	++m_iNumberOfLines;
}

void Lines::Clear()
{
	m_iNumberOfLines = 0;
}

void Lines::Init()
{
	m_pLines = new Line[m_ciMaxLines];
	m_iNumberOfLines = 0;
	
	m_pRenderData = new RenderData();
	glGenBuffers(1, &m_pRenderData->m_uiVBO);
	glGenVertexArrays(1, &m_pRenderData->m_uiVAO);

	glBindVertexArray(m_pRenderData->m_uiVAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_pRenderData->m_uiVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Line) * m_ciMaxLines, m_pLines, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glBindVertexArray(0);
}

void Lines::Update(float a_fDelta)
{
	a_fDelta;
}

void Lines::Draw()
{
	glUseProgram(m_uiShaderProgram);
	glBindVertexArray(m_pRenderData->m_uiVAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, m_pRenderData->m_uiVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_iNumberOfLines * sizeof(Line), m_pLines);
	
	GLint uProjection = glGetUniformLocation(m_uiShaderProgram, "ProjectionView");
	glUniformMatrix4fv(uProjection, 1, false, glm::value_ptr(Camera::Get()->GetProjection() * Camera::Get()->GetView()));

	glBindVertexArray(m_pRenderData->m_uiVAO);
	glDrawArrays(GL_LINES, 0, m_iNumberOfLines * 2);

	glBindVertexArray(0);
	glUseProgram(0);

	Clear();
}