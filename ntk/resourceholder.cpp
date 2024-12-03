#include "precomp.h"
#include "resourceholder.h"

ResourceHolder::~ResourceHolder()
{
	Clean();
}

void ResourceHolder::Clean()
{
	RemoveResources(surfaceContainer, "Surface Container");
	RemoveResources(spriteContainer, "Sprite Container");
	OutputDebugString("Cleaned up all of the containers.\n\n");
}

// -----------------------------------------------------------
// Meyer's singleton instance, used to manage seversal resources (Sprites, Surfaces etc..).
// -----------------------------------------------------------
ResourceHolder& ResourceHolder::Instance()
{
	static ResourceHolder s;
	return s;
}

Surface* ResourceHolder::CreateSurface(const std::string& id, int width, int height)
{
	Surface* surface = GetSurface(id);

	// Check if the surface already exists if so use that one to avoid duplicates
	if (surface != nullptr)
	{
		OutputDebugString(std::format("[WARNING] Surface with ID '{}' already exists, using that one.\n", id).c_str());
		return surface;
	}

	// Create a new surface
	OutputDebugString(std::format("\n[LOG] Surface with ID '{}' doesn't exist, creating a new one.\n", id).c_str());
	surface = new Surface(width, height);
	surface->ownBuffer = false;
	
	// Add to container with given ID
	surfaceContainer.emplace_back(id, surface);

	return surface;
}

Sprite* ResourceHolder::CreateSprite(const std::string& id, Surface* surface, int numFrames)
{
	Sprite* sprite = GetSprite(id);

	// Check if sprite with this ID already exists
	if (sprite != nullptr)
	{
		// return the already existing sprite to avoid duplicates
		OutputDebugString(std::format("[WARNING] Sprite with ID '{}' already exists, using that one.\n", id).c_str());
		return sprite;
	}
	else {
		// Create a new sprite
		OutputDebugString(std::format("[LOG] Sprite with ID '{}' doesn't exist, creating a new one.\n", id).c_str());
		sprite = new Sprite(surface, numFrames);
		sprite->ownership = false;

		// Add to container
		spriteContainer.emplace_back(id, sprite);

		return sprite;
	}

	return nullptr;
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
	return findResource(spriteContainer, id);
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
void ResourceHolder::RemoveResources(std::vector<std::pair<std::string, T*>>& container, const std::string& containerName)
{
	OutputDebugString(std::format("\n[LOG] Removing resources from container '{}'\n", containerName).c_str());
	for (auto& pair : container)
	{
		OutputDebugString(("[LOG] ResourceHolder::RemoveResources() - Removing resource: " + pair.first + "\n").c_str());
		delete pair.second;
		pair.second = nullptr;
	}
	container.clear();

	OutputDebugString(std::format("[LOG] Cleaned up container '{}'!\n", containerName).c_str());
}
