#include "precomp.h"
#include "Projectile.h"

Projectile::Projectile(float2 startPosition, float initialAngle)
{
	position = startPosition;
	angle = initialAngle;

	ResourceHolder& rh = ResourceHolder::Instance();
	m_sprite = rh.CreateSquare("projectile", 16, 16, 0xFFFF0000);

	float projectileSpeed = 100.f;
	velocity.x = std::cos(angle * (PI / 180.f) * projectileSpeed);
	velocity.y = std::sin(angle * (PI / 180.f) * projectileSpeed);
}
