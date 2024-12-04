#include "precomp.h"
#include "collider.h"

Collider::Collider(float3 bmin, float3 bmax)
: id(0), bounds(bmin, bmax), colliderOffset(0, 0)
{ }

bool Collider::CheckCollision(const Collider* other) const
{
    bool intersects = bounds.Intersects2D(other->bounds);
    return intersects;
}

void Collider::InitializeCollider(const float2& objectPosition, float w, float h, const float2& customOffset)
{
	// Combine object position with any custom offset
	float2 colliderCenter = objectPosition + colliderOffset + customOffset;

	float halfW = w / 2.0f;
	float halfH = h / 2.0f;

	bounds.bmin[0] = colliderCenter.x - halfW;
	bounds.bmin[1] = colliderCenter.x - halfH;

	bounds.bmax[0] = colliderCenter.x + halfW;
	bounds.bmax[1] = colliderCenter.y + halfH;
}

void Collider::Render(Surface* screen, uint color)
{
	if (!screen) return;

	// Convert AABB to screen-space coordinates
	int x1 = static_cast<int>(bounds.bmin[0]);
	int y1 = static_cast<int>(bounds.bmin[1]);
	int x2 = static_cast<int>(bounds.bmax[0]);
	int y2 = static_cast<int>(bounds.bmax[1]);

	// Draw the box outline
	screen->Box(x1, y1, x2, y2, color);
}
