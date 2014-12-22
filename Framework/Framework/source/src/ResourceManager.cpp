#include "ResourceManager.h"

#include "FBXLoader.h"
#include "BitmapFont.h"
#include <SOIL.h>

ResourceManager* ResourceManager::m_pInstance = nullptr;

void ResourceManager::Create()
{
	if( m_pInstance == nullptr )
		m_pInstance = new ResourceManager();
}

void ResourceManager::Destroy()
{
	if( m_pInstance != nullptr )
		delete m_pInstance;
}

ResourceManager* ResourceManager::Get()
{
	return m_pInstance;
}

ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{
	std::map<const char*, FBXFile*>::iterator pIterFBX = m_pFBXMap.begin();
	for(; pIterFBX != m_pFBXMap.end(); ++pIterFBX)
	{
		pIterFBX->second->unload();
	}

	std::map<const char*, GLuint>::iterator pIterTexture = m_pTextureMap.begin();
	for(; pIterTexture != m_pTextureMap.end(); ++pIterTexture)
	{
		glDeleteTextures(1, &pIterTexture->second);
	}

	std::map<const char*, BitmapFont*>::iterator pIterFont = m_pFontMap.begin();
	for(; pIterFont != m_pFontMap.end(); ++pIterFont)
	{
		delete pIterFont->second;
	}
}

FBXFile* ResourceManager::LoadFBX(const char* a_ccFile, int a_iScale)
{
	if(m_pFBXMap.find(a_ccFile) == m_pFBXMap.end())
	{		
		FBXFile* pFBX = new FBXFile();
		if (!(pFBX->load(a_ccFile, FBXFile::UNIT_SCALE(a_iScale))))
		{
			printf("Failed to load file: %s \n", a_ccFile);
			return nullptr;
		}

		m_pFBXMap.insert(std::pair<const char*, FBXFile*>(a_ccFile, pFBX));

		return pFBX;
	}
	else
	{
		return m_pFBXMap[a_ccFile];
	}

}

GLuint ResourceManager::LoadTexture(const char* a_ccFilePath)
{
	if(m_pTextureMap.find(a_ccFilePath) == m_pTextureMap.end())
	{		
		GLuint pTexture = SOIL_load_OGL_texture(a_ccFilePath, 4, 0, SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_INVERT_Y);

		m_pTextureMap.insert(std::pair<const char*, GLuint>(a_ccFilePath, pTexture));

		return pTexture;
	}
	else
	{
		return m_pTextureMap[a_ccFilePath];
	}
}

BitmapFont* ResourceManager::LoadFont(const char* a_ccFont)
{
	if(m_pFontMap.find(a_ccFont) == m_pFontMap.end())
	{		
		BitmapFont* pFont = new BitmapFont();

		if (!(pFont->LoadFont(a_ccFont)))
		{
			printf("Failed to load file: %s \n", a_ccFont);
			return nullptr;
		}

		m_pFontMap.insert(std::pair<const char*, BitmapFont*>(a_ccFont, pFont));

		return pFont;
	}
	else
	{
		return m_pFontMap[a_ccFont];
	}
}