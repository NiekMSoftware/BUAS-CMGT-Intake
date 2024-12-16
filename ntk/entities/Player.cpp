#include "precomp.h"
#include "Player.h"

Player::Player()
{
	ResourceHolder& rh = ResourceHolder::Instance();

	m_sprite = rh.CreateSquare("player", 32, 32, 0x256913);
	position = { 0, 0 };
	angle = 0.f;

	speed = 1.f;
}

void Player::update(float deltaTime)
{
	float2 movement = { Input::getAxis(Input::Horizontal) * speed, Input::getAxis(Input::Vertical) * speed};
	translate(movement * deltaTime);

	if (Input::getKey(GLFW_KEY_E))
	{
		rotate(0.1f * deltaTime);
	}

	if (Input::getKey(GLFW_KEY_Q))
	{
		rotate(-0.1f * deltaTime);
	}
}
