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

Sprite* ResourceHolder::CreateSquare(const std::string& id, int w, int h)
{
	// Check if the sprite originally exists
	if (Sprite* existingSprite = FindExistingResource(spriteContainer, id))
	{
		// Log a warning and return the original sprite
		OutputDebugString(std::format("[WARNING] Sprite with ID '{}' already exists. Returning existing sprite.\n", id).c_str());
		return existingSprite;
	}

	// Create a surface filled with a specific color
	Surface* sqrSurface = new Surface(w, h);

	// Mark that we own this buffer and need to manage its memory
	sqrSurface->ownBuffer = false;
	surfaceContainer.emplace_back(id + "_surface", sqrSurface);

	// Create a single-frame sprite form this surface
	Sprite* sqrSprite = new Sprite(sqrSurface, 1);
	sqrSprite->ownership = false;
	spriteContainer.emplace_back(id, sqrSprite);

	return sqrSprite;
}

// -----------------------------------------------------------
// Loads in a sprite by assigning a file, id and the number of frames.
// -----------------------------------------------------------
bool ResourceHolder::LoadSprite(const char* filePath, const std::string& id, int numFrames)
{
	// Check if the resource already exists
	if (FindExistingResource(spriteContainer, filePath))
	{
		OutputDebugString(std::format("[WARNING] ResourceHolder::LoadSprite - Resource with filePath '{}' already exists.\n", filePath).c_str());
		return true;
	}

	// Try to retrieve the surface
	Surface* sfc = GetSurface(filePath);
	if (!sfc)
	{
		// If none is found, create a new one
		Surface* newSurface = new Surface(filePath);
		sfc = newSurface;
		sfc->ownBuffer = false;
		surfaceContainer.emplace_back(filePath, newSurface);
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
	Sprite* sprite = FindExistingResource(spriteContainer, id);
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
	return FindExistingResource(surfaceContainer, filePath);
}

// -----------------------------------------------------------
// Find a resource from the specified container by using its ID.
// -----------------------------------------------------------
template<typename T>
T* ResourceHolder::FindExistingResource(const std::vector<std::pair<std::string, T*>>& container, const std::string& id)
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