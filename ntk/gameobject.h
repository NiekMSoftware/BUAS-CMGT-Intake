#pragma once
class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

	virtual void Update(float deltaTime);
	virtual void Draw(Surface* screen);

	// position getter & setter
	void SetPosition(float2 newPos);
	float2 GetPosition() const { return position; }

	Collider* GetCollider() { return collider; }

	void CenterOrigin();

protected:
	void InitializeCollider();
	void UpdateColliderPosition();

protected:
	// key components of game objects
	float2 position;
	Sprite* sprite;
	Collider* collider;
};