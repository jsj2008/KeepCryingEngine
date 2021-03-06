#include "ModuleAudio.h"
#include <algorithm>

#include "AudioListener.h"
#include "AudioClip.h"
#include "AudioSource.h"

const int ModuleAudio::DEVICE = -1; // Default Sounddevice
const int ModuleAudio::FRECUENCY = 44100;

using namespace std;

ModuleAudio::ModuleAudio()
{
}

ModuleAudio::~ModuleAudio()
{}

bool ModuleAudio::Init()
{
	BASS_Init(DEVICE, FRECUENCY, BASS_DEVICE_3D, 0, NULL);
	sceneEffects = new SoundEffects();
	return true;
}

bool ModuleAudio::CleanUp()
{
	return true;
}

void ModuleAudio::EnableListener(AudioListener* listener)
{
	if(activeListener != nullptr)
	{
		activeListener->enabled = false;
	}

	if(listener != nullptr)
	{
		listener->enabled = true;
	}

	activeListener = listener;
}

AudioListener * ModuleAudio::GetActiveListener() const
{
	return activeListener;
}

SoundEffects * ModuleAudio::GetSceneEffects()const
{
	return sceneEffects;
}

void ModuleAudio::SubscribeSource(AudioSource& source)
{
	//If subscribe item is not already in the list, add it
	if(find(sceneSources.begin(),sceneSources.end(),&source) == sceneSources.end())
	{
		sceneSources.push_back(&source);
	}
}

void ModuleAudio::Unsubscribe(AudioSource& source)
{
	vector<AudioSource*>::iterator it = find(sceneSources.begin(), sceneSources.end(), &source);
	if( it != sceneSources.end())
	{
		sceneSources.erase(it);
	}
}

const vector<AudioSource*>& ModuleAudio::GetAllAudioSources() const
{
	return sceneSources;
}

AudioClip * ModuleAudio::Load(const AudioClipIdentifier & identifier)
{
	AudioClip* audioClip = nullptr;

	switch (identifier.audioType)
	{
	case AudioType::Music:
		audioClip = LoadMusic(identifier.path, identifier.channelType);
		break;
	case AudioType::SFX:
		audioClip = LoadSFX(identifier.path, identifier.channelType);
		break;
	}

	return audioClip;
}

void ModuleAudio::Unload(AudioClip * asset)
{
	delete asset;
}

AudioClip * ModuleAudio::LoadMusic(const std::experimental::filesystem::path & path, ChannelType channelType)
{
	AudioClip* audioClip = nullptr;
	HSTREAM handle = 0;
	switch (channelType)
	{
	case ChannelType::Mono:
		handle = BASS_StreamCreateFile(FALSE, path.string().c_str(), 0, 0, BASS_SAMPLE_MONO | BASS_SAMPLE_3D);
		break;
	case ChannelType::Stereo:
		handle = BASS_StreamCreateFile(FALSE, path.string().c_str(), 0, 0, BASS_SAMPLE_3D);
		break;
	}

	if (handle != 0)
	{
		AudioClipIdentifier audioClipIdentifier(path, channelType, AudioType::Music);
		audioClip = new AudioClip(audioClipIdentifier);
		audioClip->musicStream = handle;
	}

	return audioClip;
}

AudioClip * ModuleAudio::LoadSFX(const std::experimental::filesystem::path & path, ChannelType channelType)
{
	AudioClip* audioClip = nullptr;
	HSAMPLE handle = 0;
	switch (channelType)
	{
	case ChannelType::Mono:
		handle = BASS_SampleLoad(FALSE, path.string().c_str(), 0, 0, 5, BASS_SAMPLE_MONO | BASS_SAMPLE_3D | BASS_SAMPLE_OVER_VOL);
		break;
	case ChannelType::Stereo:
		handle = BASS_SampleLoad(FALSE, path.string().c_str(), 0, 0, 5, BASS_SAMPLE_3D | BASS_SAMPLE_OVER_VOL);
		break;
	}
	
	if (handle != 0)
	{
		AudioClipIdentifier audioClipIdentifier(path, channelType, AudioType::SFX);
		audioClip = new AudioClip(audioClipIdentifier);
		audioClip->sfxSample = handle;
	}

	return audioClip;
}
