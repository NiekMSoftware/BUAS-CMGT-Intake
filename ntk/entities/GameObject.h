#pragma once

class GameObject
{
public:
	explicit GameObject();
	virtual ~GameObject();

	virtual void update(float deltaTime);
	virtual void render(Surface* screen);

	float2 getPosition() const;
	aabb getCollider() const;
	Sprite* getSprite() const;

	void translate(const float2& translation);
	void rotate(const float& a);

protected:
	float2 position;
	float angle;

	Sprite* m_sprite;
	aabb collider;
};