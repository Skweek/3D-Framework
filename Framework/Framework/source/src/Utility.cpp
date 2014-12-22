#include "Utility.h"
#include <SOIL.h>

static GLFWwindow* m_pWindow = nullptr;
static float m_fDelta = 0;
static float m_fTotal = 0;

void Utility::GameTick()
{
	float curTime = (float)glfwGetTime();
	m_fDelta = curTime - m_fTotal;
	m_fTotal += m_fDelta;
}

GLFWwindow* Utility::GetWindow()
{
	return m_pWindow;
};

void Utility::SetWindow(GLFWwindow* a_pWindow)
{
	m_pWindow = a_pWindow;
}

float Utility::GetDelta()
{
	return m_fDelta;
}

float Utility::GetTotalTime()
{
	return m_fTotal;
}

float* Utility::GetPixelData(unsigned int a_uiTexture)
{	
	GLint internalFormat, width, height;
	glBindTexture(GL_TEXTURE_2D, a_uiTexture);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPONENTS, &internalFormat); // get internal format type of GL texture
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width); // get width of GL texture
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height); // get height of GL texture
	
	float* pixel = new float[width*height*4];

	int err = glGetError();
	glGetTexImage(GL_TEXTURE_2D, 0, internalFormat, GL_FLOAT, pixel);
	err = glGetError();
	
	glBindTexture(GL_TEXTURE_2D, 0);

	return pixel;
}

unsigned char* Utility::ReadFile(const char* a_ccPath)
{
	// open file for text reading
	FILE* pFile = fopen(a_ccPath,"rb");
	if (pFile == nullptr)
	{
		printf("Error: Unable to open file '%s' for reading!\n",a_ccPath);
		return nullptr;
	}

	// get number of bytes in file
	fseek(pFile, 0, SEEK_END);
	unsigned int uiLength = (unsigned int)ftell(pFile);
	fseek(pFile, 0, SEEK_SET);

	// allocate buffer and read file contents
	unsigned char* acBuffer = new unsigned char[uiLength + 1];
	memset(acBuffer,0,uiLength + 1);
	fread(acBuffer, sizeof(unsigned char), uiLength, pFile);

	fclose(pFile);
	return acBuffer;
}

unsigned int Utility::LoadTexture(const char* a_ccPath)
{
	return SOIL_load_OGL_texture(a_ccPath, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
								SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_TEXTURE_REPEATS);;
}

bool Utility::MouseInBouds(glm::vec2 a_pPos, glm::vec2 a_pBounds)
{
	a_pBounds;

	// Get the mouse coords
	double x, y;
	glfwGetCursorPos(m_pWindow, &x, &y);

	if(	(x < a_pBounds.x + a_pPos.x) &&
		(x > a_pPos.x)				 &&
		(y < a_pBounds.y + a_pPos.y) &&
		(y > a_pPos.y))
			return true;

	return false;
}
