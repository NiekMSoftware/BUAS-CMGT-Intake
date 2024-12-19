// Template, 2024 IGAD Edition
// Get the latest version from: https://github.com/jbikker/tmpl8
// IGAD/NHTV/BUAS/UU - Jacco Bikker - 2006-2024

#include "precomp.h"
#include "game.h"

Player* player;

// -----------------------------------------------------------
// Initialize the application
// -----------------------------------------------------------
void Game::Init()
{
	GameWorld::instance().initialize();

	player = new Player();

	GameWorld::instance().addObject(player);
}

// -----------------------------------------------------------
// Main application tick function - Executed once per frame
// -----------------------------------------------------------
void Game::Tick()
{
	GameWorld::instance().update();
}

// -----------------------------------------------------------
// Main application fixed tick function - Executed once per interval
// -----------------------------------------------------------
void Game::FixedTick()
{
	GameWorld::instance().fixedUpdate();
}

// -----------------------------------------------------------
// Main application render function - Executed once per frame
// -----------------------------------------------------------
void Game::Render()
{
	screen->Clear(0);

	GameWorld::instance().render(screen);

	// template ui
	screen->Print("Score: I don't know, maybe 1... or 2...\n", 10, 10, 0xFFFFFFFF);
}

// -----------------------------------------------------------
// Main application shutdown function - Executed at the end of the application.
// -----------------------------------------------------------
void Game::Shutdown()
{
	GameWorld::instance().clean();
}
