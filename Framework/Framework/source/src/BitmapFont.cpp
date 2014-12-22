#include "BitmapFont.h"

#include "ResourceManager.h"

BitmapFont::BitmapFont()
{

}

BitmapFont::~BitmapFont()
{
	delete m_pCharacterList;
}

bool BitmapFont::LoadFont(const char* a_ccFontFile)
{
	// Try to open file first
	FILE* fh = fopen( a_ccFontFile, "r" );
	if( fh == NULL ) return false;

	// Okay, file works, now try to get it as XML
	XML::XMLDocument font;
	if( font.LoadFile( a_ccFontFile ) == 0 )
	{
		//Set up pointers
		XML::XMLNode* pFontNode;
		XML::XMLElement* pInfoNode;
		XML::XMLElement* pCommonNode;
		XML::XMLElement* pPageNode;
		XML::XMLElement* pCharsNode;
		XML::XMLElement* pCharsElement;

		// Start looking through the xml and sorting out
		// all the information
		if( ( pFontNode = font.FirstChildElement( "font" ) ) == NULL )
		{
			printf( "Error: Failed to find node 'font'\n" );
			return false;
		}

		if( ( pInfoNode = pFontNode->FirstChildElement( "info" ) ) == NULL )
		{
			printf( "Error: Failed to find element 'info'\n" );
			return false;
		}

		// Go through all relevant elements and store information
		// face, size, bold, italic, unicode, stretchH, smooth, 
		// aa, outline
		m_ccFontFace = pInfoNode->FirstAttribute()->Value();

		m_iSize = pInfoNode->IntAttribute( "size" );

		m_bBold = pInfoNode->BoolAttribute( "bold" );
		m_bItalic = pInfoNode->BoolAttribute( "italic" );

		m_iStretchH = pInfoNode->IntAttribute( "stretchH" );
		m_iSmooth = pInfoNode->IntAttribute( "smooth" );
		m_iAA = pInfoNode->IntAttribute( "aa" );
		m_iOutline = pInfoNode->IntAttribute( "outline" );

		if( ( pCommonNode = pFontNode->FirstChildElement( "common" ) ) == NULL )
		{
			printf( "Error: Failed to find element 'common'\n" );
			return false;
		}
		
		// Go through all relevant elements and store information
		// lineheight, base, scaleW, scaleH, packed, Channels
		m_iLineHeight = pCommonNode->IntAttribute( "lineHeight" );
		m_iBase = pCommonNode->IntAttribute( "base" );

		m_iScaleW = pCommonNode->IntAttribute( "scaleW" );
		m_iScaleH = pCommonNode->IntAttribute( "scaleH" );

		m_iPacked = pCommonNode->IntAttribute( "packed" );

		m_pChannels[0] = (float)pCommonNode->IntAttribute( "alphaChnl" );
		m_pChannels[1] = (float)pCommonNode->IntAttribute( "redChnl" );
		m_pChannels[2] = (float)pCommonNode->IntAttribute( "greenChnl" );
		m_pChannels[3] = (float)pCommonNode->IntAttribute( "blueChnl" );
		
		// Get the font texture image
		if( ( pPageNode = pFontNode->FirstChildElement( "pages" ) ) == NULL )
		{
			printf( "Error: Failed to find node 'pages'\n" );
			return false;
		}

		const char* ccFontImage = pPageNode->FirstChildElement( "page" )->Attribute( "file" );
		char filepath[256];
		strcpy( filepath, "../fonts/tnr/" );
		m_uiFontTexture = ResourceManager::Get()->LoadTexture( strcat( filepath, ccFontImage ) );

		// Now to create an array to store the characters in
		
		if( ( pCharsNode = pFontNode->FirstChildElement( "chars" ) ) == NULL )
		{
			printf( "Error: Failed to find node 'chars'\n" );
			return false;
		}
		
		m_iNumCharacters = pCharsNode->IntAttribute( "count" );
		m_pCharacterList = ( Utility::BitmapFontCharacter* )malloc( sizeof( Utility::BitmapFontCharacter ) * m_iNumCharacters );

		pCharsElement = pCharsNode->FirstChildElement( "char" );
		
		for( int i = 0; i < m_iNumCharacters; ++i )
		{
			m_pCharacterList[i].id = pCharsElement->IntAttribute( "id" );
			m_pCharacterList[i].x = pCharsElement->FloatAttribute( "x" );
			m_pCharacterList[i].y = pCharsElement->FloatAttribute( "y" );
			m_pCharacterList[i].width = pCharsElement->FloatAttribute( "width" );
			m_pCharacterList[i].height = pCharsElement->FloatAttribute( "height" );
			m_pCharacterList[i].xOffset = pCharsElement->FloatAttribute( "xoffset" );
			m_pCharacterList[i].yOffset = pCharsElement->FloatAttribute( "yoffset" );
			m_pCharacterList[i].xAdvance = pCharsElement->FloatAttribute( "xadvance" );
			m_pCharacterList[i].channel = pCharsElement->FloatAttribute( "chnl" );

			pCharsElement = pCharsElement->NextSiblingElement( "char" );
		}
		return true;
	}
	return false;
}