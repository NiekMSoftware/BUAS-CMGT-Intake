#pragma once

class GameObject
{
public:
	explicit GameObject();
	virtual ~GameObject();

	// rendering and functional computing
	virtual void update(float deltaTime);
	virtual void fixedUpdate(float fixedDeltaTime);
	virtual void render(Surface* screen);

	// getters
	float2 getPosition() const;
	aabb getCollider() const;
	Sprite* getSprite() const;

	// transformation updates
	void translate(const float2& translation);
	void rotate(const float& a);

protected:
	void keepInView();

protected:
	// attributes
	float2 position;
	float2 velocity;
	float angle;

	// components
	Sprite* m_sprite;
	aabb collider;
};