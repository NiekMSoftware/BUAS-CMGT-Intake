#pragma once

class Player : public GameObject
{
public:
	Player();
	~Player() override;

	// methods
	void Update(float deltaTime) override;
	void Draw(Surface* screen) override;

private:
	void HandleInput();

private:
	float2 movementDirection = { 0, 0 };

	float moveSpeed = 0.1f;
};