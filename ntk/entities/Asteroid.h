﻿#pragma once

class Asteroid : public GameObject
{
public:
	void initialize() override;
	~Asteroid() override;

	void update() override;

	void onCollision(const CollisionEvent&) override;

	void setAsteroidPool(AsteroidPool* pool) { asteroidPool = pool; }
	void setExplosionSound(const std::string& soundPath);

private:
	float2 direction = 0;
	float rotDir = 1.f;

	AsteroidPool* asteroidPool = nullptr;
	Audio::Sound* explosionSound = nullptr;
};
