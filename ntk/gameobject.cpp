#include "precomp.h"
#include "gameobject.h"

GameObject::GameObject()
{
	// create a 32x32 square by default
	Surface* surface = new Surface(32, 32);
	surface->ownBuffer = false;
	surface->Bar(32, 32, 32, 32, 0xffffff);

	// init sprite with surface
	sprite = new Sprite(surface, 1);

	OutputDebugString("[LOG] Successfully created a sprite with the surface.\n");
}

GameObject::~GameObject()
{
	delete sprite;
}

void GameObject::Draw(Surface* screen)
{
	sprite->Draw(screen, SCRWIDTH / 2, SCRHEIGHT / 2);
}