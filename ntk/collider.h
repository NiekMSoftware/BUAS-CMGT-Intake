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

	void InitializeCollider(const float2& objectPosition, float w, float h, const float2& customOffset = {0, 0});
	void Render(Surface* screen, uint color = 0xFF0000);

public:
	// unique identifier for this collider
	uint32_t id;

	aabb bounds;
	float2 colliderOffset;
};