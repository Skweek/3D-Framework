#include "Application.h"
#include <glm/glm.hpp>

// Forward declarations from library
class Terrain;

class Game : public Application
{
public:
	Game();
	virtual ~Game();

protected:
	virtual void	Init();
	virtual void	Update(float a_fDelta);
	virtual void	Render();
	virtual void	Destroy();

	Terrain*		m_pTerrain;
};