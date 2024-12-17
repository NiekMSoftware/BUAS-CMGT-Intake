#include "precomp.h"
#include "Player.h"

Player::Player()
{
	ResourceHolder& rh = ResourceHolder::Instance();
	rh.LoadSprite("assets/playership.png", "player", 9);
	m_sprite = rh.GetSprite("player");

	position = { 0, 0 };

	thrustInput = 0.f;
	angle = 0.f;

	speedMod = 2.4f;
	rotationMod = 1.2f;

	speed = 100.f * speedMod;
	maxSpeed = 150.f;
	rotationSpeed = 90.f * rotationMod;
}

void Player::update(float deltaTime)
{
	GameObject::update(deltaTime);

	retrieveInput(deltaTime);
}

void Player::fixedUpdate(float fixedDeltaTime)
{
	thrust(fixedDeltaTime);

	// only apply drag if no input is given
	if (thrustInput == 0.f)
		applySpaceBraking(50.f, fixedDeltaTime);
}

void Player::retrieveInput(float dt)
{
	float rotationValue = Input::getAxis(Input::Horizontal) * (rotationSpeed * rotationMod * dt);
	rotate(rotationValue);

	float thrustValue = Input::getKey(GLFW_KEY_W);
	if (thrustValue != 0.f)
	{
		float forwardMovement = speed * thrustValue;
		thrustInput = forwardMovement;
	}
	else
	{
		thrustInput = 0.f;
	}
}

void Player::thrust(float fixedDeltaTime)
{
	if (thrustInput != 0.f)
	{
		if (length(velocity) > maxSpeed)
		{
			velocity = normalize(velocity) * maxSpeed;
		}

		// apply velocity in the direction of the angle (forward motion)
		velocity.x += std::cos(angle * (PI / 180.f)) * thrustInput * fixedDeltaTime;
		velocity.y += std::sin(angle * (PI / 180.f)) * thrustInput * fixedDeltaTime;
	}
}