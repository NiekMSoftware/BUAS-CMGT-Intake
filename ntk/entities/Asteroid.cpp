#include "precomp.h"
#include "Asteroid.h"

void Asteroid::initialize()
{
	name = "asteroid";

	// randomize direction
	angle = Random::getRandomAngle();
	direction = Random::getRandomFloatClamped(-1.f, 1.f);
}

void Asteroid::update()
{
	if (isActive())
	{
		position += velocity * (direction * Time::deltaTime);
		keepInView();
	}
}
