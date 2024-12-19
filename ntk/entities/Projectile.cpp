#include "precomp.h"
#include "Projectile.h"

Projectile::Projectile(float2 startPosition, float initialAngle)
{
	position = startPosition;
	angle = initialAngle;

	ResourceHolder& rh = ResourceHolder::Instance();
	m_sprite = rh.CreateSquare("projectile", 16, 16, 0xFFFF0000);

	projectileSpeed = 500.f;
	velocity.x = std::cos(angle * (PI / 180.f)) * projectileSpeed;
	velocity.y = std::sin(angle * (PI / 180.f)) * projectileSpeed;

	name = "Projectile";
}

void Projectile::update()
{
	if (position.x > SCRWIDTH)
	{
		GameWorld::instance().removeObject(this);
	}
	if (position.x < 0.0f)
	{
		GameWorld::instance().removeObject(this);
	}
	if (position.y > SCRHEIGHT)
	{
		GameWorld::instance().removeObject(this);
	}
	if (position.y < 0.0f)
	{
		GameWorld::instance().removeObject(this);
	}
}

void Projectile::fixedUpdate()
{
	// Update position based on velocity and fixed delta time
	position.x += velocity.x * Time::fixedDeltaTime;
	position.y += velocity.y * Time::fixedDeltaTime;
}
