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

void GameObject::CenterOrigin()
{
	if (sprite)
	{
		sprite->SetOrigin(sprite->GetWidth() * 0.5f, sprite->GetHeight() * 0.5f);
	}
}

Ground::Ground()
{
	// Initialize ground with a green sprite
	Surface* surface = new Surface(400, 300);
	surface->Bar(400, 300, 0, 0, 0x22BB22);

	sprite = new Sprite(surface, 1);
	sprite->ownership = true;
}

Ground::~Ground()
{
	delete sprite;
}