// Template, 2024 IGAD Edition
// Get the latest version from: https://github.com/jbikker/tmpl8
// IGAD/NHTV/BUAS/UU - Jacco Bikker - 2006-2024

#include "precomp.h"
#include "game.h"

Player* player;
Ground* ground;

// -----------------------------------------------------------
// Initialize the application
// -----------------------------------------------------------
void Game::Init()
{ 
	player = new Player();
	ground = new Ground();

	float2 center = { SCRWIDTH / 2, SCRHEIGHT / 2 };
	ground->SetPosition(center);
	player->SetPosition(center);
}

// -----------------------------------------------------------
// Main application tick function - Executed once per frame
// -----------------------------------------------------------
void Game::Tick( float deltaTime )
{
	player->Update(deltaTime);
}

// -----------------------------------------------------------
// Main application render function - Executed once per frame
// -----------------------------------------------------------
void Game::Render()
{
	screen->Clear(0);

	ground->Draw(screen);
	player->Draw(screen);
}

// -----------------------------------------------------------
// Shutdown application and cleanup
// -----------------------------------------------------------
void Game::Shutdown()
{
	delete player;
	delete ground;
}