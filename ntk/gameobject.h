#pragma once
class GameObject
{
public:
	GameObject();
	~GameObject();

	virtual void Update(float deltaTime);
	virtual void Draw(Surface* screen);

	void SetPosition(float2 newPos);
	float2 GetPosition() const;

private:
	float2 position;
	float moveSpeed =0.5f;

	Sprite* sprite;
};