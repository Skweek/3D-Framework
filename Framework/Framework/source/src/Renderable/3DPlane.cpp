#include "Renderable/3DPlane.h"
#include <SOIL.h>
#include "Scene.h"

Plane3D::Plane3D(glm::vec3 a_pPos, glm::vec2 a_pSize, unsigned int a_uiShader, glm::vec4 a_pColour, unsigned int a_uiTexture)
{
	m_uiShaderProgram = a_uiShader;
	m_pPos = a_pPos;
	m_pColour = a_pColour;
	m_pSize = a_pSize;
	

	m_uiTexture = a_uiTexture;

	m_bDisplaced = false;

	Init();
}

Plane3D::~Plane3D()
{	
	glDeleteBuffers(1, &m_pRenderData->m_uiVBO);
	glDeleteBuffers(1, &m_pRenderData->m_uiIBO);
	glDeleteVertexArrays(1, &m_pRenderData->m_uiVAO);

	glDeleteProgram(m_uiShaderProgram);
	delete m_pRenderData;
}

void Plane3D::Init()
{
	// create quad to render to in the end
						// POS						// UV
	float points[] = {	0,	0, 0,	0, 0,
						1,	0, 0,	1, 0,
						1,	0, 1,	1, 1,
						0,	0, 1,	0, 1
					};
	
	unsigned int indices[6] = {	3,1,0,
								3,2,1 };

	m_pRenderData = new RenderData();
	glGenBuffers(1, &m_pRenderData->m_uiVBO);
	glGenBuffers(1, &m_pRenderData->m_uiIBO);
	glGenVertexArrays(1, &m_pRenderData->m_uiVAO);

	glBindVertexArray(m_pRenderData->m_uiVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_pRenderData->m_uiVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_pRenderData->m_uiIBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(3*sizeof(float)));

	// unbind and next mesh
	glBindVertexArray(0);
}

void Plane3D::Displace(unsigned int a_uiDispMap)
{
	m_bDisplaced = true;
	m_uiDisplaceMap = a_uiDispMap;
}

void Plane3D::Update(float a_fDelta)
{
	a_fDelta;
}

void Plane3D::Draw()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(m_uiShaderProgram);
	glBindVertexArray(m_pRenderData->m_uiVAO);
	
	GLint uNormals = glGetUniformLocation(m_uiShaderProgram, "Normals");
	glm::vec3 Norm = glm::cross(glm::vec3(-m_pSize.x,0,-m_pSize.y) - glm::vec3(m_pSize.x,0,m_pSize.y), glm::vec3(-m_pSize.x,0,-m_pSize.y) - glm::vec3(m_pSize.x,0,-m_pSize.y));
	Norm = glm::normalize(Norm);
	glUniform3fv(uNormals, 1, glm::value_ptr(Norm));

	GLint uProjection = glGetUniformLocation(m_uiShaderProgram, "projectionView");
	glm::mat4 model = glm::translate(m_pPos) * glm::scale(glm::vec3(m_pSize.x, 1.f, m_pSize.y));
	glUniformMatrix4fv(uProjection, 1, false, glm::value_ptr(Camera::Get()->GetProjection() * Camera::Get()->GetView() * model));

	GLint uText = glGetUniformLocation(m_uiShaderProgram, "textureMap");	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_uiTexture);
	glUniform1i(uText, 0);

	if(m_bDisplaced)
	{
		GLint uDiffuse = glGetUniformLocation(m_uiShaderProgram, "displacementMap");
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_uiDisplaceMap);
		glUniform1i(uDiffuse, 1);

		glPatchParameteri(GL_PATCH_VERTICES, 3);	
		glDrawElements(GL_PATCHES, 6, GL_UNSIGNED_INT, nullptr);
	}
	else
	{
		glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	glBindVertexArray(0);
	glUseProgram(0);

	glDisable(GL_BLEND);
}