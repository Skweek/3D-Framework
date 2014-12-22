#ifndef _MARCHING_CUBE_H_
#define _MARCHING_CUBE_H_

#include "Utility.h"

class MarchingCubes
{
public:
	MarchingCubes();
	~MarchingCubes();
	
	unsigned int March(const glm::ivec3& a_pCubeCount, const glm::vec3& a_pCubeSize,
						float*** a_fVolume, float a_fVolumeThreshold, glm::vec3* a_pVerts, 
							unsigned int a_uiMaxVerts);

protected:
	unsigned int			marchCube(const glm::ivec3& a_pCubeCorners, const glm::vec3& a_pCubeSize,
										float*** a_fVolume, float a_fVolumeThreshold, glm::vec3* a_pVerts);

	static const glm::ivec3	CUBE_CORNERS[8];
	static const int		EDGE_INDICES[12][2];
	static const glm::vec3	EDGE_DIRECTIONS[12];
	static const int		CUBE_EDGE_FLAGS[256];
	static const int		TRIANGLE_CONNECTION_TABLE[256][16];
};

#endif // _MARCHING_CUBE_H_