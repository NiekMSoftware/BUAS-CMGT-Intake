#include "precomp.h"
#include "GameObject.h"

GameObject::GameObject()
{
	ResourceHolder& rh = ResourceHolder::Instance();

	m_sprite = rh.CreateSquare("square", 32, 32);
	position = { 100, 100 };
	angle = 0.0f;
}

GameObject::~GameObject()
{
	// [READ ME] - Sprite won't be deleted from here, the resource holder makes sure the allocated memory gets safely removed.
}

void GameObject::update(float)
{ }

void GameObject::render(Surface* screen)
{
	//m_sprite->Draw(screen, static_cast<int>(position.x), static_cast<int>(position.y));
	m_sprite->Draw(screen, static_cast<int>(position.x), static_cast<int>(position.y), 1.f, angle);
}

float2 GameObject::getPosition() const
{
	return position;
}

void GameObject::translate(const float2& translation)
{
	position += translation;
}

void GameObject::rotate(const float& a)
{
	angle += a;
}

aabb GameObject::getCollider() const
{
	return collider;
}

Sprite* GameObject::getSprite() const
{
	return m_sprite;
}
