#pragma once

class AudioManager
{
public:
	// Singleton.
	AudioManager() = default;
	~AudioManager();
	AudioManager(const AudioManager&) = delete;
	AudioManager& operator=(const AudioManager&) = delete;

	static AudioManager& instance();

	bool initialize();
	void shutdown();

private:
	ma_engine engine;
	bool initialized;
};
