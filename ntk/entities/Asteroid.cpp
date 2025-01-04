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

	if (explosionSound) delete explosionSound;
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
		GameManager::instance().score->incrementMultiplier();
		m_collision = true;

		if (explosionSound)
			explosionSound->play();

		asteroidPool->handleAsteroid(this);
	}
}

void Asteroid::setExplosionSound(const std::string& soundPath)
{
	// Delete previously allocated sound
	delete explosionSound;

	// Create a new sound
	explosionSound = new Audio::Sound(soundPath, Audio::Sound::Type::Sound);
	explosionSound->setVolume(0.5f);
}
