#include "precomp.h"
#include "AsteroidPool.h"

const std::vector<std::pair<AsteroidSize, AsteroidConfig>> AsteroidPool::CONFIGS = initializeConfigs();

AsteroidPool::~AsteroidPool()
{
	for (GameObject* obj : largeAsteroids)
	{
		delete obj;
	}
	largeAsteroids.clear();
	OutputDebugString("AsteroidPool::~AsteroidPool - Cleared container 'largeAsteroids'\n");

	for (GameObject* obj : mediumAsteroids)
	{
		delete obj;
	}
	OutputDebugString("AsteroidPool::~AsteroidPool - Cleared container 'mediumAsteroids'\n");

	for (GameObject* obj : smallAsteroids)
	{
		delete obj;
	}
	OutputDebugString("AsteroidPool::~AsteroidPool - Cleared container 'smallAsteroids'\n");

	for (GameObject* obj : activeAsteroids)
	{
		delete obj;
	}
	OutputDebugString("AsteroidPool::~AsteroidPool - Cleared container 'smallAsteroids'\n");
	OutputDebugString("AsteroidPool::~AsteroidPool - Cleaned up all allocated memory.\n\n");
}

AsteroidPool::AsteroidPool(int maxLargeAsteroids)
{
	initializePools(maxLargeAsteroids);
}

const std::vector<std::pair<AsteroidSize, AsteroidConfig>>& AsteroidPool::initializeConfigs()
{
	static std::vector<std::pair<AsteroidSize, AsteroidConfig>> configs{
		{
			AsteroidSize::Large, {
				.score = 100,
				.speed = 50.f,
				.size = 3.f,
				.sprite = ResourceHolder::Instance().CreateSquare("asteroid", 32, 32)
			}
		},
		{
			AsteroidSize::Medium, {
				.score = 50,
				.speed = 150.f,
				.size = 1.5f,
				.sprite = ResourceHolder::Instance().GetSprite("asteroid")
			}
		},
		{
			AsteroidSize::Small, {
				.score = 10,
				.speed = 200.f,
				.size = 0.75f,
				.sprite = ResourceHolder::Instance().GetSprite("asteroid")
			}
		}
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
	const int mediumCapacity = 2 * maxLargeAsteroids;
	const int smallCapacity = maxLargeAsteroids * 2 * 4;

	// Pre-allocate all possible asteroids
	largeAsteroids.reserve(maxLargeAsteroids);
	mediumAsteroids.reserve(mediumCapacity);
	smallAsteroids.reserve(smallCapacity);

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
		asteroid->setActive(true);
		asteroid->setPosition(position);
		asteroid->setRandomRotation();
		asteroid->setRandomVelocity(config.speed);

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
	Asteroid* asteroid = new Asteroid();
	asteroid->initialize();
	const AsteroidConfig& config = getConfig(size);

	asteroid->setSprite(config.sprite);
	asteroid->setScale(config.size);
	asteroid->setActive(false);
	asteroid->setPooled(true);

	GameWorld::instance().addObject(asteroid);
	return asteroid;
}
