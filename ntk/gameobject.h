#pragma once
class GameObject
{
public:
	explicit GameObject();
	virtual ~GameObject();

	virtual void Update(float deltaTime);
	virtual void Draw(Surface* screen);

	// getters & setters
	void SetPosition(float2 newPos);
	float2 GetPosition() const { return position; }

	void SetVelocity(float2 v) { velocity = v; }
	float2 GetVelocity() const { return velocity; }

	Collider* GetCollider() { return collider; }
	virtual void OnCollisionEnter(const Collider* other);
	virtual void OnCollisionStay(const Collider* other);
	virtual void OnCollisionExit(const Collider* other);

	void CenterOrigin();

protected:
	void InitializeCollider();
	void UpdateColliderPosition();

	void ClampSpeed(float deltaTime);
	void ApplyDrag(float deltaTime);

protected:
	// key components of game objects
	float2 position;

	float2 velocity;
	float maxSpeed;

	Sprite* sprite;
	Collider* collider;
};