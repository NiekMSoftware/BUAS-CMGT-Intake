#include "precomp.h"
#include "Asteroid.h"

void Asteroid::initialize()
{
	name = "asteroid";
	angle = Random::getRandomAngle();

	// randomize direction
	direction.x = Random::getRandomFloatClamped(-1.f, 1.f);
	direction.y = Random::getRandomFloatClamped(-1.f, 1.f);

	CollisionSystem::instance().registerObject(this,
		[this](const CollisionEvent& event)
		{
			this->onCollision(event);
		});
}

Asteroid::~Asteroid()
{
	CollisionSystem::instance().unregisterObject(this);
}


void Asteroid::update()
{
	if (isActive())
	{
		position += velocity * (direction * Time::deltaTime);
		keepInView();
		rotate(rotSpeed * Time::deltaTime);
	}
}

void Asteroid::onCollision(const CollisionEvent& event)
{
	if (event.other->getName().find("player") != std::string::npos)
	{
		std::println("Collided with player!");
	}
}
