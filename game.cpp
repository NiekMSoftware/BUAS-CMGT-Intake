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
    setupGame();
    initWorld();

	ResourceHolder::Instance().LoadSprite("assets/star_field.png", "star_field", 1);
	starField = ResourceHolder::Instance().GetSprite("star_field");

	GameManager::instance().updateWaveDisplay();
}

// -----------------------------------------------------------
// Main application tick function - Executed once per frame
// -----------------------------------------------------------
void Game::Tick()
{
	handleGameState();
}

void Game::handleGameState()
{
	switch (GameManager::instance().getCurrentState())
	{
	case InMenu:
		if (Input::getKeyDown(GLFW_KEY_ENTER))
		{
			GameManager::instance().setGameState(Playing);
		}
		break;

	case Playing:
		GameManager::instance().updateClusterDisplay();  // update this every frame for deltaTime reasons
		GameManager::instance().update();
		GameWorld::instance().update();
		CollisionSystem::instance().update();
		break;

	case GameOver:
		if (Input::getKeyDown(GLFW_KEY_R))
		{
			reset();
		}
		break;
	}
}

// -----------------------------------------------------------
// Main application fixed tick function - Executed once per interval
// -----------------------------------------------------------
void Game::FixedTick()
{
	if (GameManager::instance().getCurrentState() == Playing)
		GameWorld::instance().fixedUpdate();
}

// -----------------------------------------------------------
// Main application render function - Executed once per frame
// -----------------------------------------------------------
void Game::Render()
{
	if (GameManager::instance().getCurrentState() == InMenu)
	{
		screen->Clear(0);
		screen->Print(APP_NAME, SCRWIDTH / 2 - 25, SCRHEIGHT / 2, 0xFFFFFF);
		screen->Print("Press 'Enter' to start", SCRWIDTH / 2 - 60, SCRHEIGHT / 2 + 15, 0xFFFFFF);
	}
	else if (GameManager::instance().getCurrentState() == Playing)
	{
		starField->Draw(screen, 0, 0);
		GameWorld::instance().render(screen);
	}
	else if (GameManager::instance().getCurrentState() == GameOver)
	{
		screen->Clear(0xFF0000);
		screen->Print("Game Over!", SCRWIDTH / 2 - 15, SCRHEIGHT / 2, 0xFFFFFF);
		screen->Print("Press 'R' to restart", SCRWIDTH / 2 - 40, SCRHEIGHT / 2 + 15, 0xFFFFFF);
	}
}

// -----------------------------------------------------------
// Main application shutdown function - Executed at the end of the application.
// -----------------------------------------------------------
void Game::Shutdown()
{
    GameManager::instance().clean();
	GameWorld::instance().clean();

	delete asteroidPool;

	// Delete the local star field
	starField = nullptr;
	delete starField;

	// Delete the player object
	player = nullptr;
	delete player;
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
	GameManager::instance().setPlayer(player);
    GameManager::instance().updateLivesDisplay();
}

void Game::reset()
{
	GameManager::instance().reset();
	WaveSystem::instance().reset();

	GameManager::instance().setGameState(Playing);
}
