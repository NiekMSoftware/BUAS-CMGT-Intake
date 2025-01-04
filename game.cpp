// Template, 2024 IGAD Edition
// Get the latest version from: https://github.com/jbikker/tmpl8
// IGAD/NHTV/BUAS/UU - Jacco Bikker - 2006-2024

#include "precomp.h"
#include "game.h"

// -----------------------------------------------------------
// Initialize the application
// -----------------------------------------------------------
void Game::Init()
{
    setupGameManager();
    initWorld();
    initWaves();
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

void Game::setupGameManager()
{
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

    GameManager::instance().setScoreLabel(scoreLabel);
    GameManager::instance().setScoreMultiplierLabel(scoreMultiplierLabel);
    GameManager::instance().instantiate();
}

void Game::initWorld()
{
    GameWorld::instance().initialize();

    // load objects
    player = new Player();

    lifeLabel = new Label{
        std::format("Lives: {}", player->getLives()),
        float2(10, 10),
        0xFFFFFF
    };

    GameWorld::instance().addObject(player);
    GameWorld::instance().addObject(scoreLabel);
    GameWorld::instance().addObject(scoreMultiplierLabel);
    GameWorld::instance().addObject(lifeLabel);

    // set the life label
    GameManager::instance().setLivesLabel(lifeLabel);
    GameManager::instance().updateLivesDisplay(player->getLives());
}

void Game::initWaves()
{
    asteroidPool = new AsteroidPool{ MAX_LARGE_ASTEROIDS };
    WaveSystem::instance().initialize(asteroidPool);
    WaveSystem::instance().startWave();

    CollisionSystem::instance().initialize();
    WaveSystem::instance().spawnAsteroidWave();
}
