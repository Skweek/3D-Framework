#ifndef _APP_H_
#define _APP_H_

#define _CRTDBG_MAP_ALLOC

struct GLFWwindow;

class ChatBox;

class Application
{
public:
	Application();
	virtual ~Application();
	
	/* Create function inits app and returns true if successful */
	/* Parameters are window title, resolution, fullscreen(default false) */
	bool Create(const char* a_ccTitle, int a_iWidth, int a_iHeight, bool a_bFullScreen = false );

	/* Update function is main app loop */
	void Loop();
	void Quit();

protected:
	// Virtual Init function to be defined by program using library
	virtual void	Init() = 0;
	// Virtual update function to be defined by program using library
	virtual void	Update(float a_fDelta) = 0;
	// Virtual render function to be defined by program using library
	virtual void	Render() = 0;
	// Virtual destroy function to be defined by program using library
	virtual void	Destroy() = 0;
	

	bool			m_bRunning;
	GLFWwindow*		m_pCurrentWindow;

	double			m_dCurTime;
	double			m_fDeltaTime;
	double			m_fTotalTime;

	float			m_fValue;

	ChatBox*		m_pChat;
};


#endif //_APP_H_