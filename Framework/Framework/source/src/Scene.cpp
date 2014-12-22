#include "Scene.h"

#include "LightPass.h"
#include "RenderableObject.h"
#include "Renderable\Lines.h"
#include "ShadowMap.h"
#include "ShaderManager.h"

#include "TerrainGeneration.h"
#include "UI/UIButton.h"
#include "UI/UISlider.h"
#include "Renderable/Model.h"
#include "Renderable/Text.h"

Scene* Scene::m_pInstance = nullptr;

void Scene::Create(unsigned int a_uiShader)
{
	if( m_pInstance == nullptr )
		m_pInstance = new Scene(a_uiShader);
}

void Scene::Destroy()
{
	if( m_pInstance != nullptr )
		delete m_pInstance;
}

Scene* Scene::Get()
{
	return m_pInstance;
}

Scene::Scene(unsigned int a_uiCompShader)
{
	m_uiShaderProgram = a_uiCompShader;
	
	m_pLines = new Lines(ShaderManager::Get()->GetShader("Lines"));

	InitFBO();
	InitQuad();
}

Scene::~Scene()
{
	RemoveChildren();

	// Clear the text vector
	while(!m_pText.empty())
	{
		Text* pText = m_pText.back();
		delete pText;
		m_pText.pop_back();
	}

	glDeleteTextures(1, &m_uiDiffuseTexture);
	glDeleteTextures(1, &m_uiNormalTexture);
	glDeleteTextures(1, &m_uiPositionTexture);
	glDeleteTextures(1, &m_uiDepthTexture);

	glDeleteTextures(1, &m_uiSpecularTexture);
	glDeleteTextures(1, &m_uiMaterialTexture);

	glDeleteFramebuffers(1, &m_uiDeferredRenderFBO);

	delete m_pLines;
	delete m_pRenderData;

}

void Scene::InitFBO()
{
	//create and bind the framebuffer
	glGenFramebuffers(1, &m_uiDeferredRenderFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_uiDeferredRenderFBO);

	//generate the textures we need, these should be member variables in the class for your app
	glGenTextures(1, &m_uiDiffuseTexture);
	glGenTextures(1, &m_uiNormalTexture);
	glGenTextures(1, &m_uiPositionTexture);
	glGenTextures(1, &m_uiDepthTexture);

	glGenTextures(1, &m_uiMaterialTexture);
	glGenTextures(1, &m_uiSpecularTexture);
	
	//creating and binding the texture for the diffuse data to GL_COLOR_ATTACHMENT0
	glBindTexture(GL_TEXTURE_2D, m_uiDepthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, SCREENWIDTH, SCREENHEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_uiDepthTexture, 0);

	//creating and binding the texture for the diffuse data to GL_COLOR_ATTACHMENT0
	glBindTexture(GL_TEXTURE_2D, m_uiDiffuseTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCREENWIDTH, SCREENHEIGHT, 0, GL_RGBA, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_uiDiffuseTexture, 0);

	//creating and binding the texture for the normal data to GL_COLOR_ATTACHMENT1
	glBindTexture(GL_TEXTURE_2D, m_uiNormalTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCREENWIDTH, SCREENHEIGHT, 0, GL_RGBA, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, m_uiNormalTexture, 0);

	//creating and binding the texture for the world position data to GL_COLOR_ATTACHMENT2
	glBindTexture(GL_TEXTURE_2D, m_uiPositionTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, SCREENWIDTH, SCREENHEIGHT, 0, GL_RGBA, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, m_uiPositionTexture, 0);
	
	//creating and binding the texture for the light diffuse data to GL_COLOR_ATTACHMENT3
	glBindTexture(GL_TEXTURE_2D, m_uiMaterialTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCREENWIDTH, SCREENHEIGHT, 0, GL_RGBA, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, m_uiMaterialTexture, 0);

	//creating and binding the texture for the specular data to GL_COLOR_ATTACHMENT4
	glBindTexture(GL_TEXTURE_2D, m_uiSpecularTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCREENWIDTH, SCREENHEIGHT, 0, GL_RGBA, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, m_uiSpecularTexture, 0);

	GLenum buffs[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4};
	glDrawBuffers(5, buffs);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	
	GLenum status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
	if(status != GL_FRAMEBUFFER_COMPLETE )
	{
		printf("Framebuff not bound");
	}
}

void Scene::InitQuad()
{
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

void Scene::RenderQuad(unsigned int a_uiLightTexture)
{
	glUseProgram(m_uiShaderProgram);
	glBindVertexArray(m_pRenderData->m_uiVAO);
	
	// Bind the textures required
	GLint uDiffuseTexture = glGetUniformLocation(m_uiShaderProgram, "diffuse");	
	GLint uLightTexture = glGetUniformLocation(m_uiShaderProgram, "light");
	GLint uNorms = glGetUniformLocation(m_uiShaderProgram, "normals");
	GLint uPos = glGetUniformLocation(m_uiShaderProgram, "position");
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_uiDiffuseTexture);	
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, a_uiLightTexture);	
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_uiNormalTexture);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_uiPositionTexture);	

	glUniform1i( uDiffuseTexture, 0 );
	glUniform1i( uLightTexture, 1 );	
	glUniform1i( uNorms, 2 );	
	glUniform1i( uPos, 3 );	

	GLint uScreen = glGetUniformLocation(m_uiShaderProgram, "screenSize");
	glUniform2fv(uScreen, 1, glm::value_ptr(glm::vec2(SCREENWIDTH, SCREENHEIGHT)));

	glDrawArrays( GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);
	glUseProgram(0);

}

void Scene::AddChild(RenderableObject* a_pObject)
{
	m_pChildren.push_back(a_pObject);
}

void Scene::AddLine(glm::vec3 a_pStart, glm::vec3 a_pEnd)
{
	m_pLines->AddLines(a_pStart, a_pEnd);
}

void Scene::AddText(Text* a_pText)
{
	m_pText.push_back(a_pText);
}

void Scene::RemoveChildren()
{
	while(!m_pChildren.empty())
	{
		//RenderableObject* pObject = m_pChildren.front();
		//delete pObject;
		m_pChildren.pop_back();
	}
}

void Scene::ClearLines()
{
	m_pLines->Clear();
}

void Scene::UpdateChildren(float a_fDelta)
{
	for(int i = 0; i < (int)m_pChildren.size(); ++i)
	{
		m_pChildren[i]->Update(a_fDelta);
	}
}

void Scene::RenderChildren()
{
	glEnable(GL_DEPTH_TEST);

	// Draw to the scene FBO
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_uiDeferredRenderFBO);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_pLines->Draw();

	for(int i = 0; i < (int)m_pChildren.size(); ++i)
	{
		m_pChildren[i]->Draw();
	}
	
	m_pLines->Draw();

	// Draw any text that was added to the scene
	for (int i = 0; i < (int)m_pText.size(); ++i)
	{
		m_pText[i]->Draw();
	}

	// Clear the text vector
	while(!m_pText.empty())
	{
		m_pText.pop_back();
	}

	RemoveChildren();
	
	// Unbind FBO
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	
	

	// Draw the shadow map
	ShadowMap::Get()->CreateMatrix();


	// Render light pre pass
	unsigned int uiLightTexture = LightPass::Get()->Render(glm::ivec4(m_uiNormalTexture, m_uiMaterialTexture, m_uiSpecularTexture, m_uiPositionTexture));


	RenderQuad(uiLightTexture);
	//ShadowMap::Get()->DrawShadowMap();

	glDisable(GL_DEPTH_TEST);
}