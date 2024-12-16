#pragma once

class Player : public GameObject
{
public:
	explicit Player();
	~Player() override = default;

	void update(float deltaTime) override;

private:
	float speed;
};
