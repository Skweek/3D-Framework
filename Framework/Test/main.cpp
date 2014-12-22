#include "main.h"
#include <string>

// Library includes
#include <Scene.h>
#include <TerrainGeneration.h>

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
	m_pTerrain = new Terrain(glm::vec3(0,0,0), glm::vec2(256,256), 6, 1.f);
}

void Game::Update(float a_fDelta)
{

}

void Game::Render()
{
	Scene::Get()->AddChild(m_pTerrain);
	m_pTerrain->DrawNavMesh();
}

void Game::Destroy()
{
	delete m_pTerrain;
}