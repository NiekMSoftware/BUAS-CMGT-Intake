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

ResourceHolder& ResourceHolder::Instance()
{
	static ResourceHolder s;
	return s;
}

bool ResourceHolder::LoadSprite(const char* fileName, const std::string& id, int numFrames)
{
	if (findResource(spriteContainer, id)) {
		OutputDebugString(("[WARNING] ResourceHolder::LoadSprite - Resource with id '" + id + "' already exists.\n").c_str());
		return true;
	}

	// try to retrieve the surface
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

	// by default return false
	return false;
}

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

Surface* ResourceHolder::GetSurface(const std::string& filePath)
{
	return findResource(surfaceContainer, filePath);
}

template<typename T>
T* ResourceHolder::findResource(const std::vector<std::pair<std::string, T*>>& container, const std::string& id)
{
	// retrieve resource u sing a predicate, return the pair's id
	auto it = std::find_if(container.begin(), container.end(),
		[&](const std::pair<std::string, T*>& pair) {
			return pair.first == id;
		}
	);

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
