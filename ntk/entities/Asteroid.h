#pragma once

class Asteroid : public GameObject
{
public:
	void initialize() override;

	void update() override;

private:
	float direction = 0;
};
