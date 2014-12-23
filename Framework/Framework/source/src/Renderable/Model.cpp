#include "ShadowMap.h"
#include "Renderable/Model.h"
#include "ShaderManager.h"
#include "ResourceManager.h"
#include "Camera.h"
#include "Scene.h"
#include <SOIL.h>

Model::Model(const char* a_ccFilePath, unsigned int a_uiShader, bool a_bAnimated, int a_iScale)
{
	m_pFBX = ResourceManager::Get()->LoadFBX(a_ccFilePath, a_iScale);

	if (m_pFBX == nullptr)
	{
		printf("Failed to load file: %s \n", a_ccFilePath);
	}

	SetShader(ShaderManager::Get()->GetShader("Model"));
	a_uiShader;
	m_bAnimated = a_bAnimated;
	m_fScale = 1.f;

	Init();
}

Model::Model(Model* a_pModel)
{
	m_pFBX = a_pModel->m_pFBX;
	SetShader(a_pModel->m_uiShaderProgram);
	m_bAnimated = a_pModel->m_bAnimated;
	m_fScale = a_pModel->m_fScale;
}

Model::~Model()
{
	unsigned int meshCount = m_pFBX->getMeshCount();
	unsigned int materialCount = m_pFBX->getMaterialCount();

	// remove meshes
	for(unsigned int i=0; i<meshCount; i++)
	{
		// Get the current mesh and retrive the render data we assigned to m_userData
		FBXMeshNode* pMesh = m_pFBX->getMeshByIndex(i);
		RenderData *ro = (RenderData *)pMesh->m_userData;

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
		FBXMaterial *pMaterial = m_pFBX->getMaterialByIndex(i);
		for(int j=0; j<FBXMaterial::TextureTypes_Count; j++)
		{
			// delete the texture if it was loaded
			if( pMaterial->textureIDs[j] != 0 )
				glDeleteTextures(1, &pMaterial->textureIDs[j]);
		}
	}

	//delete m_pFBX;

	
}

void Model::SetScale(float a_fScale)
{
	m_fScale = a_fScale;
}

void Model::Init()
{
	unsigned int uiMeshCount = m_pFBX->getMeshCount();
	unsigned int uiMaterialCount = m_pFBX->getMaterialCount();

	// loop through each mesh
	for(int i=0; i < (int)uiMeshCount; ++i)
	{
		// get the current mesh
		FBXMeshNode *pMesh = m_pFBX->getMeshByIndex(i);

		// each mesh needs it's own renderdata
		RenderData *ro = new RenderData();
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
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (char *)FBXVertex::PositionOffset); // pos
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (char *)FBXVertex::ColourOffset); // col
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (char *)FBXVertex::TexCoord1Offset); // uv
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_TRUE, sizeof(FBXVertex), (char *)FBXVertex::NormalOffset); // normal
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (char *)FBXVertex::IndicesOffset); // indices
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (char *)FBXVertex::WeightsOffset); // weights

		// unbind and next mesh
		glBindVertexArray(0);

		// check for alpha
		// Need to figure out how to check for alpha
	}

	// Loop through each material and load the texture
	for(unsigned int i = 0; i < uiMaterialCount; ++i)
	{
		// get the current material being loaded
		FBXMaterial *pMaterial = m_pFBX->getMaterialByIndex(i);

		for(unsigned int j = 0; j<FBXMaterial::TextureTypes_Count; ++j)
		{
			// find the path to the texture to be loaded
			std::string path = m_pFBX->getPath();
			// append the filename of the texture
			path += pMaterial->textureFilenames[j];			
			// load the texture using SOIL
			pMaterial->textureIDs[j] = ResourceManager::Get()->LoadTexture(path.c_str());
			// lets just print what is loaded to the console...
			//printf("Loading texture %i: %s - ID: %i\n", j, path.c_str(), pMaterial->textureIDs[j]);
		}
	}

}

void Model::Update(float a_fDelta)
{
	a_fDelta;

	if( m_bAnimated )
	{
		// grab the skeleton and animation we want to use
		FBXSkeleton* skeleton = m_pFBX->getSkeletonByIndex(0);
		FBXAnimation* animation = m_pFBX->getAnimationByIndex(0);

		// evaluate the animation to update bones
		skeleton->evaluate( animation, Utility::GetTotalTime());
	}

	for(int i = 0; i < (int)m_pFBX->getMeshCount(); ++i)
	{
		// get the current mesh
		FBXMeshNode *mesh = m_pFBX->getMeshByIndex(i);

		// if you move an object around within your scene
		// children nodes are not updated until this function is called.
		mesh->updateGlobalTransform();
	}
}

void Model::Draw()
{	
	// Make sure our bound buffer is the scene render buffer
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, Scene::Get()->GetBuffer());	

	// use the appropriate shader
	glUseProgram(m_uiShaderProgram);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// bind uniforms
	GLint uModel      = glGetUniformLocation(m_uiShaderProgram, "Model");
	GLint uProjection = glGetUniformLocation(m_uiShaderProgram, "Projection");
	GLint uView       = glGetUniformLocation(m_uiShaderProgram, "View");
	
	GLint uDiffuseTexture = glGetUniformLocation(m_uiShaderProgram, "DiffuseTexture");
	GLint uSpecular = glGetUniformLocation(m_uiShaderProgram, "lightSpecular");
	GLint uMaterial = glGetUniformLocation(m_uiShaderProgram, "lightDiffuse");

	GLint uDepth = glGetUniformLocation(m_uiShaderProgram, "sceneDepth");
	glUniform1i(uDepth, Scene::Get()->GetDepth());	
	GLint uScreen = glGetUniformLocation(m_uiShaderProgram, "screenSize");
	glUniform2fv(uScreen, 1, glm::value_ptr(glm::vec2(SCREENWIDTH, SCREENHEIGHT)));

	if( m_bAnimated )
	{
		// animated stuff
		GLint uBones = glGetUniformLocation(m_uiShaderProgram, "bones");
		FBXSkeleton* skeleton = m_pFBX->getSkeletonByIndex(0);
		skeleton->updateBones();
		glUniformMatrix4fv(uBones, skeleton->m_boneCount, GL_FALSE, (float*)skeleton->m_bones);
	}

	int count = (int)m_pFBX->getMeshCount();
	// for each mesh in the model...
	for(int i=0; i < count; ++i)
	{
		// get the current mesh
		FBXMeshNode *mesh = m_pFBX->getMeshByIndex(i);
		
		// get the render data attached to the m_userData pointer for this mesh
		RenderData *ro = (RenderData *) mesh->m_userData;

		// Bind the texture to one of the ActiveTextures
		// if your shader supported multiple textures, you would bind each texture to a new Active Texture ID here
		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D, mesh->m_material->textureIDs[ FBXMaterial::DiffuseTexture] );

		// tell the shader which texture to use
		glUniform1i( uDiffuseTexture, 0 );
		glUniform4fv(uSpecular, 1, glm::value_ptr(mesh->m_material->specular));
		glUniform4fv(uMaterial, 1, glm::value_ptr(mesh->m_material->diffuse));

		// send the Model, View and Projection Matrices to the shader
		glUniformMatrix4fv( uModel,	     1, false, glm::value_ptr(m_pModel * glm::scale(glm::vec3(m_fScale)) * mesh->m_globalTransform) );
		glUniformMatrix4fv( uView,       1, false, glm::value_ptr(Camera::Get()->GetView()) );
		glUniformMatrix4fv( uProjection, 1, false, glm::value_ptr(Camera::Get()->GetProjection()) );

		// bind our vertex array object
		glBindVertexArray(ro->m_uiVAO);
		glDrawElements(GL_TRIANGLES, mesh->m_indices.size(), GL_UNSIGNED_INT, 0);
	}

	// unbind shader
	glUseProgram(0);

	// Now draw the shadow
	// Bind the shadow buffer
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, ShadowMap::Get()->GetFBO());	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Bind the shadow shader
	glUseProgram(ShadowMap::Get()->GetShader());

	// Bind the shadow MVP and then draw each mesh
	GLint uMVP      = glGetUniformLocation(ShadowMap::Get()->GetShader(), "MVP");
	glm::mat4 MVP = ShadowMap::Get()->GetMVP();
	
	uDiffuseTexture = glGetUniformLocation(ShadowMap::Get()->GetShader(), "diffuse");
	
	for(int i=0; i < count; ++i)
	{
		// get the current mesh
		FBXMeshNode *mesh = m_pFBX->getMeshByIndex(i);
		glm::mat4 pvm = MVP * mesh->m_globalTransform; 
		glUniformMatrix4fv( uMVP, 1, false, glm::value_ptr(pvm));

		// get the render data attached to the m_userData pointer for this mesh
		RenderData *ro = (RenderData *) mesh->m_userData;
		
		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D, mesh->m_material->textureIDs[ FBXMaterial::DiffuseTexture] );		
		glUniform1i( uDiffuseTexture, 0 );

		// bind our vertex array object
		glBindVertexArray(ro->m_uiVAO);
		glDrawElements(GL_TRIANGLES, mesh->m_indices.size(), GL_UNSIGNED_INT, 0);
	}
	
	glUseProgram(0);

	// Bind the back buffer again
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	glDisable(GL_BLEND);
}