#pragma once

class GameObject
{
public:
	virtual void initialize();
	virtual ~GameObject() = default;

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

	float getAngle() const { return angle; }
	float getScale() const { return scale; }

	// setters
	void setActive(const bool v) { m_active = v; }
	void setPosition(const float2 np) { position = np; }
	void setScale(const float v) { scale = v; }
	void setRotation(const float v) { angle = v; }
	
	void setRandomRotation() { angle = 0.0f; }
	void setRandomVelocity(const float v) { velocity = v; }
	void setSprite(Sprite* newSprite) { m_sprite = newSprite; }

	// transformation updates
	void translate(const float2& translation);
	void rotate(const float& a);

	virtual void markForRemoval() { m_active = false; }

protected:
	void keepInView();

protected:
	// attributes
	float2 position = 0;
	float2 velocity = 0;
	float scale = 1;
	float angle = 0;

	// components
	Sprite* m_sprite = nullptr;
	aabb collider;

	std::string name;

private:
	bool m_active = true;
};