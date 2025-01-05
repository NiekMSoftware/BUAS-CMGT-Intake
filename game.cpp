// Template, 2024 IGAD Edition
// Get the latest version from: https://github.com/jbikker/tmpl8
// IGAD/NHTV/BUAS/UU - Jacco Bikker - 2006-2024

#include "precomp.h"
#include "game.h"

// TODO: Blink the 'Cluster Incoming' text
// TODO: Make sure the asteroids spawn indifferent from one another and away from the player

// -----------------------------------------------------------
// Initialize the application
// -----------------------------------------------------------
void Game::Init()
{
    setupGame();
    initWorld();
    initWaves();

	GameManager::instance().updateWaveDisplay();
}

// -----------------------------------------------------------
// Main application tick function - Executed once per frame
// -----------------------------------------------------------
void Game::Tick()
{
	GameWorld::instance().update();
    CollisionSystem::instance().update();
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
    GameManager::instance().clean();
	GameWorld::instance().clean();

	delete asteroidPool;
}

void Game::setupLabels()
{
	lifeLabel = new Label{
		std::format("Lives: {}", 0),
		float2(10, 10),
		0xFFFFFF
	};

	scoreLabel = new Label{
		std::format("Score: {}", 0),
		float2{700, 10},
		0xFFFFFF
	};

	scoreMultiplierLabel = new Label{
		std::format("Multiplier: {:.1f}", 1.0f),
		float2{700, 25},
		0xFFFFFF
	};

	waveLabel = new Label{
		std::format("Wave {}", 0),
		float2{SCRWIDTH / 2.f - 15, 10},
		0xFFFFFF
	};

	clusterLabel = new Label{
		"",
		float2{SCRWIDTH / 2.f - 40, 25},
		0xFFFFFF
	};
}

void Game::setupGame()
{
    setupLabels();

    GameManager::instance().setLivesLabel(lifeLabel);
    GameManager::instance().setScoreLabel(scoreLabel);
    GameManager::instance().setScoreMultiplierLabel(scoreMultiplierLabel);
    GameManager::instance().setWaveLabel(waveLabel);
    GameManager::instance().setClusterLabel(clusterLabel);

    GameManager::instance().instantiate();
}

void Game::initWorld()
{
    GameWorld::instance().initialize();
	CollisionSystem::instance().initialize();

    // load objects
    player = new Player();

    GameWorld::instance().addObject(player);
    GameWorld::instance().addObject(scoreLabel);
    GameWorld::instance().addObject(scoreMultiplierLabel);
    GameWorld::instance().addObject(lifeLabel);
    GameWorld::instance().addObject(waveLabel);
    GameWorld::instance().addObject(clusterLabel);

	asteroidPool = new AsteroidPool{ MAX_LARGE_ASTEROIDS };
	WaveSystem::instance().initialize(asteroidPool);

	WaveSystem::instance().startWave();
	WaveSystem::instance().spawnAsteroidWave();

    // display player info
    GameManager::instance().updateLivesDisplay(player->getLives());
}

void Game::initWaves()
{
    
}
