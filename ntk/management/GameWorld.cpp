#include "precomp.h"
#include "GameWorld.h"

#include <print>

GameWorld& GameWorld::instance()
{
	static GameWorld s;
	return s;
}

void GameWorld::initialize()
{
	count = 0;
	capacity = 10;

	worldObjects.clear();
	worldObjects.reserve(capacity);

	std::println("Initialized GameWorld.");
}

void GameWorld::clean()
{
	for (GameObject* obj : worldObjects)
	{
		OutputDebugString(std::format("Removed '{}' from the Game Object list.\n", obj->getName()).c_str());
		delete obj;
	}
	worldObjects.clear();
}

void GameWorld::update()
{
	for (GameObject* obj : worldObjects)
	{
		if (obj && obj->isActive())
			obj->update();
	}

	// TODO: Change this to an event
	// Remove inactive objects during update
	std::erase_if(worldObjects,
	              [](GameObject* obj) {
		              if (!obj || !obj->isActive()) {
			              delete obj;
			              return true;
		              }
		              return false;
	              });
}

void GameWorld::fixedUpdate() const
{
	for (GameObject* obj : worldObjects)
	{
		// Double-check object state before rendering
		if (obj && obj->isActive())
			obj->fixedUpdate();
	}
}

void GameWorld::render(Surface* screen) const
{
	for (GameObject* obj : worldObjects) {
		// Double-check object state before rendering
		if (obj && obj->isActive()) {
			obj->render(screen);
		}
	}
}

void GameWorld::addObject(GameObject* go)
{
	// increase the capacity if more objects would be needed
	if (count >= capacity)
	{
		capacity *= 2;
		worldObjects.resize(capacity);
		OutputDebugString("No more room, making some more capacity!\n");
	}

	// add the new game object
	worldObjects.push_back(go);
	count++;
	OutputDebugString(std::format("Added game object '{}' to the list.\n", go->getName()).c_str());
}

void GameWorld::removeObject(GameObject* go)
{
	if (go) {
		go->markForRemoval();
		count--;
		OutputDebugString(std::format("Marked object '{}' for removal.\n", go->getName()).c_str());
	}
}
