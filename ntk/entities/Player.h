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

private:
	float thrustInput;

	float speed;
	float maxSpeed;
	float rotationSpeed;

	float speedMod;
	float rotationMod;
};
