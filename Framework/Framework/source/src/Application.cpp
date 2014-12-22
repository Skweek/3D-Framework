
#include "Application.h"
#include "Camera.h"
#include "ShaderManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "LightPass.h"
#include "TerrainGeneration.h"

// objects
#include "Renderable\Model.h"
#include "Renderable\Quad.h"
#include "Renderable\Volume.h"
#include "Renderable\3DPlane.h"

// UI
#include "UI\UIButton.h"
#include "UI\UISlider.h"

#include "ChatBox.h"

#include "Utility.h"

#include <string>

Application::Application()
{
	m_fDeltaTime = 0;
	m_fTotalTime = 0;
	m_dCurTime = 0;
}

Application::~Application()
{
	Camera::Destroy();
	LightPass::Destroy();
	ShaderManager::Destroy();
	ResourceManager::Destroy();
	Scene::Destroy();

	glfwDestroyWindow(Utility::GetWindow());
	glfwTerminate();
}

void Resize(GLFWwindow* a_pWindow, int a_iWidth, int a_iHeight)
{
	a_pWindow;
	Camera::Get()->SetProjection(glm::perspective(glm::pi<float>() * 0.25f, a_iWidth/(float)a_iHeight, 0.1f, 1000000.0f));
}

void Application::Quit()
{
	m_bRunning = false;
}

bool Application::Create(const char* a_ccTitle, int a_iWidth, int a_iHeight, bool a_bFullScreen)
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//_CrtSetBreakAlloc(31344);

	m_fValue = 0.f;

	// Init GLFW
	if(glfwInit() != GL_TRUE)
	{
		printf("Unable to initialise GLFW \n");
		return false;
	}

	// Initialise Camera and set matrices
	Camera::Create();
	Camera::Get()->SetCamera(glm::inverse( glm::lookAt(glm::vec3(10,10,10),glm::vec3(0,0,0), glm::vec3(0,1,0)) ));
	Camera::Get()->SetProjection(glm::perspective(glm::pi<float>() * 0.25f, a_iWidth/(float)a_iHeight, 0.1f, 1000000.0f));

	m_fDeltaTime = 0;
	m_fTotalTime = 0;
	m_dCurTime = 0;




	// Create window
	if(!a_bFullScreen)
		Utility::SetWindow(glfwCreateWindow(a_iWidth, a_iHeight, a_ccTitle, nullptr, nullptr));
	// If window is not created terminate
	if(Utility::GetWindow() == nullptr)
	{
		printf("Unable to create window \n");
		glfwTerminate();
		return false;
	}

	// Set the window resize callback function
	glfwSetWindowSizeCallback(Utility::GetWindow(), Resize);

	// Init GLEW
	glfwMakeContextCurrent(Utility::GetWindow());
	glewExperimental = true;
	if(glewInit() != GLEW_OK)
	{
		printf("Unable to create glew context \n");
		glfwTerminate();
		return false;
	}

	printf("%s \n", glGetString(GL_VERSION));

	// V-Sync off
	glfwSwapInterval(0);

	// Start loop
	m_bRunning = true;

	// Init Resource Manager
	ResourceManager::Create();

	// Initialise ShaderManager and add the model shader
	ShaderManager::Create();
	ShaderManager::Get()->AddShader("Comp", "../shaders/Default Shaders/CompShader.vert", "../shaders/Default Shaders/CompShader.frag", nullptr, nullptr, nullptr);
	ShaderManager::Get()->AddShader("Model", "../shaders/Default Shaders/Model.vert", "../shaders/Default Shaders/Model.frag", nullptr, nullptr, nullptr);
	ShaderManager::Get()->AddShader("Simple", "../shaders/Default Shaders/simple.vert", "../shaders/Default Shaders/simple.frag", nullptr, nullptr, nullptr);
	ShaderManager::Get()->AddShader("Lines", "../shaders/Default Shaders/lines.vert", "../shaders/Default Shaders/lines.frag", nullptr, nullptr, nullptr);
	ShaderManager::Get()->AddShader("3DPlane", "../shaders/Default Shaders/3DPlane.vert", "../shaders/Default Shaders/3DPlane.frag", nullptr, nullptr, nullptr);
	ShaderManager::Get()->AddShader("Water", "../shaders/Default Shaders/water.vert", "../shaders/Default Shaders/water.frag", nullptr, "../shaders/Default Shaders/water.cont", "../shaders/Default Shaders/water.eval");
	ShaderManager::Get()->AddShader("Light", "../shaders/Default Shaders/LightPass.vert", "../shaders/Default Shaders/LightPass.frag", nullptr, nullptr, nullptr);
	ShaderManager::Get()->AddShader("Shadow", "../shaders/Default Shaders/ShadowMap.vert", "../shaders/Default Shaders/ShadowMap.frag", nullptr, nullptr, nullptr);
	ShaderManager::Get()->AddShader("VolumeRender", "../shaders/Default Shaders/marchingCubes.vert", "../shaders/Default Shaders/marchingCubes.frag", "../shaders/Default Shaders/marchingCubes.geom", nullptr, nullptr);
	ShaderManager::Get()->AddShader("Terrain", "../shaders/Default Shaders/Terrain.vert", "../shaders/Default Shaders/Terrain.frag", nullptr, "../shaders/Default Shaders/Terrain.cont", "../shaders/Default Shaders/Terrain.eval");
	ShaderManager::Get()->AddShader("TerrainGen", "../shaders/Default Shaders/TerrainGen.vert", "../shaders/Default Shaders/TerrainGen.frag", nullptr, nullptr, nullptr);
	ShaderManager::Get()->AddShader("Font", "../shaders/Default Shaders/BitmapFont.vert", "../shaders/Default Shaders/BitmapFont.frag", nullptr, nullptr, nullptr);
	

	Scene::Create(ShaderManager::Get()->GetShader("Comp"));

	LightPass::Create(ShaderManager::Get()->GetShader("Light"));
	//LightPass::Get()->AddDirectional(glm::vec3(1,-0.5,0), glm::vec3(1));
	LightPass::Get()->AddDirectional(glm::vec3(0,-1,0), glm::vec3(1));
	//LightPass::Get()->AddPoint(glm::vec3(5.7,32,-86), glm::vec3(1), 50);
		
	m_pChat = new ChatBox();
	m_pChat->Initialise(glm::vec2(SCREENWIDTH - 500, SCREENHEIGHT - 200), glm::vec2(450,150));
	m_pChat->AddText("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbccccccccccccccccccccccccccccccccccccccccccccdddddddddddddddddddddddddddddddddeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeefffffffffffffffffffffffffffffffff");
	
	

	// Call the virtual Init function
	Init();
	
	return true;
}

void Application::Loop()
{
	static int frames = 0;
	static float time = 0;

	do
	{
		// game tick
		Utility::GameTick();

		float fDelta = Utility::GetDelta();

		// FPS
		++frames;
		time += fDelta;
		if( time > 1.0f )
		{
			printf("%i \n", frames);
			printf("Time running: %f, Delta Time: %f\n", Utility::GetTotalTime(), fDelta);
			frames = 0;
			time = 0;
		}
		
		// move camera
		Camera::Get()->Update();
		
		glClearColor(0.f,0.f,0.f,0.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// Set view
		glm::mat4 viewMatrix = glm::inverse( Camera::Get()->GetCamera() );
		Camera::Get()->SetView(viewMatrix);	

		// Call library virtual update function
		Update((float)fDelta);
		// Call library render function
		Render();

		// call scene update
		Scene::Get()->UpdateChildren((float)fDelta);
		// Call scene render
		Scene::Get()->RenderChildren();

		glfwSwapBuffers(Utility::GetWindow());
		glfwPollEvents();

	} while (m_bRunning && (glfwWindowShouldClose(Utility::GetWindow()) == 0));

	Destroy();
}
