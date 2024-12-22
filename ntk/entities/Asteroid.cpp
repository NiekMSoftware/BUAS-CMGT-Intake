#include "precomp.h"
#include "Asteroid.h"

void Asteroid::initialize()
{
	name = "asteroid";
	angle = Random::getRandomAngle();

	// randomize direction
	direction.x = Random::getRandomFloatClamped(-1.f, 1.f);
	direction.y = Random::getRandomFloatClamped(-1.f, 1.f);
}

void Asteroid::update()
{
	if (isActive())
	{
		position += velocity * (direction * Time::deltaTime);
		keepInView();
	}
}
