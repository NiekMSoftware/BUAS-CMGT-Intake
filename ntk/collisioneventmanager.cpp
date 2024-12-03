#include "precomp.h"
#include "collisioneventmanager.h"

CollisionEventManager::~CollisionEventManager()
{
	for (Collider* collider : colliders)
	{
		delete collider;
	}

	colliders.clear();
}

void CollisionEventManager::AddCollider(Collider* collider)
{
	colliders.push_back(collider);
}

void CollisionEventManager::RemoveCollider(Collider* collider)
{
	colliders.erase(
		std::remove(colliders.begin(), colliders.end(), collider),
		colliders.end()
	);
}

void CollisionEventManager::UpdateCollisions()
{
	// new set to track current frame's collisions
	std::set<std::pair<Collider*, Collider*>> currentCollisions;

	// Check collisions between all colliders
	for (size_t i = 0; i < colliders.size(); ++i)
	{
		for (size_t j = i + 1; j < colliders.size(); ++j)
		{
			Collider* a = colliders[i];
			Collider* b = colliders[j];

			if (a->CheckCollision(b))
			{
				auto collisionPair = std::make_pair(a, b);
				currentCollisions.insert(collisionPair);

				// Trigger events based on collision state
				if (activeCollisions.find(collisionPair) == activeCollisions.end())
				{
					// New Collision - Trigger Enter event
					if (a->OnCollisionEnter) a->OnCollisionEnter(b);
					if (b->OnCollisionEnter) b->OnCollisionEnter(a);
				}
				else {
					// Continueing collision - Trigger Stay event
					if (a->OnCollisionStay) a->OnCollisionStay(b);
					if (b->OnCollisionStay) b->OnCollisionStay(a);
				}
			}
		}
	}

	// Check for collision exits
	for (const auto& prevCollision : activeCollisions)
	{
		if (currentCollisions.find(prevCollision) == currentCollisions.end()) {
			// Collision ended - Trigger Exit event
			if (prevCollision.first->OnCollisionExit) 
				prevCollision.first->OnCollisionExit(prevCollision.second);
			if (prevCollision.second->OnCollisionExit)
				prevCollision.second->OnCollisionExit(prevCollision.first);
		}
	}

	// Update active collisions for next frame
	activeCollisions = currentCollisions;
}
