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
    // load game world
    GameWorld::instance().initialize();
    GameManager::instance().instantiate();

    // load objects
    player = new Player();

    scoreLabel = new Label{
    	std::format("Score: {}", GameManager::instance().getScore()),
    	float2(700, 10),
    	0xFFFFFF };

	scoreMultiplierLabel = new Label{
    	std::format("Multiplier: {:.1f}", GameManager::instance().getScoreMultiplier()),
    	float2(700, 25),
    	0xFFFFFF
    };

    lifeLabel = new Label{
    	std::format("Lives: {}", player->getLives()),
    	float2(10, 10),
    	0xFFFFFF
    };

    GameWorld::instance().addObject(player);
    GameWorld::instance().addObject(scoreLabel);
    GameWorld::instance().addObject(scoreMultiplierLabel);
    GameWorld::instance().addObject(lifeLabel);

    // set the label references for the game manager
    GameManager::instance().setScoreLabel(scoreLabel);
    GameManager::instance().setScoreMultiplierLabel(scoreMultiplierLabel);
    GameManager::instance().setLivesLabel(lifeLabel);
    GameManager::instance().updateLivesDisplay(player->getLives());

    // Initialize asteroid pool with a max of 4 big asteroids
    asteroidPool = new AsteroidPool{ MAX_LARGE_ASTEROIDS };
    spawnInitialAsteroids();

    CollisionSystem::instance().initialize();
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
	GameWorld::instance().clean();
    GameManager::instance().clean();

	delete asteroidPool;
}

// -----------------------------------------------------------
// Main application asteroid spawner - Executed at the start of the game.
// -----------------------------------------------------------
void Game::spawnInitialAsteroids()
{
    const float minDistToPlayer = 150.0f;
    const float minDistBetweenAsteroids = 100.0f;
    const int numAsteroids = 2;
    const int maxAttempts = 10;

    // Get player position
    float2 playerPos = player->getPosition();

    // Keep track of successfully spawned asteroid positions
    std::vector<float2> spawnedPositions;

    // Spawn our asteroids
    for (int i = 0; i < numAsteroids; i++)
    {
        bool validPosition = false;
        int attempts = 0;
        float2 spawnPos;

        // Keep trying until we find a valid position or run out of attempts
        while (!validPosition && attempts < maxAttempts)
        {
            // Get a candidate position
            spawnPos = Random::getRandomPositionAwayFrom(
                playerPos,
                minDistToPlayer,
                static_cast<float>(screen->width),
                static_cast<float>(screen->height)
            );

            // Assume position is valid until proven otherwise
            validPosition = true;

            // Check against all previously spawned asteroids
            for (const float2& existingPos : spawnedPositions)
            {
                float distance = length(spawnPos - existingPos);
                if (distance < minDistBetweenAsteroids)
                {
                    validPosition = false;
                    break;
                }
            }

            attempts++;
        }

        // If we found a valid position, spawn the asteroid
        if (validPosition)
        {
            GameObject* asteroid = asteroidPool->spawnAsteroid(AsteroidSize::Large, spawnPos);
            if (!asteroid)
            {
                std::cerr << "failed to spawn asteroid\n";
                continue;
            }

            spawnedPositions.push_back(spawnPos);
        }
        else
        {
            std::cerr << "Could not find valid spawn position for asteroid " << i << "\n";
        }
    }
}
