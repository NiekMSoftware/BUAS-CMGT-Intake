#include "precomp.h"
#include "GameWorld.h"

#include <print>

GameWorld& GameWorld::instance()
{
	static GameWorld s;
	return s;
}

GameWorld::~GameWorld()
{
	for (GameObject* obj : worldObjects)
	{
		OutputDebugString("Removed game object from the vector.\n");
		delete obj;
	}
	worldObjects.clear();
}

void GameWorld::initialize()
{
	count = 0;
	capacity = 10;

	worldObjects.clear();
	worldObjects.reserve(capacity);

	std::println("Initialized GameWorld.");
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

void GameWorld::fixedUpdate()
{
	for (GameObject* obj : worldObjects)
	{
		// Double-check object state before rendering
		if (obj && obj->isActive())
			obj->fixedUpdate();
	}
}

void GameWorld::render(Surface* screen)
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
		std::println("No more room, making some more capacity!");
	}

	// add the new game object
	worldObjects.push_back(go);
	count++;
	std::println("Added the game object.");
}

void GameWorld::removeObject(GameObject* go)
{
	if (go) {
		go->markForRemoval();
		count--;
		std::println("Marked game object for removal.");
	}
}
