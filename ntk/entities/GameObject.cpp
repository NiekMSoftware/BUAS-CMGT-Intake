#include "precomp.h"
#include "GameObject.h"

GameObject::GameObject()
{
	ResourceHolder& rh = ResourceHolder::Instance();
	rh.LoadSprite("assets/playership.png", "player", 9);

	m_sprite = rh.GetSprite("player");

	position = { 0, 0 };
}

GameObject::~GameObject()
{
	// [READ ME] - Sprite won't be deleted from here, the resource holder makes sure the allocated memory gets safely removed.
}

void GameObject::update(float deltaTime)
{ }

void GameObject::render(Surface* screen)
{
	m_sprite->Draw(screen, 0, 0);
}

float2 GameObject::getPosition() const
{
	return position;
}

aabb GameObject::getCollider() const
{
	return collider;
}

Sprite* GameObject::getSprite() const
{
	return m_sprite;
}
