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
	OutputDebugString("[LOG] AsteroidPool::~AsteroidPool - Cleared container 'largeAsteroids'\n");

	for (GameObject* obj : mediumAsteroids)
	{
		delete obj;
	}
	OutputDebugString("[LOG] AsteroidPool::~AsteroidPool - Cleared container 'mediumAsteroids'\n");

	for (GameObject* obj : smallAsteroids)
	{
		delete obj;
	}
	OutputDebugString("[LOG] AsteroidPool::~AsteroidPool - Cleared container 'smallAsteroids'\n");

	for (GameObject* obj : activeAsteroids)
	{
		delete obj;
	}
	OutputDebugString("[LOG] AsteroidPool::~AsteroidPool - Cleared container 'smallAsteroids'\n");
	OutputDebugString("[LOG] AsteroidPool::~AsteroidPool - Cleaned up all allocated memory.\n\n");
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

void AsteroidPool::destroyAsteroid(GameObject* asteroid)
{
	float scale = asteroid->getScale();
	AsteroidSize currentSize = {};

	for (const auto& config : CONFIGS)
	{
		if (std::abs(config.second.size - scale) < 0.1f)
		{
			currentSize = config.first;
			break;
		}
	}

	if (currentSize != AsteroidSize::Small)
	{
		splitAsteroid(asteroid, currentSize);
	}

	returnToPool(asteroid);
}

void AsteroidPool::splitAsteroid(GameObject* asteroid, AsteroidSize currentSize)
{
	// Determine the next smaller size
	AsteroidSize nextSize;
	if (currentSize == AsteroidSize::Large) {
		nextSize = AsteroidSize::Medium;
	}
	else if (currentSize == AsteroidSize::Medium) {
		nextSize = AsteroidSize::Small;
	}
	else {
		return; // Don't split if already smallest
	}

	// Get the original asteroid's properties
	float2 originalPos = asteroid->getPosition();
	float2 originalVel = asteroid->getVelocity();

	// Calculate the angle of the original velocity vector
	float originalAngle = atan2f(originalVel.y, originalVel.x);

	// Create 2 new asteroids at slightly offset positions
	const float SPLIT_OFFSET = 20.0f; // Distance between split pieces
	const float SPLIT_ANGLES[] = { PI / 4.0f, -PI / 4.0f }; // 45 degrees in radians

	for (float angleOffset : SPLIT_ANGLES) {
		// Calculate split angle relative to original velocity direction
		float splitAngle = originalAngle + angleOffset;

		// Calculate split position slightly offset from original position
		float2 newPos = float2(
			originalPos.x + cosf(splitAngle) * (SPLIT_OFFSET * 0.5f),
			originalPos.y + sinf(splitAngle) * (SPLIT_OFFSET * 0.5f)
		);

		GameObject* newAsteroid = spawnAsteroid(nextSize, newPos);

		if (newAsteroid) {
			// Calculate new velocity based on original velocity plus angular offset
			float newSpeed = getConfig(nextSize).speed;
			float2 newVel = float2(
				cosf(splitAngle) * newSpeed,
				sinf(splitAngle) * newSpeed
			);

			newAsteroid->setVelocity(newVel);

			// Add a bit of random rotation for visual variety
			float randomRotation = Random::getRandomFloat(-PI, PI);
			newAsteroid->setRotation(randomRotation);

			OutputDebugString(std::format("[DEBUG] Split asteroid at ({}, {}) with velocity ({}, {})\n",
				newPos.x, newPos.y, newVel.x, newVel.y).c_str());
		}
	}
}

void AsteroidPool::returnToPool(GameObject* asteroid)
{
	auto it = std::find(activeAsteroids.begin(), activeAsteroids.end(), asteroid);
	if (it != activeAsteroids.end())
	{
		activeAsteroids.erase(it);
	}

	asteroid->setActive(false);

	float scale = asteroid->getScale();
	for (const auto& config : CONFIGS)
	{
		if (std::abs(config.second.size - scale) < 0.1f) 
		{
			std::vector<GameObject*>* pool = getPoolForSize(config.first);
			pool->push_back(asteroid);
			break;
		}
	}
}

float2 AsteroidPool::calculateSplitPosition(const float2& originalPos, float angle, float offset)
{
	return float2{
		originalPos.x * cosf(angle * PI / 180.f) * offset,
		originalPos.y * sinf(angle * PI / 180.f) * offset
	};
}
