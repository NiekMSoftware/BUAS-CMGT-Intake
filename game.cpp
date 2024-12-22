// Template, 2024 IGAD Edition
// Get the latest version from: https://github.com/jbikker/tmpl8
// IGAD/NHTV/BUAS/UU - Jacco Bikker - 2006-2024

#include "precomp.h"
#include "game.h"

Player* player;
AsteroidPool* asteroidPool;

// -----------------------------------------------------------
// Initialize the application
// -----------------------------------------------------------
void Game::Init()
{
	GameWorld::instance().initialize();

	player = new Player();
	GameWorld::instance().addObject(player);

	// Initialize asteroid pool with a max of 4 asteroids
	asteroidPool = new AsteroidPool(4);
	spawnInitialAsteroids();
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
}

// -----------------------------------------------------------
// Main application shutdown function - Executed at the end of the application.
// -----------------------------------------------------------
void Game::Shutdown()
{
	GameWorld::instance().clean();

	delete asteroidPool;
}

void Game::spawnInitialAsteroids()
{
	for (int i = 0; i < 3; i++)
	{
		float2 randomPos = {
			static_cast<float>(rand() % screen->width),
			static_cast<float>(rand() % screen->height)
		};

		GameObject* asteroid = asteroidPool->spawnAsteroid(AsteroidSize::Large, randomPos);
		GameWorld::instance().addObject(asteroid);
	}
}
