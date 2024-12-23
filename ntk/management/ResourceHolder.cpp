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

Sprite* ResourceHolder::CreateSquare(const std::string& id, int w, int h, uint c)
{
	Sprite* sprite = FindExistingResource(spriteContainer, id);

	// Check if the sprite originally exists
	if (sprite)
	{
		OutputDebugString(std::format("[LOG] ResourceHolder::CreateSquare - Sprite with the id '{}' already exists, using that one.\n\n", id).c_str());
		return sprite;
	}

	OutputDebugString(std::format("[LOG] ResourceHolder::CreateSquare - Sprite with ID '{}' was not found. Creating new one.\n", id).c_str());

	// Create a surface filled with a specific color
	Surface* sqrSurface = new Surface(w, h);

	// Mark that we own this buffer and need to manage its memory
	sqrSurface->ownBuffer = false;
	sqrSurface->Bar(0, 0, w, h, c);
	surfaceContainer.emplace_back(id + "_surface", sqrSurface);

	// Create a single-frame sprite form this surface
	sprite = new Sprite(sqrSurface, 1);
	sprite->ownership = false;
	spriteContainer.emplace_back(id, sprite);

	OutputDebugString(std::format("[LOG] ResourceHolder::CreateSquare - Successfully created a square sprite, with id '{}'\n\n", id).c_str());

	return sprite;
}

// -----------------------------------------------------------
// Loads in a sprite by assigning a file, id and the number of frames.
// -----------------------------------------------------------
bool ResourceHolder::LoadSprite(const char* filePath, const std::string& id, int numFrames)
{
	// Check if the sprite already exists in the container, if return early.
	Sprite* sprite = FindExistingResource(spriteContainer, id);
	if (sprite)
	{
		OutputDebugString(std::format("[LOG] ResourceHolder::LoadSprite - Sprite with id '{}' already exists. Using the existing sprite\n", id).c_str());
		return true;
	}

	OutputDebugString(std::format("[LOG] ResourceHolder::LoadSprite - Sprite with id '{}' not found. Creating a new sprite\n", id).c_str());

	// Try to retrieve the surface
	Surface* sfc = GetSurface(filePath);
	if (!sfc)
	{
		// If none is found, create a new one
		OutputDebugString(std::format("[LOG] ResourceHolder::LoadSprite - Surface with filePath '{}' doesn't exist yet, allocating memory to a new surface\n", filePath).c_str());

		Surface* newSurface = new Surface(filePath);
		sfc = newSurface;
		sfc->ownBuffer = false;
		surfaceContainer.emplace_back(filePath, newSurface);
	}

	// Create a new sprite
	sprite = new Sprite(sfc, numFrames);
	if (sprite)
	{
		sprite->ownership = false;
		spriteContainer.emplace_back(id, sprite);
		OutputDebugString(std::format("[LOG] ResourceHolder::LoadSprite - Successfully created and stored the new sprite with ID '{}'\n\n", id).c_str());
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

	OutputDebugString(std::format("[LOG] ResourceHolder::GetSprite - Successfully retrieved sprite with ID '{}' from the sprite container\n\n", id).c_str());
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

	OutputDebugString("[LOG] ResourceHolder::RemoveResources() - Successfully cleared out the container.\n\n");
}