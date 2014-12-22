#ifndef _NAVMESH_H_
#define _NAVMESH_H_

#include "Utility.h"
#include <vector>


// Public navmesh triangle struct
struct NavTri
{
	// 3 points for each triangle
	glm::vec3 vert[4];

	// Neighbouring triangles (max of 3)
	NavTri* neighbours[3];

};

// Nodes
struct NavNode
{
	glm::vec3 position;
	NavNode* top;
	NavNode* left;
	NavNode* right;
	NavNode* bottom;
	bool open;
};

class NavMesh
{
public:

	// Create a navmesh grid based on size and a default detail level of 64 splits per side
	NavMesh(glm::vec2 a_pSize, int a_fDetail = 2);
	// Create a navmesh grid based on a models mesh
	NavMesh();

	~NavMesh();

	// return the nav mesh
	std::vector<NavTri> GetNavTris() {return m_pTris;}
	
	// Set each nodes neghbours
	void SetNeighbours();

	// transform the navmesh
	void Transform(float* a_uiNoiseTexture);

	// Cull steep joints
	void CullNavMesh();

	void DrawNavMesh();

	// Get node at position x,y
	// x and y represent each axis on the 2d map of nodes
	NavNode* GetNode(glm::vec2 a_pPos);
	
	// Reset a nodes links to neighbouring nodes
	void ResetNodeNeighbours(NavNode* a_pNode);

	// Clears a nodes links to neighbouring nodes
	void RemoveNodeNeighbours(NavNode* a_pNode);

protected:
	glm::vec2 m_pSize;
	int m_iDetail;
	std::vector<NavTri> m_pTris;
	std::vector<NavNode*> m_pNodes;

};

#endif //_NAVMESH_H_