#pragma once
class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

	virtual void Update(float deltaTime);
	virtual void Draw(Surface* screen);

	// position getter & setter
	void SetPosition(float2 newPos) { position = newPos; }
	float2 GetPosition() const { return position; }

	void CenterOrigin();

protected:
	// key components of game objects
	float2 position;
	Sprite* sprite;
};

class Ground : public GameObject
{
public: 
	Ground();
	~Ground() override;

private:
	int width, height;
};