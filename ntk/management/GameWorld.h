#pragma once

class GameWorld
{
public:
	static GameWorld& instance();
	~GameWorld();

	GameWorld(const GameWorld&) = delete;
	GameWorld& operator=(const GameWorld&) = delete;

	void initialize();

	void update();
	void fixedUpdate();
	void render(Surface* screen);

	void addObject(GameObject* go);
	void removeObject(GameObject* go);

	int getCount() const { return count; }

private:
	GameWorld() = default;

private:
	int count;
	int capacity;

	std::vector<GameObject*> worldObjects;
};