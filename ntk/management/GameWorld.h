#pragma once

class GameWorld
{
public:
	static GameWorld& instance();

	GameWorld(const GameWorld&) = delete;
	GameWorld& operator=(const GameWorld&) = delete;

	void initialize();
	void clean();

	void update();
	void fixedUpdate() const;
	void render(Surface* screen) const;

	void addObject(GameObject* go);
	void removeObject(GameObject* go);

	int getCount() const { return count; }
	std::vector<GameObject*>& getWorldObjects() { return worldObjects; }

private:
	GameWorld() = default;

private:
	int count;
	int capacity;

	std::vector<GameObject*> worldObjects;
};