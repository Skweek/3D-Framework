#include "Application.h"
#include <glm/glm.hpp>

// Forward declarations from library
class Plane3D;
class Model;

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

	Plane3D* m_testPlane;
	Model* m_testModel;
};