#pragma once
class ResourceHolder
{
public:
	// meyer's singleton
	static ResourceHolder& Instance();

	ResourceHolder(const ResourceHolder&) = delete;
	ResourceHolder& operator=(const ResourceHolder&) = delete;

	// creating custom resources
	Surface* CreateSurface(const std::string& id, int width, int height);
	Sprite* CreateSprite(const std::string& id, Surface* surface, int numFrames);

	// loading
	bool LoadSprite(const char* fileName, const std::string& id, int numFrames);

	// retrieving
	Sprite* GetSprite(const std::string& id);

private:
	ResourceHolder() = default;
	
	// Cleanup
	~ResourceHolder();
	void Clean();

	// getters
	Surface* GetSurface(const std::string& filePath);

	template <typename T>
	T* findResource(const std::vector<std::pair<std::string, T*>>& container, const std::string& id);

	template <typename T>
	void RemoveResources(std::vector<std::pair<std::string, T*>>& container, const std::string& containerName);

private:
	std::vector<std::pair<std::string, Surface*>> surfaceContainer;
	std::vector<std::pair<std::string, Sprite*>> spriteContainer;
};