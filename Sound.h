#pragma once

#define SOUND_MAX_CHANNEL 10

class Sound{
private:
	Sound();
	~Sound();

public:
	static Sound* GetInstance();

	void Init();

	void NewSound(const std::string& SoundName, const std::string path,FMOD_MODE mode);
	void Play(const std::string& SoundName, int channel);

	void Update();

private:
	static Sound* m_instance;

private:
	FMOD::System* m_system;
	FMOD::Channel* m_channel[SOUND_MAX_CHANNEL];
	std::unordered_map<std::string, FMOD::Sound*> m_soundDict{};
};

