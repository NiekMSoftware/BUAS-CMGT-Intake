#include "precomp.h"
#include "AudioManager.h"

AudioManager::~AudioManager()
{
	if (initialized)
		shutdown();
}

AudioManager& AudioManager::instance()
{
	static AudioManager s;
	return s;
}

bool AudioManager::initialize()
{
	if (initialized)
	{
		std::cerr << "AudioManager::initialize() - Already initialized.\n";
		return false;
	}

	// init engine
	if (ma_engine_init(nullptr, &engine) != MA_SUCCESS)
	{
		std::cerr << "Failed to initialize audio engine.\n";
		return false;
	}

	initialized = true;
	OutputDebugString("[LOG] AudioManager initialized successfully.\n");
	return true;
}

void AudioManager::shutdown()
{
	ma_engine_uninit(&engine);
	initialized = false;
	OutputDebugString("[LOG] AudioManager has been shutdown.\n");
}
