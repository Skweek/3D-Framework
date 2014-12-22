#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include "Utility.h"
#include "Renderable/3DPlane.h"
#include <vector>

class NoiseMap;
class NavMesh;

class UIButton;
class UISlider;

class Model;
class BitmapFont;

class Text;

class Terrain : public Plane3D
{
public:
	Terrain(glm::vec3 a_pPos, glm::vec2 a_pSize, int a_iLayers, float a_fAmp);
	~Terrain();

	void Draw();
	void DrawNavMesh();
	void Populate();

protected:
	void						Regenerate();
	void						RegenerateNavMesh(int a_iDetail);

	void						DrawTerrainUI();

	NoiseMap*					m_pNoise;
	NoiseMap*					m_pTreeNoise;

	// Lets try adn add some water
	Plane3D*					m_pWaterPlane;

	BitmapFont*					m_pLabelFont;

	// Nav mesh
	NavMesh*					m_pMesh;
	float						m_fNavMeshDetail;
	float						m_fPrevNavMeshDetail;

	// Terrain noise
	float						m_fAmp;
	int							m_iLayers;
	float						m_fPrevSeed;
	float						m_fSeed;
	float						m_fHeight;
	float						m_fPrevHeight;

	// Tree Noise
	float						m_fTreePrevSeed;
	float						m_fTreeSeed;
	float						m_fTreePrevDensity;
	float						m_fTreeDensity;
	std::vector<glm::vec3>		m_pTreesPos;
	std::vector<Model*>			m_pTreeModels;
	Model*						m_pTreeModel;
		
	// UI 
	UIButton*					m_pNavMeshGenerate;
	UIButton*					m_pTerrainGenerate;

	UISlider*					m_pNavMeshDetail;	
	UISlider*					m_pTreeDensity;
	UISlider*					m_pTreeSeed;
	UISlider*					m_pTerrainSeed;
	UISlider*					m_pTerrainHeight;

	Text*						m_TerrainSeedHeader;
	Text*						m_TerrainHeightHeader;
	Text*						m_TreeSeedHeader;
	Text*						m_TreeDensityHeader;
	Text*						m_NavMeshHeader;

	// Some terrain textures to sample from
	unsigned int				m_uiDirt;
	unsigned int				m_uiSnow;
	unsigned int				m_uiGrass;
	unsigned int				m_uiSand;

};

#endif //_TERRAIN_H_