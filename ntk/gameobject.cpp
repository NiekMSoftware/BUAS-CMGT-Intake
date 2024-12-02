#include "precomp.h"
#include "gameobject.h"

GameObject::GameObject()
{
	// create a 32x32 square by default
	Surface* surface = new Surface(32, 32);
	surface->ownBuffer = false;
	surface->Bar(8, 8, 2, 2, 0xffffff);

	// init sprite with surface
	sprite = new Sprite(surface, 1);

	OutputDebugString("[LOG] Successfully created a sprite with the surface.\n");

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
	delete sprite;
}

void GameObject::Draw(Surface* screen)
{
	sprite->DrawScaled((int)position.x, (int)position.y, sprite->GetWidth(), sprite->GetHeight(), screen);
}

void GameObject::SetPosition(float2 newPos)
{
	position = newPos;
}

float2 GameObject::GetPosition() const 
{
	return position;
}