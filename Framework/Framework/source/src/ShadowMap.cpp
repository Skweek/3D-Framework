#include "ShadowMap.h"
#include "Camera.h"
#include "Renderable\Quad.h"
#include "ShaderManager.h"

ShadowMap* ShadowMap::m_pInstance = nullptr;

void ShadowMap::Create(unsigned int a_uiShadowShader)
{
	if( m_pInstance == nullptr )
		m_pInstance = new ShadowMap(a_uiShadowShader);
}

void ShadowMap::Destroy()
{
	if( m_pInstance != nullptr )
		delete m_pInstance;
}

ShadowMap* ShadowMap::Get()
{
	return m_pInstance;
}


ShadowMap::ShadowMap(unsigned int a_uiShader)
{
	m_uiShader = a_uiShader;

	CreateBuffer();
	CreateMatrix();
}

ShadowMap::~ShadowMap()
{
	glDeleteProgram(m_uiShader);
	glDeleteFramebuffers(1, &m_uiShadowFBO);
	glDeleteTextures(1, &m_uiShadowMap);
}

void ShadowMap::CreateBuffer()
{
	// Generate the FrameBuffer
	glGenFramebuffers(1, &m_uiShadowFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_uiShadowFBO);

	glGenTextures(1, &m_uiShadowMap);
	glBindTexture(GL_TEXTURE_2D, m_uiShadowMap);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT16, SCREENWIDTH, SCREENHEIGHT, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
 
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_uiShadowMap, 0);
 
	glDrawBuffer(GL_NONE); // No color buffer is drawn to.
 
	// Always check that our framebuffer is ok
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Shadow buffer failed to initialise. \n");
	}
}

void ShadowMap::CreateMatrix()
{
	// for now assume that there is just the one directional light from -1,-1,0
	glm::vec3 lightInvDir = glm::vec3(1,-0.5,0);
 
	 // Compute the MVP matrix from the light's point of view
	 glm::mat4 depthProjectionMatrix = glm::ortho<float>(-250,250,-250,250,-50,50);
	 glm::mat4 depthViewMatrix = glm::inverse(glm::lookAt(lightInvDir, glm::vec3(0,0,0), glm::vec3(0,1,0)));
	 m_uiShadowMVP = depthProjectionMatrix * depthViewMatrix;
}

void ShadowMap::DrawShadowMap()
{
	Quad* shadowQuad = new Quad(glm::vec2(0,0), glm::vec2(300,300), ShaderManager::Get()->GetShader("Simple"), glm::vec4(1,1,1,1), m_uiShadowMap);
	shadowQuad->Draw();
}