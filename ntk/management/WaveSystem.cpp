#include "precomp.h"
#include "WaveSystem.h"

void WaveSystem::initialize(AsteroidPool* asteroidPool)
{
	pool = asteroidPool;
	currentWave = 0;
	remainingAsteroids = 0;
	waveActive = false;
}

WaveConfig WaveSystem::generateWaveConfig() const
{
	WaveConfig config;

	// Scale difficulty based on wave number
	config.numLargeAsteroids = 1 + (currentWave / 2);  // Add one large asteroid every 2 waves
	config.speedMultiplier = (currentWave != 1) ? 1.0f + (static_cast<float>(currentWave) * 0.3f) : 1.0f;
	config.specialChance = std::min(0.3f, static_cast<float>(currentWave) * 0.05f);

	return config;
}

float2 WaveSystem::generateRandomPosition() const
{
	// generate a random position within screen bounds with a margin
	const float MARGIN = 50.0f;

	float x = Random::getRandomFloat(MARGIN, SCRWIDTH - MARGIN);
	float y = Random::getRandomFloat(MARGIN, SCRHEIGHT - MARGIN);

	return float2{ x, y };
}

void WaveSystem::startWave()
{
	currentWave++;
	currentWaveConfig = generateWaveConfig();
	remainingAsteroids = currentWaveConfig.numLargeAsteroids;
	waveActive = true;
}

void WaveSystem::spawnAsteroidWave() const
{
	if (remainingAsteroids <= 0) return;

	for (int i = 0; i < remainingAsteroids; ++i)
	{
		// Generate a position within the screen
		float2 position = generateRandomPosition();

		// Determine if this should be a special pattern spawn
		if (Random::getRandomFloat(0.0f, 1.0f) < currentWaveConfig.specialChance)
		{
			if (GameObject* asteroid = pool->spawnAsteroid(AsteroidSize::Large, position))
			{
				float2 currentVel = asteroid->getVelocity();
				asteroid->setVelocity(currentVel * currentWaveConfig.speedMultiplier);
			}

			// Create a cluster of asteroids, ensuring they stay within screen bounds
			for (int j = 0; j < 2; j++)
			{
				float offsetX = Random::getRandomFloat(-25.f, 25.f);
				float offsetY = Random::getRandomFloat(-25.f, 25.f);

				float2 offsetPos{
					std::clamp(position.x, offsetX, SCRWIDTH - 50.f),
					std::clamp(position.y, offsetY, SCRHEIGHT - 50.f)
				};

				if (GameObject* asteroid = pool->spawnAsteroid(AsteroidSize::Medium, offsetPos))
				{
					float2 currentVel = asteroid->getVelocity();
					asteroid->setVelocity(currentVel * currentWaveConfig.speedMultiplier);
				}
			}
		}
		else
		{
			if (GameObject* asteroid = pool->spawnAsteroid(AsteroidSize::Large, position))
			{
				float2 currentVel = asteroid->getVelocity();
				asteroid->setVelocity(currentVel * currentWaveConfig.speedMultiplier);
			}
		}
	}
}

void WaveSystem::onAsteroidReturned()
{
	remainingAsteroids--;

	if (remainingAsteroids <= 0 && !pool->hasActiveAsteroids())
	{
		waveActive = false;
		startWave();
		spawnAsteroidWave();
	}
}
