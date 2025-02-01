#include "precomp.h"
#include "Projectile.h"

Projectile::Projectile(float2 startPosition, float initialAngle)
: projectileSpeed(500.f)
{
	position = startPosition;
	angle = initialAngle;
}

void Projectile::initialize()
{
	ResourceHolder& rh = ResourceHolder::Instance();
	m_sprite = rh.CreateSquare("projectile", 16, 16, 0xFFFF0000);
	setLayer(Layer::Projectile);
	name = "Projectile";

	initializeCollider();

	// initialize velocity
	velocity.x = std::cos(angle * (PI / 180.f)) * projectileSpeed;
	velocity.y = std::sin(angle * (PI / 180.f)) * projectileSpeed;

	CollisionSystem::instance().registerObject(this,
		[this](const CollisionEvent& event)
		{
			this->onCollision(event);
		});
}

Projectile::~Projectile()
{
	CollisionSystem::instance().unregisterObject(this);
}


void Projectile::update()
{
	if (GameManager::instance().getCurrentState() == GameOver)
	{
		GameWorld::instance().removeObject(this);
		return;
	}

	updateCollider();

	// Check the current position and remove it if tries to go out of view
	if (position.x > SCRWIDTH || position.x < 0.0f || position.y > SCRHEIGHT || position.y < 0.0f)
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

void Projectile::onCollision(const CollisionEvent& event)
{
	if (m_collision)
	{
		// mark for removal
		GameWorld::instance().removeObject(this);
		return;
	}

	// Only detect asteroid collisions
	if (event.other->getName().find("asteroid") != std::string::npos)
		m_collision = true;
}
