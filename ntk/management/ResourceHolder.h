#pragma once

class ResourceHolder
{
public:
	// meyer's singleton
	static ResourceHolder& Instance();

	ResourceHolder(const ResourceHolder&) = delete;
	ResourceHolder& operator=(const ResourceHolder&) = delete;

	// Loading
	bool LoadSprite(const char* filePath, const std::string& id, int numFrames);

	// retrieving
	Sprite* GetSprite(const std::string& id);

private:
	ResourceHolder() = default;

	// Cleanup
	~ResourceHolder();
	void Clean();

	Surface* GetSurface(const std::string& filePath);

	template<typename T>
	T* FindResource(const std::vector<std::pair<std::string, T*>>& container);

	template<typename T>
	void RemoveResource(std::vector<std::pair<std::string, T*>>& container);

private:
	std::vector<std::pair<std::string, Surface*>> surfaceContainer;
	std::vector<std::pair<std::string, Sprite*>> spriteContainer;
};
