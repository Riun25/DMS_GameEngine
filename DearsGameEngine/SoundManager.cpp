#include "SoundManager.h"
#include "../DearsSound/SoundSystem.h"
#include "Event.h"
#include <random>

extern "C" SoundSystem * GetSoundSystem();

// SoundManager::SoundManager(/*EventManager* _pEventManager*/)
// 	: m_pSoundSystem(nullptr)/*, EventListener(_pEventManager)*/
// {
// 
// }

bool SoundManager::Initialize()
{
	m_pSoundSystem = GetSoundSystem();

	DLOG(LOG_INFO, m_pSoundSystem->GetFMODVersion());

	// 	REGISTER_IMMEDIATE_EVENT("PlayBGM", PlayBGM);
	// 	REGISTER_IMMEDIATE_EVENT("PauseBGM", PauseBGM);
	// 	REGISTER_IMMEDIATE_EVENT("StopBGM", StopBGM);
	// 	REGISTER_IMMEDIATE_EVENT("FadeInBGM", FadeInBGM);
	// 	REGISTER_IMMEDIATE_EVENT("FadeOutBGM", FadeOutBGM);
	// 	REGISTER_IMMEDIATE_EVENT("PlaySFX", PlaySFX);
	// 	REGISTER_IMMEDIATE_EVENT("Play3DSound", Play3DSound);
	// 	REGISTER_IMMEDIATE_EVENT("SetListenerPosition", SetListenerPosition);
	// 	REGISTER_IMMEDIATE_EVENT("SetBGMVolume", SetBGMVolume);
	// 	REGISTER_IMMEDIATE_EVENT("SetSFXVolume", SetSFXVolume);
	// 	REGISTER_IMMEDIATE_EVENT("SetMasterVolume", SetMasterVolume);

	return m_pSoundSystem->Initialize();
}

void SoundManager::Update(float _dTime)
{
	m_pSoundSystem->Update();
}

void SoundManager::Finalize()
{
	// 	UNREGISTER_EVENT("PlayBGM");
	// 	UNREGISTER_EVENT("PauseBGM");
	// 	UNREGISTER_EVENT("StopBGM");
	// 	UNREGISTER_EVENT("FadeInBGM");
	// 	UNREGISTER_EVENT("FadeOutBGM");
	// 	UNREGISTER_EVENT("PlaySFX");
	// 	UNREGISTER_EVENT("Play3DSound");
	// 	UNREGISTER_EVENT("SetListenerPosition");
	// 	UNREGISTER_EVENT("SetBGMVolume");
	// 	UNREGISTER_EVENT("SetSFXVolume");
	// 	UNREGISTER_EVENT("SetMasterVolume");

	m_pSoundSystem->Finalize();
}

bool SoundManager::LoadBGM(const std::string& _filename, const std::string& _soundName)
{
	return m_pSoundSystem->LoadBGM(_filename, _soundName);
}

bool SoundManager::LoadSFX(const std::string& _filename, const std::string& _soundName, bool _loop)
{
	return m_pSoundSystem->LoadSFX(_filename, _soundName, _loop);
}

bool SoundManager::Load3DSound(const std::string& _filename, const std::string& _soundName, bool _loop)
{
	return m_pSoundSystem->Load3DSound(_filename, _soundName, _loop);
}

void SoundManager::PlayBGM(const std::string& _soundName)
{
	m_pSoundSystem->PlayBGM(_soundName);
}

void SoundManager::PauseBGM()
{
	m_pSoundSystem->PauseBGM();
}

void SoundManager::StopBGM()
{
	m_pSoundSystem->StopBGM();
}

void SoundManager::FadeInBGM(const std::string& _soundName, float _fadeTime)
{
	m_pSoundSystem->FadeInBGM(_soundName, _fadeTime);
}

void SoundManager::FadeOutBGM(float _fadeTime)
{
	m_pSoundSystem->FadeOutBGM(_fadeTime);
}

void SoundManager::PlaySFX(const std::string& _soundName, float _probability)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dist(0.f, 1.f);
	if (dist(gen) <= _probability)
	{
		m_pSoundSystem->PlaySFX(_soundName);
	}
}

void SoundManager::StopSFX(const std::string& _soundName)
{
	m_pSoundSystem->StopSFX(_soundName);
}

void SoundManager::Play3DSound(const std::string& _soundName, float _x, float _y, float _z)
{
	m_pSoundSystem->Play3DSound(_soundName, _x, _y, _z);
}

void SoundManager::SetListenerPosition(float _x, float _y, float _z)
{
	m_pSoundSystem->SetListenerPosition(_x, _y, _z);
}

void SoundManager::SetBGMVolume(float _volume)
{
	m_pSoundSystem->SetBGMVolume(_volume);
}

void SoundManager::SetSFXVolume(float _volume)
{
	m_pSoundSystem->SetSFXVolume(_volume);
}

void SoundManager::SetMasterVolume(float _volume)
{
	m_pSoundSystem->SetMasterVolume(_volume);
}

bool SoundManager::IsPlaying(const std::string& _soundName)
{
	return m_pSoundSystem->IsPlaying(_soundName);
}

/// 이벤트 트리거 함수들

// void SoundManager::PlayBGM(const Event& _event)
// {
// 	auto data = _event.GetDataAs<std::string>().value();
// 	PlayBGM(data);
// }
// void SoundManager::PauseBGM(const Event& _event)
// {
// 	PauseBGM();
// }
// void SoundManager::StopBGM(const Event& _event)
// {
// 	StopBGM();
// }
// void SoundManager::FadeInBGM(const Event& _event)
// {
// 	std::tuple<std::string, float> data = _event.GetDataAs<std::tuple<std::string, float>>().value();
// 	FadeInBGM(std::get<0>(data), std::get<1>(data));
// }
// void SoundManager::FadeOutBGM(const Event& _event)
// {
// 	auto data = _event.GetDataAs<float>().value();
// 	FadeOutBGM(data);
// }
// void SoundManager::PlaySFX(const Event& _event)
// {
// 	auto data = _event.GetDataAs<std::string>().value();
// 	PlaySFX(data);
// }
// void SoundManager::Play3DSound(const Event& _event)
// {
// 	auto data = _event.GetDataAs<std::string>().value();
// 	Play3DSound(data);
// }
// void SoundManager::SetListenerPosition(const Event& _event)
// {
// 	auto data = _event.GetDataAs<Vector3>().value();
// 	SetListenerPosition(data.x, data.y, data.z);
// }
// void SoundManager::SetBGMVolume(const Event& _event)
// {
// 	auto data = _event.GetDataAs<float>().value();
// 	SetBGMVolume(data);
// }
// void SoundManager::SetSFXVolume(const Event& _event)
// {
// 	auto data = _event.GetDataAs<float>().value();
// 	SetSFXVolume(data);
// }
// void SoundManager::SetMasterVolume(const Event& _event)
// {
// 	auto data = _event.GetDataAs<float>().value();
// 	SetMasterVolume(data);
// }
