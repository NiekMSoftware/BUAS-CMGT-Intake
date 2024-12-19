#pragma once

class GameObject
{
public:
	explicit GameObject();
	virtual ~GameObject();

	// rendering and functional computing
	virtual void update();
	virtual void fixedUpdate();
	virtual void render(Surface* screen);

	// getters
	float2 getPosition() const;
	aabb getCollider() const;
	Sprite* getSprite() const;
	std::string getName() const { return name; }
	bool isActive() const { return m_active; }

	// transformation updates
	void translate(const float2& translation);
	void rotate(const float& a);

	virtual void markForRemoval() { m_active = false; }

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

	std::string name;

private:
	bool m_active = true;
};