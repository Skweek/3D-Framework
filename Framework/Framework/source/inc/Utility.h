#ifndef _UTIL_H_
#define _UTIL_H_

#define _CRTDBG_MAP_ALLOC

#ifdef _DEBUG_
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#endif  // _DEBUG

// 3rd party includes
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_CXX03
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/norm.hpp>

class Utility
{
public:

	// Struct for all buffers
	struct RenderData
	{
		unsigned int	m_uiVAO;
		unsigned int	m_uiVBO;
		unsigned int	m_uiIBO;
	};

	// Bitmap Font Characters
	struct BitmapFontCharacter
	{
		int id;
		float x;
		float y;
		float width;
		float height;
		float xOffset;
		float yOffset;
		float xAdvance;
		float channel;
	};

	// function to handle reading a file
	static unsigned char* ReadFile(const char* a_ccPath);
	static unsigned int LoadTexture(const char* a_ccPath);

	static GLFWwindow* GetWindow();
	static void SetWindow(GLFWwindow* a_pWindow);

	static void	GameTick();
	static float GetDelta();
	static float GetTotalTime();

	static float* GetPixelData(unsigned int a_uiTexture);

	static inline float Clamp(float x, float min, float max)
	{
		return x < min ? min : (x > max ? max : x);
	}

	// Check if mouse is within an area
	static bool MouseInBouds(glm::vec2 a_pPos, glm::vec2 a_pBounds);
};

#endif // _UTIL_H_