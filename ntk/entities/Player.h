#pragma once

class Player : public GameObject
{
public:
	void initialize() override;
	~Player() override;

	void update() override;
	void fixedUpdate() override;

	void onCollision(const CollisionEvent& event) override;

private:
	void applySpaceBraking(float brakeForce);

	void retrieveInput();
	void thrust();
	void fireProjectile() const;

private:
	float thrustInput = 0;

	float speed = 0;
	float maxSpeed = 0;
	float rotationSpeed = 0;

	float speedMod = 0;
	float rotationMod = 0;

	// firing variables
	float timeSinceLastShot = 0;
	const float firingInterval = 0.25f;  // firing 4 times per second 
};
