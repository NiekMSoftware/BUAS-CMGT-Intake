#include "precomp.h"
#include "wall.h"

Wall::Wall()
: width(16), height(16)
{
	initialize();
}

Wall::Wall(int w, int h)
: width(w), height(h)
{
	initialize();
}

void Wall::initialize()
{
	ResourceHolder& rh = ResourceHolder::Instance();

	Surface* sfc = new Surface(width, height);
	sfc->Bar(0, 0, width, height, 0x00FF00);

	sprite = rh.CreateSprite("ground", sfc, 1);
	CenterOrigin();

	InitializeCollider();

	position = { 100, 300 };

	UpdateColliderPosition();
}