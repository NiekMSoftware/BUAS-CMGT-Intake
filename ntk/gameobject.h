#pragma once
class GameObject
{
public:
	GameObject();
	~GameObject();

	void Draw(Surface* screen);

private:
	Sprite* sprite;
};