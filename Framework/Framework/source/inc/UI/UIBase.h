#ifndef _UI_BASE_H_
#define _UI_BASE_H_

#include "Utility.h"
#include "RenderableObject.h"

class UIBase : public RenderableObject
{
public:
	UIBase();
	~UIBase();

protected:
	void		Draw();
	void		Update(float a_fDelta);
	void		Init();
};

#endif //_UI_BASE_H_