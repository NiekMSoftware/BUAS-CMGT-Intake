#pragma once

class GameObject
{
public:
	explicit GameObject();
	virtual ~GameObject();

	virtual void update(float deltaTime);
	virtual void fixedUpdate(float fixedDeltaTime);
	virtual void render(Surface* screen);

	float2 getPosition() const;
	aabb getCollider() const;
	Sprite* getSprite() const;

	void translate(const float2& translation);
	void rotate(const float& a);

protected:
	void applySpaceBraking(float brakeForce, float fixedDeltaTime);

protected:
	float2 position;
	float angle;

	// physics
	float mass = 1.f;
	float2 velocity;
	float dragCoefficient;
	const float FLUID_DENSITY = 1.225f;
	const float EPSILON = 0.01f;

	Sprite* m_sprite;
	aabb collider;
};