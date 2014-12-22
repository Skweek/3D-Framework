#ifndef _LINES_H_
#define _LINES_H_

#include "RenderableObject.h"

class Lines : public RenderableObject
{
public:
	Lines(unsigned int a_uiShader);
	virtual ~Lines();

	void					Init();
	void					Update(float a_fDelta);
	void					Draw();

	void					AddLines(glm::vec3 a_pStart, glm::vec3 a_pEnd);
	void					Clear();

protected:
	struct Line
	{
		glm::vec3			m_pStart;
		glm::vec3			m_pEnd;
	};

	int						m_iNumberOfLines;
	Line*					m_pLines;

	RenderData*				m_pRenderData;

	

};

#endif //_LINES_H_