#include "precomp.h"
#include "player.h"

Player::Player()
{
	// Initialize sprite
	auto& rh = ResourceHolder::Instance();
	rh.LoadSprite("assets/tmpl8/ctankbase.tga", "player", 16);
	sprite = rh.GetSprite("player");

	// center the origin
	CenterOrigin();
}

Player::~Player()
{
	/** Delete remaining player resources */
}

void Player::Update(float deltaTime)
{
	HandleInput();

	position += movementDirection * moveSpeed * deltaTime;
}

void Player::Draw(Surface* screen)
{
	sprite->Draw(screen, static_cast<int>(position.x), static_cast<int>(position.y));
}

void Player::HandleInput()
{
	movementDirection = { Input::GetAxis(Axis::Horizontal), Input::GetAxis(Axis::Vertical) };

	// normalize movement
	if (movementDirection.x != 0 && movementDirection.y != 0)
	{
		float length = sqrt(movementDirection.x * movementDirection.x + movementDirection.y * movementDirection.y);
		movementDirection.x /= length;
		movementDirection.y /= length;
	}
}