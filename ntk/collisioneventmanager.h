#pragma once
class CollisionEventManager
{
public:
	CollisionEventManager() = default;
	~CollisionEventManager();

	void AddCollider(Collider* collider);
	void RemoveCollider(Collider* collider);

	void UpdateCollisions();

private:
	std::vector<Collider*> colliders;
	std::set<std::pair<Collider*, Collider*>> activeCollisions;
};

