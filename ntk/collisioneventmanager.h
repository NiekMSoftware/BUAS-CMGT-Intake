#pragma once
class CollisionEventManager
{
public:
	CollisionEventManager() = default;
	~CollisionEventManager();

	// prevent copying
	CollisionEventManager(const CollisionEventManager&) = delete;
	CollisionEventManager& operator=(const CollisionEventManager&) = delete;

	void AddCollider(Collider* collider);
	void RemoveCollider(Collider* collider);

	void UpdateCollisions();

private:
	std::vector<Collider*> colliders;

	/** https://en.cppreference.com/w/cpp/container/set */
	std::set<std::pair<Collider*, Collider*>> activeCollisions;
};

