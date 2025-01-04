#pragma once

enum class Layer;
struct CollisionEvent;

class GameObject
{
public:
	virtual void initialize();
	virtual ~GameObject() = default;

	// rendering and functional computing
	virtual void update();
	virtual void fixedUpdate();
	virtual void render(Surface* screen);
	void renderCollider(Surface* target, uint color = 0xFF0000) const;

	virtual void onCollision(const CollisionEvent&);

	// getters
	float2 getPosition() const;
	aabb getCollider() const;
	std::string getName() const { return name; }
	float2 getVelocity() const { return velocity; }

	Sprite* getSprite() const;
	Layer getLayer() const { return m_layer; }

	bool isActive() const { return m_active; }
	bool isObjectPooled() const { return isPooled; }

	float getAngle() const { return angle; }
	float getScale() const { return scale; }

	// setters
	void setActive(const bool v) { m_active = v; }
	void setPosition(const float2 np) { position = np; }
	void setScale(const float v) { scale = v; }
	void setRotation(const float v) { angle = v; }
	void setVelocity(const float2 newVelocity) { velocity = newVelocity; }

	void setRotationSpeed(float v) { rotSpeed = v; }
	void setRandomRotation() { angle = 0.0f; }
	void setRandomVelocity(const float v) { velocity = v; }

	void setSprite(Sprite* newSprite) { m_sprite = newSprite; }
	void setLayer(Layer layer) { m_layer = layer; }

	void setPooled(bool pooled) { isPooled = pooled; }

	// transformation updates
	void translate(const float2& translation);
	void rotate(const float& a);

	virtual void markForRemoval() { m_active = false; }

protected:
	void initializeCollider();
	void updateCollider();

	void keepInView();

protected:
	// attributes
	float2 position = 0;
	float2 velocity = 0;
	float scale = 1;
	float angle = 0;
	float rotSpeed = 1.f;

	bool isPooled = false;
	bool m_collision = false;

	// components
	Sprite* m_sprite = nullptr;
	Layer m_layer = {};
	aabb collider;

	std::string name;

private:
	bool m_active = true;

};