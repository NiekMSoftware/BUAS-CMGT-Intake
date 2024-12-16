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

protected:
	float2 position;

	Sprite* m_sprite;
	aabb collider;
};