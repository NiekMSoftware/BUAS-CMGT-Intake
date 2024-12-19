﻿#include "precomp.h"
#include "AsteroidPool.h"

const std::vector<std::pair<AsteroidSize, AsteroidConfig>> AsteroidPool::CONFIGS = initializeConfigs();

AsteroidPool::AsteroidPool(int maxLargeAsteroids)
{
	initializePools(maxLargeAsteroids);
}

const std::vector<std::pair<AsteroidSize, AsteroidConfig>>& AsteroidPool::initializeConfigs()
{
	static std::vector<std::pair<AsteroidSize, AsteroidConfig>> configs {
		{ AsteroidSize::Large, { .score= 100, .speed= 100.f, .size= 3.f, .sprite= nullptr } },
		{ AsteroidSize::Medium, {.score = 50, .speed = 150.f, .size = 1.5f, .sprite = nullptr } },
		{ AsteroidSize::Small, {.score = 10, .speed = 120.f, .size = 0.75f, .sprite = nullptr } },
	};

	return configs;
}

const AsteroidConfig& AsteroidPool::getConfig(AsteroidSize size)
{
	for (const auto& pair : CONFIGS)
	{
		if (pair.first == size)
		{
			return pair.second;
		}
	}

	// return the second as a fallback
	return CONFIGS[0].second;
}

void AsteroidPool::initializePools(int maxLargeAsteroids)
{
	// Pre-allocate all possible asteroids
	largeAsteroids.reserve(maxLargeAsteroids);
	mediumAsteroids.reserve(2 * maxLargeAsteroids);
	smallAsteroids.reserve(2 * maxLargeAsteroids * 4);

	for (int i = 0; i < maxLargeAsteroids; ++i)
		largeAsteroids.push_back(createAsteroid(AsteroidSize::Large));
	for (int i = 0; i < maxLargeAsteroids * 2; ++i)
		mediumAsteroids.push_back(createAsteroid(AsteroidSize::Medium));
	for (int i = 0; i < maxLargeAsteroids * 2 * 4; ++i)
		smallAsteroids.push_back(createAsteroid(AsteroidSize::Small));
}

GameObject* AsteroidPool::spawnAsteroid(AsteroidSize size, const float2& position)
{
	std::vector<GameObject*>* pool = getPoolForSize(size);

	if (!pool->empty())
	{
		GameObject* asteroid = pool->back();
		pool->pop_back();

		// Configure asteroid
		const AsteroidConfig& config = getConfig(size);
		// TODO: asteroid->setActive(true);
		// TODO: asteroid->setPosition(position);
		// TODO: asteroid->setRandomRotation();
		// TODO: asteroid->setRandomVelocity(config.speed);

		activeAsteroids.push_back(asteroid);
		return asteroid;
	}

	return nullptr;
}

std::vector<GameObject*>* AsteroidPool::getPoolForSize(AsteroidSize size)
{
	switch (size)
	{
		case AsteroidSize::Large: return &largeAsteroids;
		case AsteroidSize::Medium: return &mediumAsteroids;
		case AsteroidSize::Small: return &smallAsteroids;
		default: return &largeAsteroids;  // default fallback
	}
}

GameObject* AsteroidPool::createAsteroid(AsteroidSize size)
{
	GameObject* asteroid = new Asteroid();
	const AsteroidConfig& config = getConfig(size);

	//asteroid->setSprite(config.sprite);
	asteroid->setScale(config.size);
	//asteroid->setActive(false);

	return asteroid;
}
