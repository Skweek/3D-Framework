#ifndef _BITMAP_FONT_H_
#define _BITMAP_FONT_H_

#include <tinyxml2.h>
#define XML tinyxml2 // shorthand
#include "Utility.h"

struct Font
{

};

class BitmapFont
{
public:
	BitmapFont();
	~BitmapFont();

	friend class Text;

	bool LoadFont(const char* a_ccFontFile);

protected:

	const char*						m_ccFontFace;
	GLuint							m_uiFontTexture;

	int								m_iSize;
	int								m_iNumCharacters;
	Utility::BitmapFontCharacter*	m_pCharacterList;

	bool							m_bBold;
	bool							m_bItalic;
	bool							m_bUnicode;

	int								m_iStretchH;
	int								m_iSmooth;
	int								m_iAA;

	int								m_iOutline;

	int								m_iLineHeight;
	int								m_iBase;
	int								m_iScaleW;
	int								m_iScaleH;
	int								m_iPacked;
	glm::vec4						m_pChannels;
};

#endif // _BITMAP_FONT_H_