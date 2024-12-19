#pragma once

class Asteroid : public GameObject
{
public:
	void initialize() override;

	void update() override;
	void fixedUpdate() override;

private:
	float2 direction;
};
