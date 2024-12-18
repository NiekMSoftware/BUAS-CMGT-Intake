#pragma once

class Projectile : public GameObject
{
public:
	Projectile(float2 startPosition, float initialAngle);

	void update() override;
	void fixedUpdate() override;

private:
	float projectileSpeed;
};
