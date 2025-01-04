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
	// generate sprites
	ResourceHolder::Instance().LoadSprite("assets/borrelnootje_white.png", "bn_w", 1);
	ResourceHolder::Instance().LoadSprite("assets/borrelnootje_brown.png", "bn_b", 1);
	ResourceHolder::Instance().LoadSprite("assets/borrelnootje_orange.png", "bn_o", 1);

	static std::vector<std::pair<AsteroidSize, AsteroidConfig>> configs{
		{
			AsteroidSize::Large, {
				.score = 20,
				.speed = 50.f,
				.rotSpeed = 25.f,
				.size = 3.f,
				.sprite = ResourceHolder::Instance().GetSprite("bn_w"),
				.explosionSound = "assets/audio/meteor_large_explosion.wav"
			}
		},
		{
			AsteroidSize::Medium, {
				.score = 50,
				.speed = 150.f,
				.rotSpeed = 50.f,
				.size = 1.5f,
				.sprite = ResourceHolder::Instance().GetSprite("bn_b"),
				.explosionSound = "assets/audio/meteor_medium_explosion.wav"
			}
		},
		{
			AsteroidSize::Small, {
				.score = 100,
				.speed = 200.f,
				.rotSpeed = 100.f,
				.size = 0.75f,
				.sprite = ResourceHolder::Instance().GetSprite("bn_o"),
				.explosionSound = "assets/audio/meteor_small_explosion.wav"
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
	asteroid->setRotationSpeed(config.rotSpeed);
	asteroid->setActive(false);
	asteroid->setPooled(true);
	asteroid->setAsteroidPool(this);
	asteroid->setLayer(Layer::Asteroids);
	asteroid->setExplosionSound(config.explosionSound);

	GameWorld::instance().addObject(asteroid);
	return asteroid;
}

void AsteroidPool::handleAsteroid(GameObject* asteroid)
{
	float scale = asteroid->getScale();
	AsteroidSize currentSize = {};

	for (const auto& config : CONFIGS)
	{
		if (std::abs(config.second.size - scale) < 0.1f)
		{
			currentSize = config.first;
			GameManager::instance().score->addScore(config.second.score);
			break;
		}
	}

	if (currentSize != AsteroidSize::Small)
	{
		splitAsteroid(asteroid, currentSize);
	}

	returnAsteroidToPool(asteroid);
}

void AsteroidPool::splitAsteroid(const GameObject* asteroid, const AsteroidSize currentSize)
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
	float originalAngle = atan2f(originalVel.y, originalVel.x);

	// Add randomness to the split angle range (30 to 60 degrees)
	const float MIN_SPLIT_ANGLE = PI / 6.0f;
	const float MAX_SPLIT_ANGLE = PI / 3.0f;
	const float splitAngles[] = {
		Random::getRandomFloat(MIN_SPLIT_ANGLE, MAX_SPLIT_ANGLE),
		-Random::getRandomFloat(MIN_SPLIT_ANGLE, MAX_SPLIT_ANGLE)
	};

	// Add some variance to the split offset distance
	const float BASE_SPLIT_OFFSET = 20.0f;
	const float OFFSET_VARIANCE = 5.0f;

	for (float angleOffset : splitAngles) {
		float splitAngle = originalAngle + angleOffset;

		// Randomize the offset distance for each piece
		float splitOffset = BASE_SPLIT_OFFSET + Random::getRandomFloat(-OFFSET_VARIANCE, OFFSET_VARIANCE);

		// Calculate split position with randomized offset
		float2 newPos = float2(
			originalPos.x + cosf(splitAngle) * (splitOffset * 0.5f),
			originalPos.y + sinf(splitAngle) * (splitOffset * 0.5f)
		);

		GameObject* newAsteroid = spawnAsteroid(nextSize, newPos);

		if (newAsteroid) {
			// Add some speed variance to each piece
			float speedVariance = 0.2f;
			float baseSpeed = getConfig(nextSize).speed;
			float newSpeed = baseSpeed * (1.0f + Random::getRandomFloat(-speedVariance, speedVariance));

			float2 newVel = float2(
				cosf(splitAngle) * newSpeed,
				sinf(splitAngle) * newSpeed
			);

			newAsteroid->setVelocity(newVel);

			// Add both random initial rotation and rotation speed
			float randomRotation = Random::getRandomFloat(-PI, PI);

			newAsteroid->setRotation(randomRotation);
		}
	}
}

void AsteroidPool::returnAsteroidToPool(GameObject* asteroid)
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

	WaveSystem::instance().onAsteroidReturned();
}

bool AsteroidPool::hasActiveAsteroids() const
{
	return !activeAsteroids.empty();
}

float2 AsteroidPool::calculateSplitPosition(const float2& originalPos, float angle, float offset)
{
	return float2{
		originalPos.x * cosf(angle * PI / 180.f) * offset,
		originalPos.y * sinf(angle * PI / 180.f) * offset
	};
}
