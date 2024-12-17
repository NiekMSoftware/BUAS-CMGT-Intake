#include "precomp.h"
#include "Player.h"

Player::Player()
{
	// set a unique sprite to the player
	ResourceHolder& rh = ResourceHolder::Instance();
	rh.LoadSprite("assets/playership.png", "player", 9);
	m_sprite = rh.GetSprite("player");

	position = { SCRWIDTH / 2.f, SCRHEIGHT / 2.f};

	// initialize computable attributes
	thrustInput = 0.f;
	angle = 0.f;

	// initialize speeds
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
	keepInView();
}

void Player::fixedUpdate(float fixedDeltaTime)
{
	thrust(fixedDeltaTime);

	// only apply drag if no input is given
	if (thrustInput == 0.f)
		applySpaceBraking(50.f, fixedDeltaTime);
}

/**
 * Applies a continuous force to slow down the ship.
 * @param brakeForce The force that will be applied each frame.
 * @param fixedDeltaTime The fixed time interval computed inside the fixedTick method.
 */
void Player::applySpaceBraking(float brakeForce, float fixedDeltaTime)
{
	float currentSpeed = length(velocity);
	if (currentSpeed > 0.f)
	{
		float reductionAmount = std::min(brakeForce * fixedDeltaTime, currentSpeed);
		velocity = normalize(velocity) * (currentSpeed - reductionAmount);
	}
}

/** Retrieves the input and computes the thrust input based on speed. */
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

/** Computes a forward direction and applies thrust to that direction. */
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