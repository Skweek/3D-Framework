#include "main.h"
#include <string>

// Library includes
#include <Scene.h>
#include <Renderable\3DPlane.h>
#include <Renderable\Model.h>
#include <ShaderManager.h>

int main(int argc, char* argv[])
{
	Application* game = new Game();

	// Try and create instance
	if(game->Create("Test", 1280, 720, false))
		game->Loop();

	delete game;

	_CrtDumpMemoryLeaks();

	return 0;
}

Game::Game()
{
		
}

Game::~Game()
{

}

void Game::Init()
{
	m_testPlane = new Plane3D(glm::vec3(0), glm::vec2(150,150), ShaderManager::Get()->GetShader("3DPlane"));	
	m_testModel = new Model("../models/tree/tree.fbx", 0, false, 1);
	m_testModel->SetScale(0.1f);
}

void Game::Update(float a_fDelta)
{
	m_testPlane->Update(a_fDelta);
	m_testModel->Update(a_fDelta);
}

void Game::Render()
{
	Scene::Get()->AddChild(m_testPlane);
	Scene::Get()->AddChild(m_testModel);
}

void Game::Destroy()
{

}