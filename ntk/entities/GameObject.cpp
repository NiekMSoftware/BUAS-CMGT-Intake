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
		// Center origin
		const float halfWidth = static_cast<float>(m_sprite->GetWidth()) * 0.5f * scale;
		const float halfHeight = static_cast<float>(m_sprite->GetHeight()) * 0.5f * scale;

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
	{
		m_sprite->Draw(screen, centerX, centerY, scale, angle);

#if _DEBUG
		renderCollider(screen);
#endif
	}
}

void GameObject::renderCollider(Surface* target, uint color) const
{
	if (!m_sprite || !target) return;

	// Draw the AABB (current visualization)
	target->Box(
		static_cast<int>(collider.bmin[0]),
		static_cast<int>(collider.bmin[1]),
		static_cast<int>(collider.bmax[0]),
		static_cast<int>(collider.bmax[1]),
		color
	);

	// Draw the actual rotated bounds
	const float angleRad = angle * (PI / 180.0f);
	const float cosA = cos(angleRad) * scale;
	const float sinA = sin(angleRad) * scale;

	// Calculate rotated corners
	const float halfWidth = static_cast<float>(m_sprite->GetWidth()) * 0.5f;
	const float halfHeight = (float)m_sprite->GetHeight() * 0.5f;

	float2 corners[4] = {
		{-halfWidth, -halfHeight},  // Top-left
		{ halfWidth, -halfHeight},  // Top-right
		{ halfWidth,  halfHeight},  // Bottom-right
		{-halfWidth,  halfHeight}   // Bottom-left
	};

	// Transform and draw the actual rotated box
	for (int i = 0; i < 4; i++)
	{
		float2 current = corners[i];
		float2 next = corners[(i + 1) % 4];

		// Transform corners
		float2 transformedCurrent = {
			(current.x * cosA - current.y * sinA) + position.x,
			(current.x * sinA + current.y * cosA) + position.y
		};
		float2 transformedNext = {
			(next.x * cosA - next.y * sinA) + position.x,
			(next.x * sinA + next.y * cosA) + position.y
		};

		// Draw the edge in a different color
		target->Line(
			transformedCurrent.x, transformedCurrent.y,
			transformedNext.x, transformedNext.y,
			0x00FF00  // Green for actual bounds
		);
	}

	// Draw center point (as before)
	const float centerX = (collider.bmin[0] + collider.bmax[0]) * 0.5f;
	const float centerY = (collider.bmin[1] + collider.bmax[1]) * 0.5f;

	const int crossSize = 3;
	target->Line(
		centerX - crossSize, centerY,
		centerX + crossSize, centerY,
		0xFFFF00
	);
	target->Line(
		centerX, centerY - crossSize,
		centerX, centerY + crossSize,
		0xFFFF00
	);
}

/** TODO: Enter a good description of this method */
void GameObject::onCollision(const CollisionEvent&)
{
	if (m_collision) return;
	m_collision = true;
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
/**
 * Updates the collider's AABB bounds based on the game object's transform state.
 * @note This function expects the sprite to be centered at its local origin (0,0)
 */
void GameObject::updateCollider() {
	if (!m_sprite) return;

	// Cache sprite dimensions
	const float spriteWidth = static_cast<float>(m_sprite->GetWidth());
	const float spriteHeight = static_cast<float>(m_sprite->GetHeight());

	// Convert angle to radians and pre-calculate our transformation values
	const float angleRad = angle * (PI / 180.0f);
	const float cosA = cos(angleRad) * scale;  // Combine rotation and scale
	const float sinA = sin(angleRad) * scale;

	// Define our corners in local space relative to sprite center
	const float localX[4] = { -0.5f * spriteWidth,  0.5f * spriteWidth, -0.5f * spriteWidth,  0.5f * spriteWidth };
	const float localY[4] = { -0.5f * spriteHeight, -0.5f * spriteHeight, 0.5f * spriteHeight, 0.5f * spriteHeight };

	// Initialize bounds to the first transformed point
	float transformedX = localX[0] * cosA - localY[0] * sinA + position.x;
	float transformedY = localX[0] * sinA + localY[0] * cosA + position.y;

	float minX = transformedX;
	float minY = transformedY;
	float maxX = transformedX;
	float maxY = transformedY;

	// Transform remaining corners and update bounds
	// Starting from 1 since we already processed corner 0
	for (int i = 1; i < 4; ++i) {
		// Apply rotation and scale transformation, then translate
		transformedX = localX[i] * cosA - localY[i] * sinA + position.x;
		transformedY = localX[i] * sinA + localY[i] * cosA + position.y;

		// Update bounds
		minX = std::min(minX, transformedX);
		minY = std::min(minY, transformedY);
		maxX = std::max(maxX, transformedX);
		maxY = std::max(maxY, transformedY);
	}

	// Update the collider with our calculated bounds
	collider.bmin[0] = minX;
	collider.bmin[1] = minY;
	collider.bmax[0] = maxX;
	collider.bmax[1] = maxY;
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
