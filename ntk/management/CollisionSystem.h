﻿#pragma once

struct CollisionEvent
{
	GameObject* other;
	float2 contactPoint;
	float2 normal;
};

// using from <function>, a callback that handles collision events and notifies its listeners
using CollisionCallback = std::function<void(const CollisionEvent&)>;

struct CollisionRegistration
{
	GameObject* object;
	CollisionCallback callback;

	bool operator==(const GameObject* other) const { return object == other; }
};

class CollisionSystem
{
public:
	static CollisionSystem& instance();
	void initialize();

	void registerObject(GameObject* obj, CollisionCallback callback);
	void unregisterObject(GameObject* obj);

	void update();

private:
	void checkCollision(GameObject* a, const CollisionCallback& cb1, GameObject* b, const CollisionCallback& cb2);
	bool checkAABB(const aabb& a, const aabb& b);

	// ----------
	// AI generated methods
	// 
		float2 calculateContactPoint(const aabb& a, const aabb& b) const;
		float2 calculateCollisionNormal(const aabb& a, const aabb& b) const;
	//
	// ----------

	std::vector<CollisionRegistration> m_collisionObjects;
};
