#pragma once

class Player : public GameObject
{
public:
	void initialize() override;
	~Player() override;

	void update() override;
	void fixedUpdate() override;

	void onCollision(const CollisionEvent& event) override;

	void addLife(int add);
	void removeLife(int sub);

	int getLives() const { return lives; }

	bool isDead() const { return lives == 0; }

private:
	void applySpaceBraking(float brakeForce);

	void retrieveInput();
	void thrust();
	void fireProjectile() const;

	bool isImmune() const { return collisionTimer > 0.0f; }

private:
	float thrustInput = 0;

	// speed attributes
	float speed = 0;
	float maxSpeed = 0;
	float rotationSpeed = 0;

	float speedMod = 0;
	float rotationMod = 0;

	// firing variables
	float timeSinceLastShot = 0;
	const float firingInterval = 0.25f;  // firing 4 times per second

	// live attributes
	int lives = 0;
	int maxLives = 0;

	// immunity frames until next collision with asteroid
	float collisionTimer = 0.0f;
	const float immunity = 1.0f;

	Audio::Sound* fireSound = nullptr;
	Audio::Sound* hitSound = nullptr;
	Audio::Sound* explosionSound = nullptr;
};