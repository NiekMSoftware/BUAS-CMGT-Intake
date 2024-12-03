#pragma once
class Collider
{
public:
	Collider(float3 bmin, float3 bmax);

	// Collision event callbacks
	std::function<void(const Collider*)> OnCollisionEnter;
	std::function<void(const Collider*)> OnCollisionStay;
	std::function<void(const Collider*)> OnCollisionExit;

	bool CheckCollision(const Collider* other) const;

public:
	// unique identifier for this collider
	uint32_t id;

	aabb bounds;
};