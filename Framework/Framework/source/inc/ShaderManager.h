#ifndef _SHADER_H_
#define _SHADER_H_

#include "Utility.h"
#include <map>

class ShaderManager
{
public:
	static void Create();
	static void Destroy();
	static ShaderManager* Get();

	/* Add an already compiled shader */
	void AddShader(const char* a_ccShaderName, unsigned int a_uiShader);

	/* Compile and add a shader*/
	void AddShader(const char* a_ccShaderName, const char* a_ccVert, 
					const char* a_ccFrag, const char* a_ccGeom, 
					const char* a_ccCont, const char* a_ccEval);

	/* Compile and return a shader program */
	unsigned int CreateProgram(const char* a_ccVert, const char* a_ccFrag, const char* a_ccGeom, const char* a_ccCont, const char* a_ccEval);

	/* Load a shader file and return the handle */
	unsigned int LoadShader(const char* a_ccFilePath, unsigned int a_uiShaderType);

	inline unsigned int GetShader(const char* a_ccShaderName)
	{
		return m_pShaderList[a_ccShaderName];
	}

protected:
	static ShaderManager* m_pInstance;

	ShaderManager();
	~ShaderManager();

	std::map<const char*, unsigned int>	m_pShaderList;

};

#endif // _SHADER_H_