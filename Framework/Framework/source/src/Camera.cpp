
#include "Camera.h"
#include <GLFW/glfw3.h>

Camera* Camera::m_pInstance = nullptr;

void Camera::Create()
{
	if( m_pInstance == nullptr )
		m_pInstance = new Camera();
}

void Camera::Destroy()
{
	if( m_pInstance != nullptr )
		delete m_pInstance;
}

Camera* Camera::Get()
{
	return m_pInstance;
}

Camera::Camera()
{

}

Camera::~Camera()
{

}

void Camera::SetProjection(glm::mat4 a_pProj)
{
	m_pProjMat = a_pProj;
}

glm::mat4 Camera::GetProjection()
{
	return m_pProjMat;
}

void Camera::SetCamera(glm::mat4 a_pCam)
{
	m_pCamMat = a_pCam;
}

glm::mat4 Camera::GetCamera()
{
	return m_pCamMat;
}

void Camera::SetView(glm::mat4 a_pView)
{
	m_pViewMat = a_pView;
}

glm::mat4 Camera::GetView()
{
	return m_pViewMat;
}

void Camera::Update()
{
	GLFWwindow* window = glfwGetCurrentContext();

	// Get the camera's forward, right, up, and location vectors
	glm::vec4 vForward = m_pCamMat[2];
	glm::vec4 vRight = m_pCamMat[0];
	glm::vec4 vUp = m_pCamMat[1];
	glm::vec4 vTranslation = m_pCamMat[3];

	float frameSpeed = 50 * Utility::GetDelta();

	// Translate camera
	if (glfwGetKey(window,'W') == GLFW_PRESS)
	{
		vTranslation -= vForward * frameSpeed;
	}
	if (glfwGetKey(window,'S') == GLFW_PRESS)
	{
		vTranslation += vForward * frameSpeed;
	}
	if (glfwGetKey(window,'D') == GLFW_PRESS)
	{
		vTranslation += vRight * frameSpeed;
	}
	if (glfwGetKey(window,'A') == GLFW_PRESS)
	{
		vTranslation -= vRight * frameSpeed;
	}
	if (glfwGetKey(window,'Q') == GLFW_PRESS)
	{
		vTranslation += vUp * frameSpeed;
	}
	if (glfwGetKey(window,'E') == GLFW_PRESS)
	{
		vTranslation -= vUp * frameSpeed;
	}

	m_pCamMat[3] = vTranslation;

	// check for camera rotation
	static bool sbMouseButtonDown = false;
	if (glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
	{
		static double siPrevMouseX = 0;
		static double siPrevMouseY = 0;

		if (sbMouseButtonDown == false)
		{
			sbMouseButtonDown = true;
			glfwGetCursorPos(window,&siPrevMouseX,&siPrevMouseY);
		}

		double mouseX = 0, mouseY = 0;
		glfwGetCursorPos(window,&mouseX,&mouseY);

		double iDeltaX = mouseX - siPrevMouseX;
		double iDeltaY = mouseY - siPrevMouseY;

		siPrevMouseX = mouseX;
		siPrevMouseY = mouseY;

		glm::mat4 mMat;
		
		// pitch
		if (iDeltaY != 0)
		{
			mMat = glm::axisAngleMatrix( glm::vec3(vRight.xyz()), (float)-iDeltaY / 150.0f );
			vRight = mMat * vRight;
			vUp = mMat * vUp;
			vForward = mMat * vForward;
		}

		// yaw
		if (iDeltaX != 0)
		{
			mMat = glm::axisAngleMatrix( glm::vec3(0,1,0), (float)-iDeltaX / 150.0f );
			vRight = mMat * vRight;
			vUp = mMat * vUp;
			vForward = mMat * vForward;
		}

		m_pCamMat[0] = vRight;
		m_pCamMat[1] = vUp;
		m_pCamMat[2] = vForward;
	}
	else
	{
		sbMouseButtonDown = false;
	}

}