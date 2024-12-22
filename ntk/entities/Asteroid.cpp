#include "precomp.h"
#include "Asteroid.h"

void Asteroid::initialize()
{
	name = "asteroid";
	// randomize direction
}

void Asteroid::update()
{
	GameObject::update();

	keepInView();
}

void Asteroid::fixedUpdate()
{
	
}
