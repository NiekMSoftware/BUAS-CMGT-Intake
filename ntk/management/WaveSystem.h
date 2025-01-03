#pragma once

struct WaveConfig
{
	int numLargeAsteroids;  //< Number of large asteroids in this wave
	float speedMultiplier;  //< Multiplier for asteroid base speeds
	float specialChange;  //< Chance of special asteroid patterns
};

class WaveSystem
{
public:
	static WaveSystem& instance();

	void initialize(AsteroidPool* asteroidPool);
	void startWave();

	// Getters for game state
	bool isWaveActive() const { return waveActive; }
	int getCurrentWav() const { return currentWave; }
	int getRemainingAsteroids() const { return remainingAsteroids; }

	void onAsteroidDestroyed();

	/** Spawns a single asteroid for the current wave at a random position */
	void spawnWaveAsteroid();

private:
	WaveSystem() : pool(nullptr), currentWaveConfig(), currentWave(0), remainingAsteroids(0), waveActive(false)
	{ }

	WaveConfig generateWaveConfig() const;

	/** Generated a random position within the screen bounds. */
	float2 generateRandomPosition() const;

private:
	AsteroidPool* pool;
	WaveConfig currentWaveConfig;

	int currentWave;
	int remainingAsteroids;
	bool waveActive;
};
