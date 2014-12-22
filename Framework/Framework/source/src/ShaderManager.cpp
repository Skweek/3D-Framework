#include "ShaderManager.h"

ShaderManager* ShaderManager::m_pInstance = nullptr;

void ShaderManager::Create()
{
	if( m_pInstance == nullptr )
		m_pInstance = new ShaderManager();
}

void ShaderManager::Destroy()
{
	if( m_pInstance != nullptr )
		delete m_pInstance;
}

ShaderManager* ShaderManager::Get()
{
	return m_pInstance;
}

ShaderManager::ShaderManager()
{

}

ShaderManager::~ShaderManager()
{
	// iterate through map and destroy shaders
	std::map<const char*, unsigned int>::iterator pIter = m_pShaderList.begin();
	for( ; pIter != m_pShaderList.end(); ++pIter)
	{
		glDeleteProgram(pIter->second);
	}
}

void ShaderManager::AddShader(const char* a_ccShaderName, unsigned int a_uiShader)
{
	m_pShaderList.insert(std::pair<const char*, unsigned int>(a_ccShaderName, a_uiShader));
}

void ShaderManager::AddShader(const char* a_ccShaderName, const char* a_ccVert, 
				const char* a_ccFrag, const char* a_ccGeom, 
				const char* a_ccCont, const char* a_ccEval)
{
	m_pShaderList.insert(std::pair<const char*, unsigned int>(a_ccShaderName, CreateProgram(a_ccVert, a_ccFrag, a_ccGeom, a_ccCont, a_ccEval)));
}

unsigned int ShaderManager::CreateProgram(const char* a_ccVert, const char* a_ccFrag, const char* a_ccGeom, const char* a_ccCont, const char* a_ccEval)
{
	unsigned int uiHandle = glCreateProgram();
	// attach our shaders
	if(a_ccVert != nullptr)
		glAttachShader(uiHandle, LoadShader(a_ccVert, GL_VERTEX_SHADER));
	if(a_ccFrag != nullptr)
		glAttachShader(uiHandle, LoadShader(a_ccFrag, GL_FRAGMENT_SHADER));
	if(a_ccGeom != nullptr)
		glAttachShader(uiHandle, LoadShader(a_ccGeom, GL_GEOMETRY_SHADER));
	if(a_ccCont != nullptr)
		glAttachShader(uiHandle, LoadShader(a_ccCont, GL_TESS_CONTROL_SHADER));
	if(a_ccEval != nullptr)
		glAttachShader(uiHandle, LoadShader(a_ccEval, GL_TESS_EVALUATION_SHADER));
		
	glBindFragDataLocation(uiHandle, 0, "Diffuse");
	glBindFragDataLocation(uiHandle, 1, "Normal");
	glBindFragDataLocation(uiHandle, 2, "Position");

	glBindFragDataLocation(uiHandle, 3, "Material");
	glBindFragDataLocation(uiHandle, 4, "Specular");

	// Link shaders together
	glLinkProgram(uiHandle);

	int result;
	glGetProgramiv(uiHandle, GL_LINK_STATUS, &result);
	if (result == GL_FALSE)
	{
		int infoLogLength = 0;		
		glGetShaderiv(uiHandle, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];

		glGetShaderInfoLog(uiHandle, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n %s \n", infoLog);
		delete[] infoLog;
		return 0;
	}

	return uiHandle;
}

unsigned int ShaderManager::LoadShader(const char* a_ccFilePath, unsigned int a_uiShaderType)
{
	unsigned  char* cShaderSource = Utility::ReadFile(a_ccFilePath);
	unsigned int uiHandle = glCreateShader(a_uiShaderType);

	glShaderSource(uiHandle, 1, (const char**)&cShaderSource, 0);
	glCompileShader(uiHandle);

	// Make sure it compiled fine
	int result;
	glGetShaderiv(uiHandle, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int infoLogLength = 0;		
		glGetShaderiv(uiHandle, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];

		glGetShaderInfoLog(uiHandle, infoLogLength, 0, infoLog);
		printf("Error: Failed to compile shader!\n %s \n", infoLog);
		delete[] infoLog;
		return 0;
	}

	// cleanup
	delete cShaderSource;

	// return the shader handle
	return uiHandle;
}