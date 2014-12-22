#include "Renderable/Volume.h"

Volume::Volume(unsigned int a_uiShader)
{
	m_uiShaderProgram = a_uiShader;
	Init();
}

Volume::~Volume()
{
	glDeleteBuffers(1, &m_pRenderData->m_uiVBO);
	glDeleteBuffers(1, &m_pRenderData->m_uiIBO);
	glDeleteVertexArrays(1, &m_pRenderData->m_uiVAO);

	glDeleteProgram(m_uiShaderProgram);
	delete m_pRenderData;
}

void Volume::Init()
{
	// create grid
	m_pGridSize = glm::ivec3(10,60,10);
	m_pCubeSize = glm::vec3(0.9f,0.25f,0.9f);

	// fill volume array
	int rows = m_pGridSize.x + 1;
	int cols = m_pGridSize.y + 1;
	int depth = m_pGridSize.z + 1;

	m_fVolume = new float** [rows];
	for (int i = 0 ; i < rows ; ++i )
	{
		m_fVolume[i] = new float* [cols];
		for (int j = 0 ; j < cols ; ++j )
		{
			m_fVolume[i][j] = new float [depth];
		}
	}

	m_uiMaxVerts = m_pGridSize.x * m_pGridSize.y * m_pGridSize.z * 5 * 3;
	m_pVerts = new glm::vec3[ m_uiMaxVerts ];

	// Set up ogl buffers
	m_pRenderData = new RenderData();
	glGenBuffers(1, &m_pRenderData->m_uiVBO);
	glGenVertexArrays(1, &m_pRenderData->m_uiVAO);

	glBindVertexArray(m_pRenderData->m_uiVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_pRenderData->m_uiVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * m_uiMaxVerts, nullptr, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (char*)0);

	glBindVertexArray(0);

	// Set up 'blobs'
	m_uiBlobCount = 10;
	m_pBlobPoints = new glm::vec3[m_uiBlobCount];
	m_pBlobVelocity = new glm::vec3[m_uiBlobCount];

	// fill in data
	for(unsigned int i = 0; i < m_uiBlobCount; ++i)
	{
		m_pBlobVelocity[i].x = 0;
		m_pBlobVelocity[i].y = -9.8f * Utility::GetDelta() * 2;
		m_pBlobVelocity[i].z = 0;

		glm::vec2 rad = glm::circularRand(1.0f);
		m_pBlobPoints[i].x = rad.x;
		m_pBlobPoints[i].y = m_pGridSize.y * m_pCubeSize.y * 0.5f + glm::linearRand(2.0f, 20.0f);
		m_pBlobPoints[i].z = rad.y;
	}

	// set volume properties
	m_pFluidColour = glm::vec4(1,0,0,1); // red
	m_pFluidTransform = glm::translate(-2.75f, 0.f, -2.75f);

	m_fVolumeThreshold = 1.5f;
}

void Volume::Update(float a_fDelta)
{
	// update the blobs
	for ( unsigned int i = 0; i < m_uiBlobCount; ++i)
	{
		m_pBlobVelocity[i].y -= 9.8f * a_fDelta;
		m_pBlobPoints[i] += m_pBlobVelocity[i] * a_fDelta;

		if (m_pBlobPoints[i].y < -(m_pGridSize.y * m_pCubeSize.y * 0.5f + 2))
		{
			m_pBlobVelocity[i].x = 0;
			m_pBlobVelocity[i].y = -9.8f * a_fDelta * 2;
			m_pBlobVelocity[i].z = 0;

			glm::vec2 radius = glm::circularRand(1.0f);
			m_pBlobPoints[i].x = radius.x;
			m_pBlobPoints[i].y = m_pGridSize.y * m_pCubeSize.y * 0.5f + glm::linearRand(2.0f,10.0f);
			m_pBlobPoints[i].z = radius.y;
		}
	}

	// populate a volume
	glm::vec3 dist;
	glm::vec3 position;
	float result = 0;
	glm::vec3 start = glm::vec3(m_pGridSize) * m_pCubeSize * -0.5f;

	int rows = m_pGridSize.x + 1;
	int cols = m_pGridSize.y + 1;
	int depth = m_pGridSize.z + 1;
	int x, y, z;

	for ( x = 0 ; x < rows ; ++x)
	{
		for ( y = 0 ; y < cols ; ++y )
		{
			for ( z = 0 ; z < depth ; ++z )
			{
				result = 0.0f;

				// gather the distances to the blobs
				for ( unsigned int i = 0 ; i < m_uiBlobCount ; ++i )
				{
					position = start + glm::vec3(x,y,z) * m_pCubeSize;

					dist = position - m_pBlobPoints[i];
					result += 1.0f / glm::length2( dist );
				}

				m_fVolume[ x ][ y ][ z ] = result;
			}
		}
	}

}

void Volume::Draw()
{	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(m_uiShaderProgram);
	
	// march through our volume and create triangles
	unsigned int vertexCount = m_pCubes.March(m_pGridSize, m_pCubeSize, m_fVolume, m_fVolumeThreshold, m_pVerts, m_uiMaxVerts);

	int location = glGetUniformLocation(m_uiShaderProgram, "projectionViewModel");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(Camera::Get()->GetProjection() * Camera::Get()->GetView() * m_pFluidTransform));

	location = glGetUniformLocation(m_uiShaderProgram, "transposeInvModel");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr( glm::inverseTranspose(m_pFluidTransform) ));

	location = glGetUniformLocation(m_uiShaderProgram, "materialColour");
	glUniform4fv(location, 1, glm::value_ptr(m_pFluidColour));

	glBindVertexArray(m_pRenderData->m_uiVAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, m_pRenderData->m_uiVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * vertexCount, m_pVerts);
	
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);

	glBindVertexArray(0);
	glUseProgram(0);

	glDisable(GL_BLEND);
}