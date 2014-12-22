#include "LightPass.h"

#include "FBXLoader.h"
#include "ShadowMap.h"
#include "Camera.h"
#include "ShaderManager.h"

LightPass* LightPass::m_pInstance = nullptr;

void LightPass::Create(unsigned int a_uiLightShader)
{
	if( m_pInstance == nullptr )
		m_pInstance = new LightPass(a_uiLightShader);
}

void LightPass::Destroy()
{
	if( m_pInstance != nullptr )
		delete m_pInstance;
}

LightPass* LightPass::Get()
{
	return m_pInstance;
}

LightPass::LightPass(unsigned int a_uiLightShader)
{
	m_uiLightShader = a_uiLightShader;

	InitFBO();

	InitDirLight();
	InitPointLight();
	InitSpotLight();	
	
	ShadowMap::Create(ShaderManager::Get()->GetShader("Shadow"));
}

LightPass::~LightPass()
{
	unsigned int meshCount = m_pSphere->getMeshCount();
	unsigned int materialCount = m_pSphere->getMaterialCount();

	// remove meshes
	for(unsigned int i=0; i<meshCount; i++)
	{
		// Get the current mesh and retrive the render data we assigned to m_userData
		FBXMeshNode* pMesh = m_pSphere->getMeshByIndex(i);
		Utility::RenderData *ro = (Utility::RenderData*)pMesh->m_userData;

		// delete the buffers and free memory from the graphics card
		glDeleteBuffers(1, &ro->m_uiVBO);
		glDeleteBuffers(1, &ro->m_uiIBO);
		glDeleteVertexArrays(1, &ro->m_uiVAO);

		delete ro;
	}

	// loop through each of the materials
	for(int i=0; i < (int)materialCount; i++)
	{
		// get the current material
		FBXMaterial *pMaterial = m_pSphere->getMaterialByIndex(i);
		for(int j=0; j<FBXMaterial::TextureTypes_Count; j++)
		{
			// delete the texture if it was loaded
			if( pMaterial->textureIDs[j] != 0 )
				glDeleteTextures(1, &pMaterial->textureIDs[j]);
		}
	}

	m_pSphere->unload();

	glDeleteFramebuffers(1, &m_uiLightPrePassFBO);
	glDeleteProgram(m_uiLightShader);

	glDeleteTextures(1, &m_uiDepthTexture);
	glDeleteTextures(1, &m_uiLightPassTexture);

	ShadowMap::Destroy();
}

void LightPass::InitFBO()
{
	//create and bind the framebuffer
	glGenFramebuffers(1, &m_uiLightPrePassFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_uiLightPrePassFBO);

	//generate the textures we need, these should be member variables in the class for your app
	glGenTextures(1, &m_uiLightPassTexture);
	
	//creating and binding the texture for the diffuse data to GL_COLOR_ATTACHMENT0
	glBindTexture(GL_TEXTURE_2D, m_uiDepthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, SCREENWIDTH, SCREENHEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_uiDepthTexture, 0);

	//creating and binding the texture for the diffuse data to GL_COLOR_ATTACHMENT0
	glBindTexture(GL_TEXTURE_2D, m_uiLightPassTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCREENWIDTH, SCREENHEIGHT, 0, GL_RGBA, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_uiLightPassTexture, 0);

	GLenum buffs[] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, buffs);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	
	GLenum status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
	if(status != GL_FRAMEBUFFER_COMPLETE )
	{
		printf("Framebuff not bound");
	}
}

void LightPass::InitDirLight()
{
	float points[] = { -1, 1, 0, 1,
						-1, -1, 0, 1,
						1, -1, 0, 1,

						-1, 1, 0, 1,
						1, -1, 0, 1,
						1, 1, 0, 1 };

	glGenVertexArrays(1, &m_pDirLightData.m_uiVAO);
	glGenBuffers(1, &m_pDirLightData.m_uiVBO);

	glBindVertexArray(m_pDirLightData.m_uiVAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_pDirLightData.m_uiVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void LightPass::InitPointLight()
{
	m_pSphere = new FBXFile();
	m_pSphere->load("../models/sphere.fbx", FBXFile::UNITS_CENTIMETER);

	unsigned int meshCount = m_pSphere->getMeshCount();

	for(int i=0; i < (int)meshCount; ++i)
	{
		// get the current mesh
		FBXMeshNode *pMesh = m_pSphere->getMeshByIndex(i);

		// each mesh needs it's own renderdata
		Utility::RenderData *ro = new Utility::RenderData();
		pMesh->m_userData = ro;

		// generate and bind data to buffers
		glGenBuffers(1, &ro->m_uiVBO);
		glGenBuffers(1, &ro->m_uiIBO);
		glGenVertexArrays(1, &ro->m_uiVAO);

		glBindVertexArray(ro->m_uiVAO);
		glBindBuffer(GL_ARRAY_BUFFER, ro->m_uiVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ro->m_uiIBO);

		glBufferData(GL_ARRAY_BUFFER, pMesh->m_vertices.size() * sizeof(FBXVertex), pMesh->m_vertices.data(), GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, pMesh->m_indices.size() * sizeof(unsigned int), pMesh->m_indices.data(), GL_STATIC_DRAW);

		// enable the attribute locations that will be used on our shaders
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (char *)FBXVertex::PositionOffset); // pos

		// unbind and next mesh
		glBindVertexArray(0);
	}
}

void LightPass::InitSpotLight()
{

}

void LightPass::AddDirectional(glm::vec3 a_pDir, glm::vec3 a_pCol)
{
	Light pLight;
	pLight.direction = a_pDir;
	pLight.colour = a_pCol;
	pLight.type = LIGHT_DIRECTIONAL;

	m_pLights.push_back(pLight);
}

void LightPass::AddPoint(glm::vec3 a_pPos, glm::vec3 a_pCol, float a_fIntensity)
{	
	Light pLight;
	pLight.position = a_pPos;
	pLight.colour = a_pCol;
	pLight.intensity = a_fIntensity;
	pLight.type = LIGHT_POINT;

	m_pLights.push_back(pLight);
}

unsigned int LightPass::Render(glm::ivec4 a_pTextures)
{
	// Use the light pre pass FBO
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_uiLightPrePassFBO);
	glClearColor(0.f,0.f,0.f,1.f);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glUseProgram(m_uiLightShader);

	// Additive blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	// Declare uniforms
	GLint uProjection		=		glGetUniformLocation(m_uiLightShader, "Projection");
	GLint uView				=		glGetUniformLocation(m_uiLightShader, "View");
	GLint uModel			=		glGetUniformLocation(m_uiLightShader, "Model");
	GLint uScreen			=		glGetUniformLocation(m_uiLightShader, "screenSize");
	GLint uCamPos			=		glGetUniformLocation(m_uiLightShader, "camera_pos");

	GLint uColour			=		glGetUniformLocation(m_uiLightShader, "Colour");
	GLint uAtt				=		glGetUniformLocation(m_uiLightShader, "Attenuation");
	GLint uAmbient			=		glGetUniformLocation(m_uiLightShader, "ambLight");
	GLint uLightPos			=		glGetUniformLocation(m_uiLightShader, "lightPos");
	GLint uLightDir			=		glGetUniformLocation(m_uiLightShader, "lightDir");
	GLint uMVP				=		glGetUniformLocation(m_uiLightShader, "shadowMVP");

	// Textures
	GLint uWorldTexture		=		glGetUniformLocation(m_uiLightShader, "WorldText");
	GLint uNormalTexture	=		glGetUniformLocation(m_uiLightShader, "NormalText");
	GLint uLightDiffTexture =		glGetUniformLocation(m_uiLightShader, "DiffuseText");
	GLint uSpecuTexture		=		glGetUniformLocation(m_uiLightShader, "SpecText");
	GLint uShadow			=		glGetUniformLocation(m_uiLightShader, "shadowMap");

	// Subroutines
	//GLint uLightType		=		glGetSubroutineUniformLocation(m_uiLightShader, GL_FRAGMENT_SHADER, "LightPosition");
	GLuint uDirLight		=		glGetSubroutineIndex(m_uiLightShader, GL_FRAGMENT_SHADER, "DirLight");
	GLuint uPointLight		=		glGetSubroutineIndex(m_uiLightShader, GL_FRAGMENT_SHADER, "PointLight");
		
	// Things that only need to be bound once
	// textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, a_pTextures[0]);		
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, a_pTextures[1]);	
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, a_pTextures[2]);	
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, a_pTextures[3]);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, ShadowMap::Get()->GetShadowMap());	

	glUniform1i( uNormalTexture, 0 );
	glUniform1i( uLightDiffTexture, 1 );
	glUniform1i( uSpecuTexture, 2 );
	glUniform1i( uWorldTexture, 3 );
	glUniform1i( uShadow, 4 );
	

	glUniformMatrix4fv( uMVP, 1, false, glm::value_ptr(ShadowMap::Get()->GetMVP()));
	glUniform2fv(uScreen, 1, glm::value_ptr(glm::vec2(SCREENWIDTH, SCREENHEIGHT)));
	glUniform4fv(uCamPos, 1, glm::value_ptr(Camera::Get()->GetCamera()[3]));
	glUniform3fv(uAmbient, 1, glm::value_ptr(glm::vec3(0.1f, 0.1f, 0.1f)));			

	glm::mat4 model = glm::mat4();
	float intens = 1.f;
	//FBXNode* sphereRoot;
	//glm::mat4 rootNode;
	for(int i = 0; i < (int)m_pLights.size(); ++i)
	{
		glUniform4fv( uColour, 1, glm::value_ptr(glm::vec4(m_pLights[i].colour, 1)) );
	
		switch(m_pLights[i].type)
		{
		case LIGHT_DIRECTIONAL:
			// Matrices
			glUniformMatrix4fv(uView, 1, false, glm::value_ptr(glm::mat4()));
			glUniformMatrix4fv(uProjection, 1, false, glm::value_ptr(glm::mat4()));
			glUniformMatrix4fv(uModel, 1, false, glm::value_ptr(glm::mat4()));
						
			glUniform3fv(uLightDir, 1, glm::value_ptr(m_pLights[i].direction));

			glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &uDirLight);
			
			glBindVertexArray(m_pDirLightData.m_uiVAO);
			glDrawArrays( GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
			break;

		case LIGHT_POINT:
			intens = m_pLights[i].intensity;

			//sphereRoot = m_pSphere->getRoot();
			//rootNode = glm::mat4();
			//rootNode[3] = glm::vec4( m_pLights[i].position, 1.0);
			//sphereRoot->m_localTransform = rootNode;
			//sphereRoot->updateGlobalTransform();

			glUniformMatrix4fv(uView, 1, false, glm::value_ptr(Camera::Get()->GetView()));
			glUniformMatrix4fv(uProjection, 1, false, glm::value_ptr(Camera::Get()->GetProjection()));
			
			glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &uPointLight);
						
			glUniform2fv(uAtt, 1, glm::value_ptr(glm::vec2(0.1f, intens)));
			
			glUniform3fv(uLightPos, 1, glm::value_ptr(m_pLights[i].position));
			
			
			// for each mesh in the model...
			for(int i=0; i < (int)m_pSphere->getMeshCount(); ++i)
			{
				// get the current mesh
				FBXMeshNode *mesh = m_pSphere->getMeshByIndex(i);
				mesh->updateGlobalTransform();

				glm::mat4 model = glm::translate(m_pLights[i].position);
				model *= glm::scale(glm::vec3(intens));

				// get the render data attached to the m_userData pointer for this mesh
				Utility::RenderData *ro = (Utility::RenderData*)mesh->m_userData;
				glUniformMatrix4fv(uModel, 1, false, glm::value_ptr(model));
				glBindVertexArray(ro->m_uiVAO);
				glDrawElements(GL_TRIANGLES, mesh->m_indices.size(), GL_UNSIGNED_INT, 0);
			}		
			break;

		default:
			break;
		}
	}

	// unbind shader
	glUseProgram(0);
	
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		
	glDisable(GL_BLEND);
	
	// return the light texture
	return m_uiLightPassTexture;
}