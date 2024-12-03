#include "precomp.h"
#include "collider.h"

Collider::Collider(float3 bmin, float3 bmax)
: id(0), bounds(bmin, bmax)
{ }

bool Collider::CheckCollision(const Collider* other) const
{
	aabb intersection = bounds.Intersection(other->bounds);
	return (intersection.Area() > 0);
}