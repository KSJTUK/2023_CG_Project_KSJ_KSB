#include "pch.h"
#include "Sound.h"

Sound* Sound::m_instance = nullptr;


Sound* Sound::GetInstance(){

	if (m_instance == nullptr) {
		m_instance = new Sound();
	}


	return m_instance;
}

void Sound::NewSound(const std::string& SoundName, const std::string path, FMOD_MODE mode){

	FMOD::Sound* nsound{};

	m_system->createSound(path.c_str(), mode, 0, &nsound);



	m_soundDict.insert(std::make_pair(SoundName,nsound));
}

void Sound::Play(const std::string& SoundName){

	auto iter = m_soundDict.find(SoundName);


	if (iter == m_soundDict.end()) {
		return;
	}




	m_system->playSound(iter->second, nullptr, false, &m_channel);




}

void Sound::Update(){
	m_system->update();
}

void Sound::Init(){

	FMOD::System_Create(&m_system);
	m_system->init(32, FMOD_INIT_NORMAL, NULL);

}



