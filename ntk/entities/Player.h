#pragma once

class Player : public GameObject
{
public:
	explicit Player();
	~Player() override = default;

	void update() override;
	void fixedUpdate() override;

private:
	void applySpaceBraking(float brakeForce);

	void retrieveInput();
	void thrust();
	void fireProjectile();

private:
	float thrustInput;

	float speed;
	float maxSpeed;
	float rotationSpeed;

	float speedMod;
	float rotationMod;

	// firing variables
	float timeSinceLastShot;
	const float firingInterval = 0.25f;  // firing 4 times per second 
};
