#include "precomp.h"
#include "CollisionSystem.h"

CollisionSystem& CollisionSystem::instance()
{
	static CollisionSystem s;
	return s;
}

void CollisionSystem::initialize()
{
	m_collisionObjects.reserve(100);
}

void CollisionSystem::registerObject(GameObject* obj, CollisionCallback callback)
{
	if (!obj) return;

	// Check if object is already registered
	auto it = ranges::find_if(m_collisionObjects,
	                          [obj](const CollisionRegistration& reg) { return reg.object == obj; });

	if (it == m_collisionObjects.end())
	{
		// Only add if not already present
		CollisionRegistration registration{ .object= obj, .callback= callback};
		m_collisionObjects.push_back(registration);
	}
}

void CollisionSystem::unregisterObject(GameObject* obj)
{
	// Remove object if found
	auto it = ranges::find_if(m_collisionObjects,
		[obj](const CollisionRegistration& reg) { return reg.object == obj; });

	if (it != m_collisionObjects.end())
	{
		// If it's not the last element, swap with last to avoid shifting elements
		if (it != m_collisionObjects.end() - 1)
		{
			*it = m_collisionObjects.back();
		}
		m_collisionObjects.pop_back();
	}
}

void CollisionSystem::update()
{
	// Remove any inactive objects first
	std::erase_if(m_collisionObjects,
	  [](const CollisionRegistration& reg)
	  {
	      return !reg.object || !reg.object->isActive();
	  }
	);

	const size_t objectCount = m_collisionObjects.size();
	for (size_t i = 0; i < objectCount; i++)
	{
		for (size_t j = 0; j < objectCount; j++)
		{
			checkCollision(
				m_collisionObjects[i].object,
				m_collisionObjects[i].callback,
				m_collisionObjects[j].object,
				m_collisionObjects[j].callback
			);
		}
	}
}

void CollisionSystem::checkCollision(GameObject* a, const CollisionCallback& cb1, GameObject* b,
	const CollisionCallback& cb2)
{
	aabb col1 = a->getCollider();
	aabb col2 = b->getCollider();

	if (checkAABB(col1, col2))
	{
		float2 cp = calculateContactPoint(col1, col2);
		float2 n = calculateCollisionNormal(col1, col2);

		// notify
		cb1(CollisionEvent{ .other= b, .contactPoint= cp, .normal = n});
		cb2(CollisionEvent{ .other= a, .contactPoint= cp, .normal = n});
	}
}

bool CollisionSystem::checkAABB(const aabb& a, const aabb& b)
{
	return (a.bmin[0] <= b.bmax[0] && a.bmax[0] >= b.bmin[0]) &&
			(a.bmin[1] <= b.bmax[1] && a.bmax[1] >= b.bmin[1]);
}

float2 CollisionSystem::calculateContactPoint(const aabb& a, const aabb& b) const
{
	return float2{
		(std::max(a.bmin[0], b.bmin[0]) + std::min(a.bmax[0], b.bmax[0])) * 0.5f,
		(std::max(a.bmin[1], b.bmin[1]) + std::min(a.bmax[1], b.bmax[1])) * 0.5f
	};
}

float2 CollisionSystem::calculateCollisionNormal(const aabb& a, const aabb& b) const
{
	float2 centerA = {(a.bmin[0] + a.bmax[0]) * 0.5f, (a.bmin[1] + a.bmax[1]) * 0.5f};
	float2 centerB = {(b.bmin[0] + b.bmax[0]) * 0.5f, (b.bmin[1] + b.bmax[1]) * 0.5f};

	float2 normal = { centerB.x - centerA.x, centerB.y - centerA.y };
	float l = length(normal);

	if (l > 0)
	{
		normal.x /= l;
		normal.y /= l;
	}

	return normal;
}
