#include "Renderable/Quad.h"
#include <SOIL.h>
#include "Scene.h"

Quad::Quad(glm::vec2 a_pPos, glm::vec2 a_pSize, unsigned int a_uiShader, glm::vec4 a_pColour, unsigned int a_uiTexture)
{
	m_uiShaderProgram = a_uiShader;
	SetPos(a_pPos);
	m_pColour = a_pColour;
	SetSize(a_pSize);

	m_uiTexture = a_uiTexture;

	Init();
}

Quad::~Quad()
{	
	glDeleteBuffers(1, &m_pRenderData->m_uiVBO);
	glDeleteBuffers(1, &m_pRenderData->m_uiIBO);
	glDeleteVertexArrays(1, &m_pRenderData->m_uiVAO);

	glDeleteProgram(m_uiShaderProgram);
	delete m_pRenderData;
}

void Quad::Init()
{
	// create quad to render to in the end
	float points[] = { 0, 0, 0, 0, 0,
						1, 0, 0, 1, 0,
						0, 1, 0, 0, 1,

						0, 1, 0, 0, 1,
						1, 0, 0, 1, 0,
						1, 1, 0, 1, 1};

	m_pRenderData = new RenderData();
	glGenBuffers(1, &m_pRenderData->m_uiVBO);
	glGenBuffers(1, &m_pRenderData->m_uiIBO);
	glGenVertexArrays(1, &m_pRenderData->m_uiVAO);

	glBindVertexArray(m_pRenderData->m_uiVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_pRenderData->m_uiVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_pRenderData->m_uiIBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(3*sizeof(float)));

	// unbind and next mesh
	glBindVertexArray(0);
}

void Quad::SetPos(glm::vec2 a_pPos)
{
	m_pPos = a_pPos;
}

void Quad::SetSize(glm::vec2 a_pPos)
{
	m_pSize = a_pPos;
}

void Quad::Update(float a_fDelta)
{
	a_fDelta;
	m_pModel = glm::mat4(	m_pSize.x,	0,			0,	0,
							0,			m_pSize.y,	0,	0,
							0,			0,			1,	0,
							m_pPos.x,	SCREENHEIGHT - m_pPos.y - m_pSize.y,	0,	1); // TO_DO: Fix this issue
	// Drawing UI in Ortho to ensure they are 2D causes them to be flipped.
	// This is a dirty fix, need to find a better way around it
}

void Quad::Draw()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(m_uiShaderProgram);
	glBindVertexArray(m_pRenderData->m_uiVAO);
	
	GLint uModel      = glGetUniformLocation(m_uiShaderProgram, "Model");	
	glm::mat4 ortho = glm::ortho(0.f, (float)SCREENWIDTH, 0.f, (float)SCREENHEIGHT, -1.f, 1.f);
	glUniformMatrix4fv( uModel,	     1, false, glm::value_ptr(ortho * m_pModel) );

	GLint uTexture    = glGetUniformLocation(m_uiShaderProgram, "Texture");		
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_uiTexture);	
	glUniform1i(uTexture, 0);

	GLint uColour     = glGetUniformLocation(m_uiShaderProgram, "Colour");	
	glUniform4fv( uColour, 1, glm::value_ptr(m_pColour) );

	glDrawArrays( GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);
	glUseProgram(0);

	glDisable(GL_BLEND);
}