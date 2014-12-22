#include "Renderable/Text.h"
#include "BitmapFont.h"
#include "Camera.h"

Text::Text(const char* a_ccText, glm::vec4 a_pPos, glm::vec4 a_pCol, unsigned int a_uiShader, BitmapFont* a_pFont, float a_fScale)
{
	m_pFont = a_pFont;
	m_ccTextString = a_ccText;
	m_pPos = a_pPos;
	m_pCol = a_pCol;
	m_fScale = a_fScale;

	m_uiShaderProgram = a_uiShader;

	Init();
}

Text::~Text()
{
	glDeleteBuffers(1, &m_pRenderData->m_uiVBO);
	glDeleteBuffers(1, &m_pRenderData->m_uiIBO);
	glDeleteVertexArrays(1, &m_pRenderData->m_uiVAO);

	glDeleteProgram(m_uiShaderProgram);
	delete m_pRenderData;
}

void Text::Init()
{
	// create quad to render to in the end
	float points[] = { 0, 0, 0, 0, 0,
						1, 0, 0, 1, 0,
						0, 1, 0, 0, 1,

						0, 1, 0, 0, 1,
						1, 0, 0, 1, 0,
						1, 1, 0, 1, 1};

	m_pRenderData = new Utility::RenderData();
	glGenBuffers(1, &m_pRenderData->m_uiVBO);
	glGenVertexArrays(1, &m_pRenderData->m_uiVAO);

	glBindVertexArray(m_pRenderData->m_uiVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_pRenderData->m_uiVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(3*sizeof(float)));

	// unbind and next mesh
	glBindVertexArray(0);
}

void Text::Update(float a_fDelta){a_fDelta;}

void Text::Draw()
{
	glUseProgram(m_uiShaderProgram);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLuint ProjectionID = glGetUniformLocation(m_uiShaderProgram, "Projection");	
	glm::mat4 ortho = glm::ortho(0.f, (float)SCREENWIDTH, 0.f, (float)SCREENHEIGHT, -1.f, 1.f);
	glUniformMatrix4fv(ProjectionID, 1, false, glm::value_ptr(ortho));

	glBindTexture(GL_TEXTURE_2D, m_pFont->m_uiFontTexture);
	if(m_ccTextString != NULL)
	{
		float fCurXOffset = 0.f;

		int iSize = strlen(m_ccTextString);
		int iFirstID = m_pFont->m_pCharacterList[0].id;
		for(int c = 0; c < iSize; ++c)
		{
			int iCurChar = m_ccTextString[c] - iFirstID;			
			Utility::BitmapFontCharacter* pChar = &m_pFont->m_pCharacterList[iCurChar];	
			
			glm::mat4 m_pModel = glm::mat4(	pChar->width * m_fScale,	0,			0,	0,
											0,			pChar->height * m_fScale,	0,	0,
											0,			0,			1,		0,
											m_pPos.x + (fCurXOffset * m_fScale),	SCREENHEIGHT - m_pPos.y - (pChar->height * m_fScale),	0.5,	1);
			
			GLuint ModelID = glGetUniformLocation(m_uiShaderProgram, "Model");	
			glUniformMatrix4fv(ModelID, 1, false, glm::value_ptr(m_pModel));

			float points[] = {	0, 0, 0, 
								pChar->x / m_pFont->m_iScaleW,						(pChar->y + pChar->height) / m_pFont->m_iScaleH,  // 0
								1, 0, 0, 
								(pChar->x + pChar->width) / m_pFont->m_iScaleW,		(pChar->y + pChar->height) / m_pFont->m_iScaleH,  // 1
								0, 1, 0, 
								pChar->x / m_pFont->m_iScaleW,						pChar->y / m_pFont->m_iScaleH,  // 2
												 
								1, 0, 0,
								(pChar->x + pChar->width) / m_pFont->m_iScaleW,		(pChar->y + pChar->height) / m_pFont->m_iScaleH,  // 1
								1, 1, 0,
								(pChar->x + pChar->width) / m_pFont->m_iScaleW,		pChar->y / m_pFont->m_iScaleH, // 3
								0, 1, 0, 
								pChar->x / m_pFont->m_iScaleW,						pChar->y / m_pFont->m_iScaleH  // 2
							};


			glBindBuffer(GL_ARRAY_BUFFER, m_pRenderData->m_uiVBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);

			glBindVertexArray(m_pRenderData->m_uiVAO);

			glDrawArrays( GL_TRIANGLES, 0, 6);

			fCurXOffset += pChar->xAdvance;
		}
	}
	
	glUseProgram( 0 );
	glBindTexture( GL_TEXTURE_2D, 0 );
	glBindVertexArray( 0 );
	glDisable(GL_BLEND);
}