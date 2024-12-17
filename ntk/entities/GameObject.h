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

private:
	void keepInView();

protected:
	void applySpaceBraking(float brakeForce, float fixedDeltaTime);

protected:
	float2 position;
	float angle;

	// physics
	float2 velocity;

	Sprite* m_sprite;
	aabb collider;
};