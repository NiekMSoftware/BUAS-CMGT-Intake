#include "precomp.h"
#include "item.h"

Item::Item(float w, float h)
	: width(w), height(h)
{
	initialize();
}

void Item::initialize()
{
	ResourceHolder& rh = ResourceHolder::Instance();

	Surface* sfc = new Surface(static_cast<int>(width), static_cast<int>(height));
	sfc->Bar(0, 0, static_cast<int>(width), static_cast<int>(height), 0xFFFF6F61);

	sprite = rh.CreateSprite("item", sfc, 1);
	CenterOrigin();

	InitializeCollider();

	position = { 700, 300 };

	UpdateColliderPosition();
}