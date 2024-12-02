#pragma once
class ResourceHolder
{
public:
	// meyer's singleton
	static ResourceHolder& Instance();

	ResourceHolder(const ResourceHolder&) = delete;
	ResourceHolder& operator=(const ResourceHolder&) = delete;

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
	Surface* getSurface(const std::string& filePath);

	template <typename T>
	T* FindResource(const std::vector<std::pair<std::string, T*>>& container, const std::string& id);

	template <typename T>
	void RemoveResources(std::vector<std::pair<std::string, T*>>& container);

private:
	std::vector<std::pair<std::string, Surface*>> surfaceContainer;
	std::vector<std::pair<std::string, Sprite*>> spriteContainer;
};