#include "precomp.h"
#include "GameObject.h"

GameObject::GameObject()
{
	ResourceHolder& rh = ResourceHolder::Instance();

	velocity = { 0, 0 };
	dragCoefficient = 0.05f;
	m_sprite = rh.CreateSquare("square", 32, 32);
	position = { 100, 100 };
	angle = 0.0f;
}

GameObject::~GameObject()
{
	// [READ ME] - Sprite won't be deleted from here, the resource holder makes sure the allocated memory gets safely removed.
}

void GameObject::update(float deltaTime)
{
	position.x += velocity.x * deltaTime;
	position.y += velocity.y * deltaTime;
}

void GameObject::fixedUpdate(float fixedDeltaTime)
{
	/* Incorporate any related physics updates */
}

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

void GameObject::applySpaceBraking(float brakeForce, float fixedDeltaTime)
{
	float currentSpeed = length(velocity);
	if (currentSpeed > 0.f)
	{
		float reductionAmount = std::min(brakeForce * fixedDeltaTime, currentSpeed);
		velocity = normalize(velocity) * (currentSpeed - reductionAmount);
	}
}
