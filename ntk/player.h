#pragma once

class Player : public GameObject
{
public:
	Player();
	~Player() override;

	// methods
	void Update(float deltaTime) override;
	void Draw(Surface* screen) override;

	void TakeDamage(int damage);
	void RecoverHealth(int value);

private:
	void HandleInput();
	void PrintHealth(Surface* screen);

private:
	float2 movementDirection = { 0, 0 };

	float moveSpeed = 0.15f;

	int currentHealth;
};