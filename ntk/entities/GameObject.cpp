#include "precomp.h"
#include "GameObject.h"

void GameObject::initialize()
{
	ResourceHolder& rh = ResourceHolder::Instance();

	velocity = { 0, 0 };

	m_sprite = rh.CreateSquare("square", 32, 32);
	position = { 100, 100 };
	angle = 0.0f;
	name = "Game Object";
}

void GameObject::update()
{
	if (m_active)
	{
		position.x += velocity.x * Time::deltaTime;
		position.y += velocity.y * Time::deltaTime;
	}

	// Update collider position based on sprite dimensions
	if (m_sprite && m_active)
	{
		float halfWidth = static_cast<float>(m_sprite->GetWidth()) * 0.5f * scale;
		float halfHeight = static_cast<float>(m_sprite->GetHeight()) * 0.5f * scale;

		collider.bmin[0] = position.x - halfWidth;
		collider.bmin[1] = position.y - halfHeight;
		collider.bmax[0] = position.x + halfWidth;
		collider.bmax[1] = position.y + halfHeight;

		updateCollider();
	}
}

void GameObject::fixedUpdate()
{
	/* Incorporate any related physics updates */
}

void GameObject::render(Surface* screen)
{
	// Calculate center-based coordinates of the sprite
	int centerX = static_cast<int>(position.x - static_cast<float>(m_sprite->GetWidth()) * 0.5f);
	int centerY = static_cast<int>(position.y - static_cast<float>(m_sprite->GetHeight()) * 0.5f);

	// Only render if the sprite exists and is active
	if (m_sprite && m_active)
		m_sprite->Draw(screen, centerX, centerY, scale, angle);
}

/**
 * Moves the position to a new position by translating it (make sure to multiply by delta time).
 * @param translation The translation to compute to.
 */
void GameObject::translate(const float2& translation)
{
	position += translation;
}

/**
 * Rotates the game object around in 2D space with a value.
 * @param a The angular rotation value.
 */
void GameObject::rotate(const float& a)
{
	angle += a;
}

/** Returns the position of the game object. */
float2 GameObject::getPosition() const
{
	return position;
}

/** Returns the collider of the game object. */
aabb GameObject::getCollider() const
{
	return collider;
}

/** Returns the sprite of the game object as a pointer. */
Sprite* GameObject::getSprite() const
{
	return m_sprite;
}

/** Initializes the collider of the game object by taking the sprite's width and height */
void GameObject::initializeCollider()
{
	if (!m_sprite) return;

	// Get the sprite's width and height
	float spriteWidth = static_cast<float>(m_sprite->GetWidth());
	float spriteHeight = static_cast<float>(m_sprite->GetWidth());

	// Compute the initial bmin and bmax based on position and scale
	float halfWidth = (spriteWidth * scale) / 2.0f;
	float halfHeight = (spriteHeight * scale) / 2.0f;

	collider.bmin[0] = position.x - halfWidth;
	collider.bmin[1] = position.y - halfHeight;
	collider.bmax[0] = position.x + halfWidth;
	collider.bmax[1] = position.y + halfHeight;
}

/** Updates the collider with the game object's position, scale and rotation. */
void GameObject::updateCollider()
{
	if (!m_sprite) return;

	// Get the sprite's width and height
	float spriteWidth = static_cast<float>(m_sprite->GetWidth());
	float spriteHeight = static_cast<float>(m_sprite->GetWidth());

	// Compute the half extents
	float halfWidth = (spriteWidth * scale) / 2.0f;
	float halfHeight = (spriteHeight * scale) / 2.0f;

	collider.bmin[0] = position.x - halfWidth;
	collider.bmin[1] = position.y - halfHeight;
	collider.bmax[0] = position.x + halfWidth;
	collider.bmax[1] = position.y + halfHeight;

	// TODO: perform rotations
}

/**
 * Constraints the game object clamping it around the screen.
 */
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
