#ifndef _CAMERA_H_
#define _CAMERA_H_

#define GLM_FORCE_CXX03
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Utility.h"

#define SCREENWIDTH 1280
#define SCREENHEIGHT 720

// Singleton class used to globally store the camera, view and projection matrices
class Camera
{
public:
	static void Create();
	static void Destroy();
	static Camera* Get();

	void SetProjection(glm::mat4 a_pProj);
	glm::mat4 GetProjection();

	void SetCamera(glm::mat4 a_pCam);
	glm::mat4 GetCamera();

	void SetView(glm::mat4 a_pView);
	glm::mat4 GetView();

	void SetPos(glm::vec4 a_pPos) { m_pCamMat[3] = a_pPos; }

	void Update();

protected:
	static Camera* m_pInstance;

	Camera();
	~Camera();

	glm::mat4 m_pProjMat;
	glm::mat4 m_pCamMat;
	glm::mat4 m_pViewMat;

};

#endif // _CAMERA_H_