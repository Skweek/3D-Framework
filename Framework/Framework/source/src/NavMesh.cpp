#include "NavMesh.h"
#include "Scene.h"

NavMesh::NavMesh(glm::vec2 a_pSize, int a_iDetail)
{
	m_pSize = a_pSize;

	// Detail is 2 to the power of a_iDetail, makes nice square grids
	m_iDetail = (int)pow(2.0, (double)a_iDetail);
	glm::vec2 pGridSize = m_pSize / (float)m_iDetail;

	// list of node positions already added to node map
	std::vector<glm::vec3> positions;

	// Create navtris
	for(int x = 0; x < m_iDetail; ++x)
	{
		for(int y = 0; y < m_iDetail; ++y)
		{
			// 4 points all up for the grid space
			glm::vec3 p0 = glm::vec3((x*pGridSize.x), 0, (y*pGridSize.y));
			glm::vec3 p1 = glm::vec3((x*pGridSize.x)+pGridSize.x, 0, (y*pGridSize.y));
			glm::vec3 p2 = glm::vec3((x*pGridSize.x)+pGridSize.x, 0, (y*pGridSize.y)+pGridSize.y);
			glm::vec3 p3 = glm::vec3((x*pGridSize.x), 0, (y*pGridSize.y)+pGridSize.y);

			// add the points as a node
			// seems messy for now, need to come back and optimise
			if(std::find(positions.begin(), positions.end(), p0) == positions.end())
			{
				NavNode* pNode0 = new NavNode;
				pNode0->open = true;
				pNode0->position = p0;
				pNode0->top = nullptr;
				pNode0->bottom = nullptr;
				pNode0->left = nullptr;
				pNode0->right = nullptr;

				if(x > 0)
				{
					pNode0->bottom = m_pNodes[((x-1)*m_iDetail)+y];
					m_pNodes[((x-1)*m_iDetail)+y]->top = pNode0;
				}

				if(y > 0)
				{
					pNode0->left = m_pNodes[(x * m_iDetail) + y - 1];
					m_pNodes[(x * m_iDetail) + y - 1]->right = pNode0;
				}

				m_pNodes.push_back(pNode0);
				positions.push_back(p0);
			}

			if(std::find(positions.begin(), positions.end(), p0) == positions.end())
			{
				NavNode* pNode0 = new NavNode;
				pNode0->open = true;
				pNode0->position = p0;
				pNode0->top = nullptr;
				pNode0->bottom = nullptr;
				pNode0->left = nullptr;
				pNode0->right = nullptr;

				if(x > 0)
				{
					pNode0->bottom = m_pNodes[((x-1)*m_iDetail)+y];
					m_pNodes[((x-1)*m_iDetail)+y]->top = pNode0;
				}

				if(y > 0)
				{
					pNode0->left = m_pNodes[(x * m_iDetail) + y - 1];
					m_pNodes[(x * m_iDetail) + y - 1]->right = pNode0;
				}

				m_pNodes.push_back(pNode0);
				positions.push_back(p0);
			}

			if(std::find(positions.begin(), positions.end(), p0) == positions.end())
			{
				NavNode* pNode0 = new NavNode;
				pNode0->open = true;
				pNode0->position = p0;
				pNode0->top = nullptr;
				pNode0->bottom = nullptr;
				pNode0->left = nullptr;
				pNode0->right = nullptr;

				if(x > 0)
				{
					pNode0->bottom = m_pNodes[((x-1)*m_iDetail)+y];
					m_pNodes[((x-1)*m_iDetail)+y]->top = pNode0;
				}

				if(y > 0)
				{
					pNode0->left = m_pNodes[(x * m_iDetail) + y - 1];
					m_pNodes[(x * m_iDetail) + y - 1]->right = pNode0;
				}

				m_pNodes.push_back(pNode0);
				positions.push_back(p0);
			}

			if(std::find(positions.begin(), positions.end(), p0) == positions.end())
			{
				NavNode* pNode0 = new NavNode;
				pNode0->open = true;
				pNode0->position = p0;
				pNode0->top = nullptr;
				pNode0->bottom = nullptr;
				pNode0->left = nullptr;
				pNode0->right = nullptr;

				if(x > 0)
				{
					pNode0->bottom = m_pNodes[((x-1)*m_iDetail)+y];
					m_pNodes[((x-1)*m_iDetail)+y]->top = pNode0;
				}

				if(y > 0)
				{
					pNode0->left = m_pNodes[(x * m_iDetail) + y - 1];
					m_pNodes[(x * m_iDetail) + y - 1]->right = pNode0;
				}

				m_pNodes.push_back(pNode0);
				positions.push_back(p0);
			}
		}
	}
}

NavMesh::NavMesh()
{

}

NavMesh::~NavMesh()
{
	std::vector<NavNode*>::iterator pIter = m_pNodes.begin();
	for(; pIter != m_pNodes.end(); ++pIter)
	{
		NavNode* pNode = *(pIter);
		delete pNode;
	}
}

void NavMesh::SetNeighbours()
{
	for(int x = 0; x < m_iDetail; ++x)
	{
		for(int y = 0; y < m_iDetail; ++y)
		{
			if(x > 0)
			{
				m_pNodes[((x)*m_iDetail)+y]->bottom = m_pNodes[((x-1)*m_iDetail)+y];
				m_pNodes[((x-1)*m_iDetail)+y]->top = m_pNodes[((x)*m_iDetail)+y];
			}

			if(y > 0)
			{
				m_pNodes[(x * m_iDetail) + y]->left = m_pNodes[(x * m_iDetail) + y - 1];
				m_pNodes[(x * m_iDetail) + y - 1]->right = m_pNodes[(x * m_iDetail) + y];
			}
		}
	}
}

void NavMesh::Transform(float* a_uiNoiseTexture)
{
	SetNeighbours();
	int size = (int)(m_pNodes.size());
	// loop through each navtri
	for(int index = 0; index < size; ++index)
	{
		// set the y coord to the r component * 5
		int iPixelIndex = (int)(Utility::Clamp(m_pNodes[index]->position.z - 1.f, 0.f, m_pSize.x - 1.f) * m_pSize.x + (Utility::Clamp(m_pNodes[index]->position.x - 1.f, 0.f, m_pSize.y - 1.f)));
		m_pNodes[index]->position.y = a_uiNoiseTexture[iPixelIndex * 4] * 10.f;
	}

	CullNavMesh();
}

void NavMesh::CullNavMesh()
{
	int size = (int)(m_pNodes.size());
	// loop through each navtri
	for(int index = 0; index < size; ++index)
	{
		// Get the dot product of each node and its neighbours
		if(m_pNodes[index]->top != nullptr)
		{
			float angle = abs(m_pNodes[index]->top->position.y - m_pNodes[index]->position.y);
			if(angle > 0.9f)
				m_pNodes[index]->top = nullptr;
		}
			
		if(m_pNodes[index]->bottom != nullptr)
		{
			float angle = abs(m_pNodes[index]->bottom->position.y - m_pNodes[index]->position.y);
			if(angle > 0.9f)
				m_pNodes[index]->bottom = nullptr;
		}
						
		if(m_pNodes[index]->right != nullptr)
		{
			float angle = abs(m_pNodes[index]->right->position.y - m_pNodes[index]->position.y);
			if(angle > 0.9f)
				m_pNodes[index]->right = nullptr;
		}
			
		if(m_pNodes[index]->left != nullptr)
		{
			float angle = abs(m_pNodes[index]->left->position.y - m_pNodes[index]->position.y);
			if(angle > 0.9f)
				m_pNodes[index]->left = nullptr;
		}
			
	}
}

void NavMesh::DrawNavMesh()
{
	glm::vec3 yDisp = glm::vec3(0.f, 1.f, 0.f);
	
	for(int index = 0; index < (int)m_pNodes.size(); ++index)
	{
		// Draw a line between each node
		if(m_pNodes[index]->top != nullptr)
			Scene::Get()->AddLine(m_pNodes[index]->position + yDisp, m_pNodes[index]->top->position + yDisp);
		if(m_pNodes[index]->bottom != nullptr)
			Scene::Get()->AddLine(m_pNodes[index]->position + yDisp, m_pNodes[index]->bottom->position + yDisp);
		if(m_pNodes[index]->right != nullptr)
			Scene::Get()->AddLine(m_pNodes[index]->position + yDisp, m_pNodes[index]->right->position + yDisp);
		if(m_pNodes[index]->left != nullptr)
			Scene::Get()->AddLine(m_pNodes[index]->position + yDisp, m_pNodes[index]->left->position + yDisp);
	}
}

NavNode* NavMesh::GetNode(glm::vec2 a_pPos)
{
	// Get the position relevant to itself in the array
	int x = (int)(a_pPos.x / (m_pSize.x / m_iDetail));
	int y = (int)(a_pPos.y / (m_pSize.y / m_iDetail));

	if((x*y) < (int)m_pNodes.size())
	{
		return m_pNodes[((x)*m_iDetail)+y];
	}

	return nullptr;
}

void NavMesh::ResetNodeNeighbours(NavNode* a_pNode)
{	
	// Get the position relevant to itself in the array
	int x = (int)(a_pNode->position.x / (m_pSize.x / m_iDetail));
	int y = (int)(a_pNode->position.z / (m_pSize.y / m_iDetail));

	// bottom link
	if(x > 0)
	{
		a_pNode->bottom = m_pNodes[((x-1)*m_iDetail)+y];
		m_pNodes[((x-1)*m_iDetail)+y]->top = a_pNode;
	}
	// top link
	if(x < 127)
	{
		a_pNode->top = m_pNodes[((x+1)*m_iDetail)+y];
		m_pNodes[((x+1)*m_iDetail)+y]->bottom = a_pNode;
	}
	// left link
	if(y > 0)
	{
		a_pNode->left = m_pNodes[(x * m_iDetail) + y - 1];
		m_pNodes[(x * m_iDetail) + y - 1]->right = a_pNode;
	}
	// right link
	if(y < 127)
	{
		a_pNode->right = m_pNodes[(x * m_iDetail) + y + 1];
		m_pNodes[(x * m_iDetail) + y + 1]->left = a_pNode;
	}
}

void NavMesh::RemoveNodeNeighbours(NavNode* a_pNode)
{	
	// Get the position relevant to itself in the array
	int x = (int)(a_pNode->position.x / (m_pSize.x / m_iDetail));
	int y = (int)(a_pNode->position.z / (m_pSize.y / m_iDetail));

	// bottom link
	if(x > 0)
	{
		a_pNode->bottom = nullptr;
		m_pNodes[((x-1)*m_iDetail)+y]->top = nullptr;
	}
	// top link
	if(x < m_iDetail - 1)
	{
		a_pNode->top = nullptr;
		m_pNodes[((x+1)*m_iDetail)+y]->bottom = nullptr;
	}
	// left link
	if(y > 0)
	{
		a_pNode->left = nullptr;
		m_pNodes[(x * m_iDetail) + y - 1]->right = nullptr;
	}
	// right link
	if(y < m_iDetail - 1)
	{
		a_pNode->right = nullptr;
		m_pNodes[(x * m_iDetail) + y + 1]->left = nullptr;
	}
}