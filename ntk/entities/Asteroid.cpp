#include "precomp.h"
#include "Asteroid.h"

void Asteroid::initialize()
{
	name = "asteroid";
	angle = Random::getRandomAngle();

	// randomize direction
	direction.x = Random::getRandomFloatClamped(-1.f, 1.f);
	direction.y = Random::getRandomFloatClamped(-1.f, 1.f);

	rotDir = Random::getRandomFloatClamped(-1.f, 1.f);

	CollisionSystem::instance().registerObject(this,
		[this](const CollisionEvent& event)
		{
			m_collision = false;
			this->onCollision(event);
		});
}

Asteroid::~Asteroid()
{
	CollisionSystem::instance().unregisterObject(this);
	asteroidPool = nullptr;
	delete asteroidPool;
}


void Asteroid::update()
{
	if (isActive())
	{
		position += velocity * (direction * Time::deltaTime);
		keepInView();
		rotate(rotDir * rotSpeed * Time::deltaTime);
		updateCollider();
	}
}

void Asteroid::onCollision(const CollisionEvent& event)
{
	if (m_collision) return;
	if (event.other->getName().find("Projectile") != std::string::npos)
	{
		GameManager::instance().incrementMultiplier();
		m_collision = true;
		asteroidPool->destroyAsteroid(this);
	}
}
