#include "precomp.h"
#include "ResourceHolder.h"

ResourceHolder& ResourceHolder::Instance()
{
	static ResourceHolder s;
	return s;
}

ResourceHolder::~ResourceHolder()
{
	Clean();
}

void ResourceHolder::Clean()
{
	RemoveResource(surfaceContainer);
	RemoveResource(spriteContainer);
}

// -----------------------------------------------------------
// Loads in a sprite by assigning a file, id and the number of frames.
// -----------------------------------------------------------
bool ResourceHolder::LoadSprite(const char* fileName, const std::string& id, int numFrames)
{
	// Check if the resource already exists
	if (FindComponent(spriteContainer, fileName))
	{
		OutputDebugString(std::format("[WARNING] ResourceHolder::LoadSprite - Resource with filePath '{}' already exists.\n", fileName).c_str());
		return true;
	}

	// Try to retrieve the surface
	Surface* sfc = GetSurface(fileName);
	if (!sfc)
	{
		// If none is found, create a new one
		Surface* newSurface = new Surface(fileName);
		sfc = newSurface;
		sfc->ownBuffer = false;
		surfaceContainer.emplace_back(fileName, newSurface);
	}

	// Allocate memory to a new sprite
	if (Sprite* sprite = new Sprite(sfc, numFrames))
	{
		sprite->ownership = false;
		spriteContainer.emplace_back(id, sprite);
		return true;
	}

	// By default, return false
	return false;
}

// -----------------------------------------------------------
// Retrieve the sprite with the ID that was given originally.
// -----------------------------------------------------------
Sprite* ResourceHolder::GetSprite(const std::string& id)
{
	// Attempt to find the sprite
	Sprite* sprite = FindComponent(spriteContainer, id);
	if (!sprite)
	{
		// return null and show the user an error
		std::cerr << "[ERROR] Sprite with ID: '" << id << "' not found.\n";
		return nullptr;
	}

	return sprite;
}

// -----------------------------------------------------------
// Retrieve the Surface by using its file path acting as an ID.
// -----------------------------------------------------------
Surface* ResourceHolder::GetSurface(const std::string& filePath)
{
	return FindComponent(surfaceContainer, filePath);
}

// -----------------------------------------------------------
// Find a resource from the specified container by using its ID.
// -----------------------------------------------------------
template<typename T>
T* ResourceHolder::FindComponent(const std::vector<std::pair<std::string, T*>>& container, const std::string& id)
{
	// Retrieve the resource using a predicate, return the pair's id
	auto it = std::find_if(container.begin(), container.end(),
		[&](const std::pair<std::string, T*>& pair) {
			return pair.first == id;
		}
	);

	// Return either the resource or nullptr to handle it off properly
	return it != container.end() ? it->second : nullptr;
}

template<typename T>
void ResourceHolder::RemoveResource(std::vector<std::pair<std::string, T*>>& container)
{
	// remove allocated memory to ensure cleanup
	for (auto& pair : container)
	{
		OutputDebugString(("[LOG] ResourceHolder::RemoveResources() - Removing resource: " + pair.first + "\n").c_str());
		delete pair.second;
		pair.second = nullptr;
	}
	container.clear();
}