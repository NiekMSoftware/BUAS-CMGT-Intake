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
}

void GameWorld::clean()
{
	for (GameObject* obj : worldObjects)
	{
		OutputDebugString(std::format("[LOG] Removed '{}' from the Game Object list.\n", obj->getName()).c_str());
		delete obj;
	}
	worldObjects.clear();
}

void GameWorld::update()
{
	// TODO: Change this to an event
	// Remove inactive objects during update
	std::erase_if(worldObjects, [](const GameObject* obj) {
		    if (!obj || !obj->isActive()) {
			    delete obj; 
		    	return true;
		    }

			return false;
	    }
	);

	for (GameObject* obj : worldObjects)
	{
		if (obj && obj->isActive())
			obj->update();
	}
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
	// Validate input pointer
	if (!go) {
		OutputDebugString("[ERROR] Attempting to add null GameObject!\n");
		return;
	}

	// Debug: Print pointer address before insertion
	OutputDebugString(std::format("[DEBUG] Adding object at address: {}\n",
		reinterpret_cast<uintptr_t>(go)).c_str());

	// increase the capacity if more objects would be needed
	if (count >= capacity)
	{
		capacity *= 2;
		try
		{
			worldObjects.reserve(capacity);
			OutputDebugString("[LOG] Increased object capacity safely!\n");
		}
		catch (const std::bad_alloc&)
		{
			OutputDebugString("[CRITICAL] Failed to allocate memory!\n");
			return;
		}
	}

	// add the new game object
	worldObjects.push_back(go);
	count++;

	// validate insertion
	if (worldObjects.back() != go) 
	{
		OutputDebugString("[ERROR] Object insertion failed!\n");
	}

	OutputDebugString(std::format("[LOG] Added game object '{}' to the list.\n", go->getName()).c_str());
}

void GameWorld::removeObject(GameObject* go)
{
	if (go) {
		go->markForRemoval();
		count--;
		OutputDebugString(std::format("[LOG] Marked object '{}' for removal.\n", go->getName()).c_str());
	}
}
