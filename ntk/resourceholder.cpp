#include "precomp.h"
#include "resourceholder.h"

ResourceHolder::~ResourceHolder()
{
	Clean();
}

void ResourceHolder::Clean()
{
	RemoveResources(surfaceContainer);
	RemoveResources(spriteContainer);
}

// -----------------------------------------------------------
// Meyer's singleton instance, used to manage seversal resources (Sprites, Surfaces etc..).
// -----------------------------------------------------------
ResourceHolder& ResourceHolder::Instance()
{
	static ResourceHolder s;
	return s;
}

// -----------------------------------------------------------
// Loads in a sprite by assigning a file, id and the number of frames.
// -----------------------------------------------------------
bool ResourceHolder::LoadSprite(const char* fileName, const std::string& id, int numFrames)
{
	// Check if the resource already exists
	if (findResource(spriteContainer, fileName)) 
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
	Sprite* sprite = new Sprite(sfc, numFrames);
	if (sprite)
	{
		sprite->ownership = false;
		spriteContainer.emplace_back(id, sprite);
		return true;
	}

	// By default return false
	return false;
}

// -----------------------------------------------------------
// Retrieve the sprite with the ID that was given originally.
// -----------------------------------------------------------
Sprite* ResourceHolder::GetSprite(const std::string& id)
{
	// Attempt to find the sprite
	Sprite* sprite = findResource(spriteContainer, id);
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
	return findResource(surfaceContainer, filePath);
}

// -----------------------------------------------------------
// Find a resource from the specified container by using its ID.
// -----------------------------------------------------------
template<typename T>
T* ResourceHolder::findResource(const std::vector<std::pair<std::string, T*>>& container, const std::string& id)
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
void ResourceHolder::RemoveResources(std::vector<std::pair<std::string, T*>>& container)
{
	for (auto& pair : container)
	{
		OutputDebugString(("[LOG] ResourceHolder::RemoveResources() - Removing resource: " + pair.first + "\n").c_str());
		delete pair.second;
		pair.second = nullptr;
	}
	container.clear();
}
