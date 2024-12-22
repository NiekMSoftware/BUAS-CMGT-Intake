#pragma once

class Asteroid : public GameObject
{
public:
	void initialize() override;

	void update() override;

private:
	float2 direction = 0;
};
