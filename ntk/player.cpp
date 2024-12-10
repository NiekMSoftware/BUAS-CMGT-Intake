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

	InitializeCollider();

	UpdateColliderPosition();

	// Physics
	velocity = { 0, 0 };
	maxSpeed = 0.25f;

	// setup physic attributes
	dragCoefficient = 1.f;
	mass = 50.f;

	// setup data
	currentHealth = 3;
}

Player::~Player()
{
	/** Delete remaining player resources */
}

void Player::Update(float deltaTime)
{
	HandleInput();

	velocity += movementDirection * (moveSpeed * deltaTime);

	UpdateColliderPosition();
}

void Player::Draw(Surface* screen) 
{
	sprite->Draw(screen, static_cast<int>(position.x), static_cast<int>(position.y));
	PrintHealth(screen);
}

void Player::TakeDamage(int damage)
{
	currentHealth -= damage;
	if (currentHealth < 0) {
		currentHealth = 0;
	}
}

void Player::RecoverHealth(int value)
{
	currentHealth += value;
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

void Player::PrintHealth(Surface* screen)
{
	char buffer[50];
	snprintf(buffer, sizeof(buffer), "Player Health: %i", currentHealth);
	const char* format = buffer;
	screen->Print(format, 10, 10, 0xffffffff);
}
