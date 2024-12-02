#include "precomp.h"
#include "gameobject.h"

GameObject::GameObject()
{
	sprite = nullptr;

	position = { SCRWIDTH / 2, SCRHEIGHT / 2 };
}

void GameObject::Update(float)
{
	
}

GameObject::~GameObject()
{
	/** Delete any remaining resources. (Resource Holder manages the sprite) */
}

void GameObject::Draw(Surface* screen)
{
	// Only draw the sprite if it exists
	if (sprite)
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