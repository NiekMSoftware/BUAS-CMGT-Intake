#pragma once

class Projectile : public GameObject
{
public:
	Projectile(float2 startPosition, float initialAngle);
	void initialize() override;

	void update() override;
	void fixedUpdate() override;

private:
	float projectileSpeed;
};
