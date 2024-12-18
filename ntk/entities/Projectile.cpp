#include "precomp.h"
#include "Projectile.h"

Projectile::Projectile(float2 startPosition, float initialAngle)
{
	position = startPosition;
	angle = initialAngle;

	ResourceHolder& rh = ResourceHolder::Instance();
	m_sprite = rh.CreateSquare("projectile", 16, 16, 0xFFFF0000);

	projectileSpeed = 500.f;
	velocity.x = std::cos(angle * (PI / 180.f) * (projectileSpeed * Time::deltaTime));
	velocity.y = std::sin(angle * (PI / 180.f) * (projectileSpeed * Time::deltaTime));

	name = "Projectile";
}

void Projectile::update()
{ }

void Projectile::fixedUpdate()
{
	velocity.x += projectileSpeed * Time::fixedDeltaTime;
	printf("WOAHZA, I GOT FIRED!\n");
}
