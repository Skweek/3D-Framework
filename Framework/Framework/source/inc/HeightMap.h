#ifndef _HEIGHT_MAP_H_
#define _HEIGHT_MAP_H_

#include "Utility.h"

class HeightMap
{
public:
	HeightMap();
	~HeightMap();

protected:
	void	Generate(unsigned int a_uiSeed);

};

#endif //_HEIGHT_MAP_H_