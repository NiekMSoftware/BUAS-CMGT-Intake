#pragma once

class Player : public GameObject
{
public:
	Player();
	~Player() override;

	// methods
	void Update(float deltaTime) override;
	void Draw(Surface* screen) override;

	void OnCollisionEnter(const Collider* other) override;
	void OnCollisionStay(const Collider* other) override;
	void OnCollisionExit(const Collider* other) override;

private:
	void HandleInput();

private:
	float2 movementDirection = { 0, 0 };

	float moveSpeed = 0.15f;
};