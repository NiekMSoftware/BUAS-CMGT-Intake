#include "precomp.h"
#include "Player.h"

Player::Player()
{
	ResourceHolder& rh = ResourceHolder::Instance();
	rh.LoadSprite("assets/playership.png", "player", 9);
	m_sprite = rh.GetSprite("player");

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
