#include "precomp.h"
#include "AudioManager.h"

#define DR_WAV_IMPLEMENTATION
#include "dr_wav.h"

AudioManager::~AudioManager()
{
	for (const auto& [fst, snd] : m_audioCache)
	{
		delete snd;
	}
	m_audioCache.clear();
}

bool AudioManager::initOpenAl()
{
	al_device = alcOpenDevice(nullptr);
	if (!al_device)
	{
		std::cerr << "AudioManager::initOpenAl () - Failed to open audio device\n";
		return false;
	}

	al_context = alcCreateContext(al_device, nullptr);
	if (!al_context)
	{
		std::cerr << "AudioManager::initOpenAl() - Failed to create context\n";
		return false;
	}

	if (!alcMakeContextCurrent(al_context))
	{
		std::cerr << "AudioManager::initOpenAl() - Failed to make context current\n";
		return false;
	}

	OutputDebugString("[LOG] Successfully initialized OpenAL\n");

	return true;
}

void AudioManager::shutdown()
{
	alcMakeContextCurrent(nullptr);
	if (al_context && al_device)
	{
		alcDestroyContext(al_context);
		al_context = nullptr;

		alcCloseDevice(al_device);
		al_device = nullptr;
	}
}

void AudioManager::loadAudioFile(const std::string& filePath, const std::string& id)
{
	// automatically manage the dr wav instance
	std::unique_ptr<drwav, decltype(&drwav_uninit)> wav(nullptr, drwav_uninit);
	wav.reset(new drwav);  // reset instance

	// throw an error if loading of file failed
	if (!drwav_init_file(wav.get(), filePath.c_str(), nullptr))
	{
		throw std::runtime_error(std::format("Failed to initialize file in path '{}'!", filePath));
	}

	// Read the audio data
	size_t dataSize = wav->totalPCMFrameCount * wav->channels * sizeof(drwav_int16);
	std::vector<drwav_int16> audioData;
	audioData.reserve(wav->totalPCMFrameCount * wav->channels);

	if (drwav_read_pcm_frames_s16(wav.get(), wav->totalPCMFrameCount, audioData.data()) != 
		wav->totalPCMFrameCount)
	{
		throw std::runtime_error("Failed to read audio data!");
	}

	// generate openAL buffer
	ALuint buffer;
	alGenBuffers(1, &buffer);

	// decide format
	ALenum format;
	if (wav->channels == 1)
		format = AL_FORMAT_MONO16;
	else if (wav->channels == 2)
		format = AL_FORMAT_STEREO16;
	else
		throw std::runtime_error("Unsupported number of channels!");

	alBufferData(buffer, format, audioData.data(),static_cast<ALsizei>(dataSize), static_cast<ALsizei>(wav->sampleRate));

	// cache buffer in m_audioCache
	auto bufferPtr = new ALuint{ buffer };
	m_audioCache.emplace_back(id, bufferPtr);
}

ALuint* AudioManager::retrieveAudioBuffer(const std::string& id)
{
	// search for the first element of the pair
	const auto it = ranges::find_if(m_audioCache,
	                          [&id](const std::pair<std::string, ALuint*>& p)
	                          {
		                          return p.first == id;
	                          });

	// check if the element was found
	if (it != m_audioCache.end())
	{
		return it->second;
	}

	return nullptr;
}

void AudioManager::setGroupVolume(const AudioGroup group, const float volume)
{
	// if the group is master adjust all groups with it
	if (group == Master)
	{
		m_masterVolume = volume;
		for (auto& pair : m_audioGroups)
		{
			for (ALuint source : pair.second.sources)
			{
				float adjustedVolume = pair.second.volume * m_masterVolume;
				alSourcef(source, AL_GAIN, adjustedVolume);
			}
		}
	}
	else // adjust specific audio group
	{
		m_audioGroups[group].second.volume = volume;
		for (const ALuint source : m_audioGroups[group].second.sources)
		{
			const float adjustedVolume = volume * m_masterVolume;
			alSourcef(source, AL_GAIN, adjustedVolume);
		}
	}
}

float AudioManager::getGroupVolume(AudioGroup group) const
{
	// return either the master volume or group volume based on specified group
	if (group == Master)
		return m_masterVolume;

	return m_audioGroups.at(group).second.volume;
}

void AudioManager::assignGroup(const ALuint source, const AudioGroup group)
{
	// remove the source from any previous group it may have been in
	for (auto& pair : m_audioGroups)
	{
		auto& sources = pair.second.sources;
		std::erase(sources, source);
	}

	// add the source to a group
	m_audioGroups[group].second.sources.push_back(source);

	// set the initial volume for the source based on the group and master volume
	float adjustedVolume = m_audioGroups[group].second.volume * m_masterVolume;
	alSourcef(source, AL_GAIN, adjustedVolume);
}
