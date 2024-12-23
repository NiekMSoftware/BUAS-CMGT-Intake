﻿#pragma once

class Asteroid : public GameObject
{
public:
	void initialize() override;
	~Asteroid() override;

	void update() override;

	void onCollision(const CollisionEvent&) override;
private:
	float2 direction = 0;
};
