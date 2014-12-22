#ifndef _MODEL_H_
#define _MODEL_H_

#include "RenderableObject.h"
#include "FBXLoader.h"
#include <list>
#include <vector>

class Model : public RenderableObject
{
public:
	Model(const char* a_ccFilePath, unsigned int a_uiShader, bool a_bAnimated = false, int a_iScale = FBXFile::UNITS_METER);
	Model(Model* a_pModel);
	virtual ~Model();

	void						SetScale(float a_fScale);

	void						Update(float a_fDelta);
	void						Draw();

protected:
	void						Init();

	FBXFile*					m_pFBX;

	bool						m_bAnimated;
	float						m_fScale;
};

#endif // _MODEL_H_