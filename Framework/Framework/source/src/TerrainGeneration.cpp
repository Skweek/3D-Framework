#include "TerrainGeneration.h"
#include "ShaderManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "UI\UISlider.h"
#include "UI\UIButton.h"
#include "NoiseMap.h"
#include "NavMesh.h"
#include "Renderable\Model.h"
#include "Renderable\Text.h"

Terrain::Terrain(glm::vec3 a_pPos, glm::vec2 a_pSize, int a_iLayers, float a_fAmp) : Plane3D(a_pPos, a_pSize, ShaderManager::Get()->GetShader("Terrain"))
{
	m_pLabelFont = ResourceManager::Get()->LoadFont("../fonts/tnr/tnr.fnt");

	// Terrain Noise
	m_pNoise = new NoiseMap(ShaderManager::Get()->GetShader("TerrainGen"));
	m_pNoise->Init(a_pSize);
	m_fAmp = a_fAmp;
	m_iLayers = a_iLayers;
	m_fPrevSeed = 0.f;
	m_fSeed = 1.f;
	m_fPrevHeight = 0.f;
	m_fHeight = 0.5f;

	// Tree noise
	m_pTreeNoise = new NoiseMap(ShaderManager::Get()->GetShader("TerrainGen"));
	m_pTreeNoise->Init(a_pSize);
	m_fTreePrevSeed = 0.f;
	m_fTreeSeed = 1.f;
	m_fTreePrevDensity = 0.f;
	m_fTreeDensity = 5.f;
		
	m_fNavMeshDetail = 1.f;
	m_fPrevNavMeshDetail = 0.f;
	m_pMesh = new NavMesh(m_pSize, (int)m_fNavMeshDetail);

	m_pTreeModel = new Model("../models/tree/tree.fbx", ShaderManager::Get()->GetShader("Model"), false, 3);
	m_pTreeModel->SetScale(0.3f);

	// Create our water plane
	m_pWaterPlane = new Plane3D(a_pPos, a_pSize, ShaderManager::Get()->GetShader("Water"), glm::vec4(0,0,1,1), Utility::LoadTexture("../textures/water.png"));

		// Terrain variables
	m_pTerrainSeed = new UISlider(glm::vec2(10,10), glm::vec2(100,15), ShaderManager::Get()->GetShader("Simple"), &m_fSeed, glm::vec2(1.f,10.f), 1.f);
	m_pTerrainHeight = new UISlider(glm::vec2(10,30), glm::vec2(100,15), ShaderManager::Get()->GetShader("Simple"), &m_fHeight, glm::vec2(0.5f,1.1f), 0.1f);

	// Tree variables
	m_pTreeSeed = new UISlider(glm::vec2(10,50), glm::vec2(100,15), ShaderManager::Get()->GetShader("Simple"), &m_fTreeSeed, glm::vec2(1.f,10.f), 1.f);	
	m_pTreeDensity = new UISlider(glm::vec2(10,70), glm::vec2(100,15), ShaderManager::Get()->GetShader("Simple"), &m_fTreeDensity, glm::vec2(5.f,20.f), 1.f);
	
	// Generate tree/terrain
	m_pTerrainGenerate = new UIButton(glm::vec2(50,90), glm::vec2(60,15), ShaderManager::Get()->GetShader("Simple"), [this]{this->Regenerate();}, Utility::LoadTexture("../textures/Terrain.png"));

	// Nav mesh detail	
	m_pNavMeshDetail = new UISlider(glm::vec2(10,110), glm::vec2(100,15), ShaderManager::Get()->GetShader("Simple"), &m_fNavMeshDetail, glm::vec2(1.f,8.f), 1.f);
	m_pNavMeshGenerate = new UIButton(glm::vec2(50,130), glm::vec2(60,15), ShaderManager::Get()->GetShader("Simple"), [this]{this->RegenerateNavMesh((int)m_fNavMeshDetail);}, Utility::LoadTexture("../textures/NavMesh.png"));

	// Load up our terrain samples
	m_uiDirt = Utility::LoadTexture("../textures/dirt.png");
	m_uiGrass = Utility::LoadTexture("../textures/grass.png");
	m_uiSand = Utility::LoadTexture("../textures/sand.png");
	m_uiSnow = Utility::LoadTexture("../textures/snow.png");

	// Create our labels
	m_TerrainSeedHeader = new Text("Terrain Seed", glm::vec4(15,10,0,1), glm::vec4(1), ShaderManager::Get()->GetShader("Font"), m_pLabelFont, 0.4f);
	m_TerrainHeightHeader = new Text("Slope Height", glm::vec4(15,30,0,1), glm::vec4(1), ShaderManager::Get()->GetShader("Font"), m_pLabelFont, 0.4f);
	m_TreeSeedHeader = new Text("Tree Seed", glm::vec4(15,50,0,1), glm::vec4(1), ShaderManager::Get()->GetShader("Font"), m_pLabelFont, 0.4f);
	m_TreeDensityHeader = new Text("Tree Density", glm::vec4(15,70,0,1), glm::vec4(1), ShaderManager::Get()->GetShader("Font"), m_pLabelFont, 0.4f);
	m_NavMeshHeader = new Text("NavMesh Detail", glm::vec4(15,110,0,1), glm::vec4(1), ShaderManager::Get()->GetShader("Font"), m_pLabelFont, 0.4f);

	// Initial generation
	Regenerate();
}

Terrain::~Terrain()
{
	delete m_pNoise;
	delete m_pTreeNoise;

	delete m_pNavMeshGenerate;
	delete m_pTerrainGenerate;
	 
	delete m_pNavMeshDetail;	
	delete m_pTreeDensity;
	delete m_pTreeSeed;
	delete m_pTerrainSeed;
	delete m_pTerrainHeight;

	delete m_pMesh;

	delete m_pTreeModel;
	delete m_pWaterPlane;

	delete m_TerrainSeedHeader;
	delete m_TerrainHeightHeader;
	delete m_TreeSeedHeader;
	delete m_TreeDensityHeader;
	delete m_NavMeshHeader;
}

void Terrain::DrawNavMesh()
{
	m_pMesh->DrawNavMesh();
}

void Terrain::Draw()
{
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(m_uiShaderProgram);
	
	GLint uProjection = glGetUniformLocation(m_uiShaderProgram, "projectionView");
	glm::mat4 model = glm::translate(m_pPos) * glm::scale(glm::vec3(m_pSize.x, 1.f, m_pSize.y));
	glUniformMatrix4fv(uProjection, 1, false, glm::value_ptr(Camera::Get()->GetProjection() * Camera::Get()->GetView() * model));	

	GLint uDiffuse = glGetUniformLocation(m_uiShaderProgram, "textureMap");

	// Terrain samples	
	GLint uSand = glGetUniformLocation(m_uiShaderProgram, "sand");
	GLint uSnow = glGetUniformLocation(m_uiShaderProgram, "snow");
	GLint uDirt = glGetUniformLocation(m_uiShaderProgram, "dirt");
	GLint uGrass = glGetUniformLocation(m_uiShaderProgram, "grass");
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_pNoise->m_uiNoiseTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_uiSand);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_uiSnow);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_uiDirt);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, m_uiGrass);


	glUniform1i(uDiffuse, 0);
	glUniform1i(uSand, 1);
	glUniform1i(uSnow, 2);
	glUniform1i(uDirt, 3);
	glUniform1i(uGrass, 4);
	
	glBindVertexArray(m_pRenderData->m_uiVAO);
	glPatchParameteri(GL_PATCH_VERTICES, 3);	
	glDrawElements(GL_PATCHES, 6, GL_UNSIGNED_INT, nullptr);

	glBindVertexArray(0);
	glUseProgram(0);
	
	glDisable(GL_BLEND);

	// Draw the water plane
	m_pWaterPlane->Draw();

	// draw populated assets
	for(int i = 0; i < (int)m_pTreeModels.size(); ++i)
	{
		m_pTreeModels[i]->Draw();
	}

	DrawTerrainUI();
}

void Terrain::DrawTerrainUI()
{	
	m_pNavMeshGenerate->Update(0.f);
	m_pTerrainGenerate->Update(0.f);

	m_pNavMeshDetail->Update(0.f);	
	m_pTreeDensity->Update(0.f);
	m_pTreeSeed->Update(0.f);
	m_pTerrainSeed->Update(0.f);
	m_pTerrainHeight->Update(0.f);

	m_pNavMeshGenerate->Draw();
	m_pTerrainGenerate->Draw();
	
	m_pNavMeshDetail->Draw();	
	m_pTreeDensity->Draw();
	m_pTreeSeed->Draw();
	m_pTerrainSeed->Draw();
	m_pTerrainHeight->Draw();

	// Add some text labels
	Scene::Get()->AddText(m_TerrainSeedHeader);
	Scene::Get()->AddText(m_TerrainHeightHeader);
	Scene::Get()->AddText(m_TreeSeedHeader);
	Scene::Get()->AddText(m_TreeDensityHeader);
	Scene::Get()->AddText(m_NavMeshHeader);
}

void Terrain::Regenerate()
{
	if(m_fSeed != m_fPrevSeed || m_fHeight != m_fPrevHeight)
	{
		m_pNoise->Generate(m_iLayers, m_fAmp * m_fHeight, m_fSeed);
		m_pMesh->Transform(Utility::GetPixelData(m_pNoise->m_uiNoiseTexture));
		m_fPrevSeed = m_fSeed;
		m_fPrevHeight = m_fHeight;
		Populate();
		m_pWaterPlane->Displace(m_pNoise->m_uiNoiseTexture);
	}

	if(m_fTreeSeed != m_fTreePrevSeed || m_fTreeDensity != m_fTreePrevDensity)
	{
		m_pTreeNoise->Generate(m_iLayers, m_fAmp, m_fTreeSeed);
		m_fTreePrevSeed = m_fTreeSeed;
		m_fTreePrevDensity = m_fTreeDensity;
		Populate();
	}
}

void Terrain::RegenerateNavMesh(int a_iDetail)
{
	if(m_fNavMeshDetail != m_fPrevNavMeshDetail)
	{
		m_pMesh = new NavMesh(m_pSize, a_iDetail);
		m_pMesh->Transform(Utility::GetPixelData(m_pNoise->m_uiNoiseTexture));

		m_fPrevNavMeshDetail = m_fNavMeshDetail;
	}
}


void Terrain::Populate()
{
	m_pTreeModels.clear();

	float* pTreeNoise = Utility::GetPixelData(m_pTreeNoise->m_uiNoiseTexture);
	float* pTerrainNoise = Utility::GetPixelData(m_pNoise->m_uiNoiseTexture);
	for(int x = 0; x < (int)m_pSize.x; ++x)
	{
		for(int y = 0; y < (int)m_pSize.y; ++y)
		{
			int iPixelIndex = (int)(y * m_pSize.x + x);
			if(pTreeNoise[iPixelIndex*4] > 0.25f && pTreeNoise[iPixelIndex*4] < 0.31f 
				&& pTerrainNoise[iPixelIndex*4] > 0.2/*Stop trees being planted in water*/)
			{
				glm::vec3 pos = m_pMesh->GetNode(glm::vec2(x,y))->position;

				Model* pTree = new Model(m_pTreeModel);
				pTree->SetModel(glm::mat4() * glm::translate(glm::vec3(x, (pTerrainNoise[iPixelIndex*4] * 10.f), y)));
				m_pTreeModels.push_back(pTree);
				
				srand((unsigned int)glfwGetTime());

				// close the node
				m_pMesh->RemoveNodeNeighbours(m_pMesh->GetNode(glm::vec2(x,y)));
				x += rand() % (40 - (int)m_fTreeDensity);
				if(x >= m_pSize.x) x = (int)m_pSize.x - 1;
				y += rand() % (35 - (int)m_fTreeDensity);
				if(y >= m_pSize.y) y = (int)m_pSize.y - 1;
			}
		}
	}

	std::cout << m_pTreeModels.size() << " trees planted.\n";

	delete pTerrainNoise;
	delete pTreeNoise;
}