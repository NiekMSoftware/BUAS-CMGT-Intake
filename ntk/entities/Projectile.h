#pragma once

class Projectile : public GameObject
{
public:
	Projectile(float2 startPosition, float initialAngle);
	void initialize() override;
	~Projectile() override;

	void update() override;
	void fixedUpdate() override;

	void onCollision(const CollisionEvent&) override;

private:
	float projectileSpeed;
};
