#include "precomp.h"
#include "GameObject.h"

GameObject::GameObject()
{
	ResourceHolder& rh = ResourceHolder::Instance();

	velocity = { 0, 0 };
	
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

	keepInView();
}

void GameObject::fixedUpdate(float)
{
	/* Incorporate any related physics updates */
}

void GameObject::render(Surface* screen)
{
	// Calculate center-based coordinates of the sprite
	int centerX = static_cast<int>(position.x - static_cast<float>(m_sprite->GetWidth()) * 0.5f);
	int centerY = static_cast<int>(position.y - static_cast<float>(m_sprite->GetHeight()) * 0.5f);
	m_sprite->Draw(screen, centerX, centerY, 1.f, angle);
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

void GameObject::keepInView()
{
	// constraint the object to the screen's width and height
	if (position.x > SCRWIDTH)
	{
		position.x = 0.0f;
	}
	else if (position.x < 0.0f)
	{
		position.x = SCRWIDTH;
	}

	if (position.y > SCRHEIGHT)
	{
		position.y = 0.0f;
	}
	else if (position.y < 0.0f)
	{
		position.y = SCRHEIGHT;
	}
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
