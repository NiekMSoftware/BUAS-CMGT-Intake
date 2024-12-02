#include "precomp.h"
#include "gameobject.h"

GameObject::GameObject()
{
	auto& rh = ResourceHolder::Instance();
	rh.LoadSprite("assets/tmpl8/ctankbase.tga", "player", 16);

	// init sprite with surface
	sprite = rh.GetSprite("player");

	position = { SCRWIDTH / 2, SCRHEIGHT / 2 };
}

void GameObject::Update(float deltaTime)
{
	float2 moveDirection = { Input::GetAxis(Axis::Horizontal), Input::GetAxis(Axis::Vertical)};

	// Normalize movement
	if (moveDirection.x != 0 && moveDirection.y != 0)
	{
		float length = sqrt(moveDirection.x * moveDirection.x + moveDirection.y * moveDirection.y);
		moveDirection.x /= length;
		moveDirection.y /= length;
	}

	// Apply movement based on speed and delta time
	position += moveDirection * moveSpeed * deltaTime;

	// Clamp within bounds
	position.x = std::max(0.0f, std::min(position.x, static_cast<float>(SCRWIDTH - 32)));
	position.y = std::max(0.0f, std::min(position.y, static_cast<float>(SCRHEIGHT - 32)));
}

GameObject::~GameObject()
{
	/** Delete any remaining resources. */
}

void GameObject::Draw(Surface* screen)
{
	sprite->Draw(screen, (int)position.x, (int)position.y);
}

void GameObject::SetPosition(float2 newPos)
{
	position = newPos;
}

float2 GameObject::GetPosition() const 
{
	return position;
}