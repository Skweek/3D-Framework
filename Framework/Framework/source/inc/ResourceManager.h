#ifndef _RESOURCE_MANAGER_H_
#define _RESOURCE_MANAGER_H_

#include "Utility.h"
#include <map>

class FBXFile;
class BitmapFont;

class ResourceManager
{
public:
	static void Create();
	static void Destroy();
	static ResourceManager* Get();

	FBXFile* LoadFBX(const char* a_ccFile, int a_iScale);
	GLuint LoadTexture(const char* a_ccTexture);
	BitmapFont* LoadFont(const char* a_ccFont);

protected:
	static ResourceManager* m_pInstance;

	ResourceManager();
	~ResourceManager();
	
	// Map of FBXs
	std::map<const char*, FBXFile*> m_pFBXMap;

	// Map of textures
	std::map<const char*, GLuint> m_pTextureMap;

	// Map of Fonts
	std::map<const char*, BitmapFont*> m_pFontMap;
};

#endif // _RESOURCE_MANAGER_H_