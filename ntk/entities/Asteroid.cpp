#include "precomp.h"
#include "Asteroid.h"

void Asteroid::initialize()
{
	// set the direction the asteroid will move to randomly

	// initialize sprite
	m_sprite = ResourceHolder::Instance().CreateSquare("asteroid_l", 32, 32, 0xFFFFFFFF);
	scale = 3.f;
	position = { 100, 100 };
}

void Asteroid::update()
{
	GameObject::update();

	keepInView();
}

void Asteroid::fixedUpdate()
{
	
}
