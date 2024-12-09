#pragma once
class GameObject
{
public:
	explicit GameObject();
	virtual ~GameObject();

	virtual void Update(float deltaTime);
	virtual void FixedUpdate(float fixedDeltaTime);
	virtual void Draw(Surface* screen);

	void Translate(const float2& translation);

	// getters & setters
	void SetPosition(float2 newPos);
	float2 GetPosition() const { return position; }

	void SetVelocity(float2 v) { velocity = v; }
	float2 GetVelocity() const { return velocity; }

	float2 GetForce() const { return force; }

	Collider* GetCollider() { return collider; }
	virtual void OnCollisionEnter(const Collider* other);
	virtual void OnCollisionStay(const Collider* other);
	virtual void OnCollisionExit(const Collider* other);

	void CenterOrigin();

protected:
	void InitializeCollider();
	void UpdateColliderPosition();

	void ClampSpeed(float deltaTime);
	void ApplyDrag(float fixedDeltaTime);

protected:
	// key components of game objects
	float2 position;
	
	// physics
	float2 velocity;
	float2 force;
	float dragCoefficient;
	float mass;

	Sprite* sprite;
	Collider* collider;

	float maxSpeed;
};