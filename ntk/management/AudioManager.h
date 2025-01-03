#pragma once

class AudioManager
{
public:
	enum AudioGroup
	{
		Master,
		Music,
		Sfx
	};

public:
	static AudioManager& instance()
	{
		static AudioManager s;
		return s;
	}

	AudioManager(const AudioManager&) = delete;
	AudioManager& operator=(const AudioManager&) = delete;

	bool initOpenAl();
	void shutdown();

	// Audio functions
	void loadAudioFile(const std::string& filePath, const std::string& id);
	ALuint* retrieveAudioBuffer(const std::string& id);

	// Managing groups
	void setGroupVolume(AudioGroup group, float volume);
	float getGroupVolume(AudioGroup group) const;

	void assignGroup(ALuint source, AudioGroup group);

private:
	struct AudioGroupData
	{
		float volume = 1.f;
		std::vector<ALuint> sources;
	};

	AudioManager() : al_device(nullptr), al_context(nullptr), m_masterVolume(1.f) { }
	~AudioManager();

private:
	std::vector<std::pair<AudioGroup, AudioGroupData>> m_audioGroups;

	ALCdevice* al_device;
	ALCcontext* al_context;

	std::vector<std::pair<std::string, ALuint*>> m_audioCache;
	float m_masterVolume;
};
