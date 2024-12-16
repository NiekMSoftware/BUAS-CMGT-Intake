#pragma once

class Player : public GameObject
{
public:
	explicit Player();
	~Player() override = default;

	void update(float deltaTime) override;
	void fixedUpdate(float fixedDeltaTime) override;

private:
	void retrieveInput(float dt);
	void thrust(float fixedDeltaTime);

private:
	float thrustInput;

	float speed;
	float maxSpeed;
	float rotationSpeed;

	float speedMod;
	float rotationMod;
};
