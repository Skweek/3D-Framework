#include "NoiseMap.h"
#include "NavMesh.h"

NoiseMap::NoiseMap(unsigned int a_uiShader)
{
	m_uiShader = a_uiShader;
}

NoiseMap::~NoiseMap()
{
	glDeleteFramebuffers(1, &m_uiFBO);
	glDeleteTextures(1, &m_uiNoiseTexture);

	glDeleteBuffers(1, &m_pRenderData->m_uiVBO);
	glDeleteBuffers(1, &m_pRenderData->m_uiIBO);
	glDeleteVertexArrays(1, &m_pRenderData->m_uiVAO);
}

void NoiseMap::Init(glm::vec2 a_pSize)
{
	//create and bind the framebuffer
	glGenFramebuffers(1, &m_uiFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_uiFBO);

	glGenTextures(1, &m_uiNoiseTexture);
	m_pSize = a_pSize;

	//creating and binding the texture for the diffuse data to GL_COLOR_ATTACHMENT0
	glBindTexture(GL_TEXTURE_2D, m_uiNoiseTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (int)m_pSize.x, (int)m_pSize.y, 0, GL_RGBA, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_uiNoiseTexture, 0);

	GLenum buffs[] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, buffs);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	
	GLenum status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
	if(status != GL_FRAMEBUFFER_COMPLETE )
	{
		printf("Framebuff not bound");
	}
	
	// create quad to render to in the end
	float points[] = { -1, 1, 0,
						-1, -1, 0,
						1, -1, 0,

						-1, 1, 0,
						1, -1, 0,
						1, 1, 0};

	m_pRenderData = new Utility::RenderData();
	glGenBuffers(1, &m_pRenderData->m_uiVBO);
	glGenVertexArrays(1, &m_pRenderData->m_uiVAO);

	glBindVertexArray(m_pRenderData->m_uiVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_pRenderData->m_uiVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	// unbind and next mesh
	glBindVertexArray(0);
}

void NoiseMap::Generate(int a_uiLayers, float a_fAmplitude, float a_fSeed)
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_uiFBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glUseProgram(m_uiShader);
	glBindVertexArray(m_pRenderData->m_uiVAO);
	
	// Bind the textures required
	GLint uAmp = glGetUniformLocation(m_uiShader, "Amp");	
	GLint uSeed = glGetUniformLocation(m_uiShader, "Seed");	
	GLint uLayers = glGetUniformLocation(m_uiShader, "Layers");
	GLint uSize = glGetUniformLocation(m_uiShader, "Size");

	glUniform1f( uAmp, a_fAmplitude );

	srand((int)a_fSeed);
	float seed = (float)(rand() % 100 + 100) / 100;
	glUniform1f( uSeed, seed);
	glUniform1i( uLayers, a_uiLayers );		
	glUniform2fv(uSize, 1, glm::value_ptr(m_pSize));

	glDrawArrays( GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);
	glUseProgram(0);	
	
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}