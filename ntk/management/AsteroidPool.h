#pragma once

enum class AsteroidSize
{
	Large,
	Medium,
	Small
};

struct AsteroidConfig
{
	int score;
	float speed;
	float size;
	Sprite* sprite;
};

class AsteroidPool
{
public:
	AsteroidPool(int maxLargeAsteroids = 10);

	static const std::vector<std::pair<AsteroidSize, AsteroidConfig>>& initializeConfigs();
	static const AsteroidConfig& getConfig(AsteroidSize size);

	void initializePools(int maxLargeAsteroids);
	GameObject* spawnAsteroid(AsteroidSize size, const float2& position);

private:
	std::vector<GameObject*>* getPoolForSize(AsteroidSize size);
	GameObject* createAsteroid(AsteroidSize size);

private:
	std::vector<GameObject*> largeAsteroids;
	std::vector<GameObject*> mediumAsteroids;
	std::vector<GameObject*> smallAsteroids;

	/**
	 * Configuration for each size stored in a vector, using static to ensure it's initialized once and shared
	*/
	static const std::vector<std::pair<AsteroidSize, AsteroidConfig>> CONFIGS;

	// keep track of active asteroids
	std::vector<GameObject*> activeAsteroids;
};
