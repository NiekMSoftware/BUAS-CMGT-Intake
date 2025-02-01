#pragma once

struct WaveConfig
{
	int numLargeAsteroids;  //< Number of large asteroids in this wave
	float speedMultiplier;  //< Multiplier for asteroid base speeds
	float specialChance;  //< Chance of special asteroid patterns
};

class WaveSystem
{
public:
	static WaveSystem& instance()
	{
		static WaveSystem s;
		return s;
	}

	void initialize(AsteroidPool* asteroidPool);
	void startWave();
	void reset();

	// Getters for game state
	bool isWaveActive() const { return waveActive; }
	int getCurrentWav() const { return currentWave; }
	int getRemainingAsteroids() const { return remainingAsteroids; }

	void onAsteroidReturned();

	/** Spawns a single asteroid for the current wave at a random position */
	void spawnAsteroidWave() const;

private:
	WaveSystem() : pool(nullptr), currentWaveConfig(), currentWave(0), remainingAsteroids(0), waveActive(false)
	{ }

	WaveConfig generateWaveConfig() const;

	/** Generated a random position within the screen bounds. */
	float2 generateRandomPosition() const;

	AsteroidPool* pool;
	WaveConfig currentWaveConfig;

	int currentWave;
	int remainingAsteroids;
	bool waveActive;
};
