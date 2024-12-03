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
}

Player::~Player()
{
	/** Delete remaining player resources */
}

void Player::Update(float deltaTime)
{
	HandleInput();

	position += movementDirection * moveSpeed * deltaTime;
	UpdateColliderPosition();
}

void Player::Draw(Surface* screen)
{
#if _DEBUG
	collider->Render(screen);
#endif
	sprite->Draw(screen, static_cast<int>(position.x), static_cast<int>(position.y));
}

void Player::OnCollisionEnter(const Collider* other)
{
	printf(std::format("{}, entered Player collider.\n", other->id).c_str());
}

void Player::OnCollisionStay(const Collider*)
{
	//printf(std::format("{}, staying inside the Player collider.\n", other->id).c_str());
}

void Player::OnCollisionExit(const Collider* other)
{
	printf(std::format("{}, exited out of  collider.\n", other->id).c_str());
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