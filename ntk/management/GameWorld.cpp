﻿#include "precomp.h"
#include "GameWorld.h"

#include <print>

GameWorld& GameWorld::instance()
{
	static GameWorld s;
	return s;
}

/**
 * Initializes the entire game world, it sets a fixed number as initial capacity and reserves
 * elements for the GameObjects with the explicit capacity.
 *
 * @remark Explicitly call this upon the Game's initialization.
 */
void GameWorld::initialize()
{
	count = 0;
	capacity = 25;

	worldObjects.clear();
	worldObjects.reserve(capacity);

	OutputDebugString("\n[LOG] GameWorld::initialize - Successfully initialized the Game World.\n\n");
}

/**
 * Cleans up all the game objects of the world, freeing all previously allocated memory.
 *
 * @remark Explicitly call this upon the Game's shutdown.
 */
void GameWorld::clean()
{
	for (GameObject* obj : worldObjects)
	{
		OutputDebugString(std::format("[LOG] GameWorld::clean - Removed '{}' from the Game Object list.\n", obj->getName()).c_str());
		delete obj;
	}
	worldObjects.clear();

	OutputDebugString("[LOG] GameWorld::clean - Successfully cleaned up the Game World.\n\n");
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
		// Double-check object state before updating
		if (obj && obj->isActive())
			obj->update();
	}
}

void GameWorld::fixedUpdate() const
{
	for (GameObject* obj : worldObjects)
	{
		// Double-check object state before updating
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

/** Adds a game object to the game world. */
void GameWorld::addObject(GameObject* go)
{
	// Validate input pointer
	if (!go)
	{
		OutputDebugString("[ERROR] GameWorld::addObject - Attempting to add null GameObject!\n");
		return;
	}

	// Debug: Print pointer address before insertion
	OutputDebugString(std::format("[DEBUG] GameWorld::addObject - Adding object at address: {}\n",
		reinterpret_cast<uintptr_t>(go)).c_str());

	// increase the capacity if more objects would be needed
	if (count >= capacity)
	{
		capacity *= 2;
		try
		{
			worldObjects.reserve(capacity);
			OutputDebugString("[LOG] GameWorld::addObject - Increased object capacity safely!\n");
		}
		catch (const std::bad_alloc&)
		{
			OutputDebugString("[CRITICAL] GameWorld::addObject - Failed to allocate memory!\n");
			return;
		}
	}

	// initialize and add the new game object
	go->initialize();
	worldObjects.push_back(go);
	count++;

	// validate insertion
	if (worldObjects.back() != go) 
	{
		OutputDebugString("[ERROR] GameWorld::addObject - Object insertion failed!\n");
	}

	OutputDebugString(std::format("[LOG] GameWorld::addObject - Added game object '{}' to the list.\n\n", go->getName()).c_str());
}

/** Remove any game object existing in the world. This also decreases the count of the container. */
void GameWorld::removeObject(GameObject* go)
{
	if (go && go->isActive())
	{
		go->markForRemoval();
		count--;
		OutputDebugString(std::format("[LOG] GameWorld::removeObject - Marked the Game Object for removal '{}'.\n\n", go->getName()).c_str());
	}
}