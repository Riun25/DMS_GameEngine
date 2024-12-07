#include "pch.h"
#include "SoundSystem.h"

class SoundSystem::Impl
{
public:
	Impl()
		: m_pSystem(nullptr)
	{
	}
	~Impl() = default;

	/// <summary>
	/// FMOD System 객체 생성과 초기화, 채널 그룹들 생성과 초기화
	/// </summary>
	/// <returns></returns>
	bool Initialize()
	{
		// System 객체 생성
		FMOD_RESULT fr = FMOD::System_Create(&m_pSystem);
		if (fr != FMOD_OK)
		{
			return false;
		}

		// System 객체 초기화
		fr = m_pSystem->init(64, FMOD_INIT_NORMAL, nullptr);
		if (fr != FMOD_OK)
		{
			return false;
		}

		// 모든 사운드가 최종 믹스되는 마스터 채널 그룹을 가져옵니다.
		fr = m_pSystem->getMasterChannelGroup(&m_pMasterGroup);
		if (fr != FMOD_OK)
		{
			return false;
		}

		// 배경음 채널 그룹을 생성합니다.
		fr = m_pSystem->createChannelGroup("BackgroundMusicGroup", &m_pBGMGroup);
		if (fr != FMOD_OK)
		{
			return false;
		}

		// 효과음 채널 그룹을 생성합니다.
		fr = m_pSystem->createChannelGroup("EffectSoundGroup", &m_pSFXGroup);
		if (fr != FMOD_OK)
		{
			return false;
		}

		// 마스터 채널 그룹에 배경음 채널 그룹과 효과음 채널 그룹을 추가합니다.
		m_pMasterGroup->addGroup(m_pBGMGroup);
		m_pMasterGroup->addGroup(m_pSFXGroup);

		return true;
	}

	/// <summary>
	/// FMOD System 객체 업데이트
	/// </summary>
	void Update()
	{
		if (m_pSystem)
		{
			// 시스템 업데이트
			m_pSystem->update();
		}
	}

	/// <summary>
	/// FMOD 오디오 버퍼 및 시스템 초기화, 할당 해제
	/// </summary>
	void Finalize()
	{
		// 모든 사운드 버퍼 초기화
		if (m_pSystem)
		{
			// 			m_pSystem->update();
						// 사운드 맵 초기화
			for (auto& sound : m_pSounds)
			{
				sound.second->release();
				sound.second = nullptr;
			}
			m_pSounds.clear();

			// 배경음 채널 멥 초기화
			for (auto& channel : m_pBGMChannels)
			{
				channel.second->stop();
				channel.second = nullptr;
			}
			m_pBGMChannels.clear();

			// 효과음 채널 맵 초기화
			for (auto& channel : m_pSFXChannels)
			{
				channel.second->stop();
				channel.second = nullptr;
			}
			m_pSFXChannels.clear();

			// 배경음 채널 그룹 초기화
			if (m_pBGMGroup)
			{
				m_pBGMGroup->release();
				m_pBGMGroup = nullptr;
			}

			// 효과음 채널 그룹 초기화
			if (m_pSFXGroup)
			{
				m_pSFXGroup->release();
				m_pSFXGroup = nullptr;
			}

			// 마스터 채널 그룹 초기화
			if (m_pMasterGroup)
			{
				m_pMasterGroup->release();
				m_pMasterGroup = nullptr;
			}

			// 시스템 초기화 및 할당 해제
// 			m_pSystem->update();

			m_pSystem->release();
			m_pSystem = nullptr;
		}
	}

	bool LoadBGM(const std::string& _filename, const std::string& _soundName)
	{
		if (m_pSystem)
		{
			// 배경음 생성
			if (m_pSounds.find(_soundName) == m_pSounds.end())
			{
				FMOD::Sound* pSound = nullptr;
				FMOD_RESULT fr = m_pSystem->createStream(_filename.c_str(), FMOD_LOOP_NORMAL | FMOD_2D, 0, &pSound);
				if (fr != FMOD_OK)
				{
					return false;
				}
				// 사운드 맵에 생성한 배경음 추가
				m_pSounds[_soundName] = pSound;
				return true;
			}
		}
		return false;
	}

	bool LoadSFX(const std::string& _filename, const std::string& _soundName, bool _loop)
	{
		if (m_pSystem)
		{
			FMOD::Sound* pSound = nullptr;
			// 효과음 생성
			if (_loop)
			{
				FMOD_RESULT fr = m_pSystem->createSound(_filename.c_str(), FMOD_LOOP_NORMAL, 0, &pSound);
				if (fr != FMOD_OK)
				{
					return false;
				}
			}
			else
			{
				FMOD_RESULT fr = m_pSystem->createSound(_filename.c_str(), FMOD_LOOP_OFF, 0, &pSound);
				if (fr != FMOD_OK)
				{
					return false;
				}
			}
			// 사운드 맵에 생성한 효과음 추가
			m_pSounds[_soundName] = pSound;
			return true;
		}
		return false;
	}

	bool Load3DSound(const std::string& _filename, const std::string& _soundName, bool _loop)
	{
		if (m_pSystem)
		{
			FMOD::Sound* pSound = nullptr;
			// 3D 효과음 생성
			if (_loop)
			{
				FMOD_RESULT fr = m_pSystem->createSound(_filename.c_str(), FMOD_3D | FMOD_LOOP_NORMAL, 0, &pSound);
				if (fr != FMOD_OK)
				{
					return false;
				}
			}
			else
			{
				FMOD_RESULT fr = m_pSystem->createSound(_filename.c_str(), FMOD_3D | FMOD_LOOP_OFF, 0, &pSound);
				if (fr != FMOD_OK)
				{
					return false;
				}
			}
			// 사운드 맵에 생성한 3D 효과음 추가
			m_pSounds[_soundName] = pSound;
			return true;
		}
		return false;
	}

	void PlayBGM(const std::string& _soundName)
	{
		if (!IsPlaying(_soundName))
		{
			if (m_pSystem && m_pSounds.find(_soundName) != m_pSounds.end())
			{
				FMOD::Channel* pChannel = nullptr;
				// 인자로 받은 키에 해당하는 배경음 반복 재생
				m_pSystem->playSound(m_pSounds[_soundName], m_pBGMGroup, false, &pChannel);
				if (pChannel)
				{
					// 채널 맵에 현재 재생중인 채널 추가
					m_pBGMChannels[_soundName] = pChannel;
					// 현재 채널을 재생중인 채널로 설정
					m_pCurrentBGMChannel = pChannel;
				}
			}
		}
	}

	void PauseBGM()
	{
		if (m_pCurrentBGMChannel)
		{
			bool isPaused;
			// 현재 채널의 배경음이 재생중인지 검사
			m_pCurrentBGMChannel->getPaused(&isPaused);
			// 재생중이면 일시정지하고, 일시정지 상태라면 다시 재생
			m_pCurrentBGMChannel->setPaused(!isPaused);
		}
	}

	void StopBGM()
	{
		if (m_pCurrentBGMChannel)
		{
			// 현재 채널의 배경음을 정지
			m_pCurrentBGMChannel->stop();
			m_pCurrentBGMChannel = nullptr;
		}
	}

	void FadeInBGM(const std::string& _soundName, float _fadeTime)
	{
		PlayBGM(_soundName);
		if (m_pCurrentBGMChannel)
		{
			float targetVolume = 1.0f;
			float volumeStep = targetVolume / (_fadeTime * 60); // 가정하는 FPS에 따라 조정

			for (int i = 0; i < _fadeTime * 60; ++i)
			{
				float currentVolume;
				m_pCurrentBGMChannel->getVolume(&currentVolume);
				currentVolume += volumeStep;
				m_pCurrentBGMChannel->setVolume(currentVolume);
			}
		}
	}

	void FadeOutBGM(float _fadeTime)
	{
		float currentVolume;
		m_pCurrentBGMChannel->getVolume(&currentVolume);
		float step = currentVolume / (_fadeTime * 60); // 페이드 아웃을 위한 한 프레임당 감소량 계산

		// 매 프레임 볼륨을 조금씩 줄이기
		for (int i = 0; i < _fadeTime * 60; ++i)
		{
			currentVolume -= step;
			if (currentVolume <= 0)
			{
				break;
			}
			m_pCurrentBGMChannel->setVolume(currentVolume);
		}

		m_pCurrentBGMChannel->setVolume(0); // 마지막에 볼륨을 0으로 설정
		StopBGM();
	}

	void PlaySFX(const std::string& _soundName)
	{
		// 사운드가 이미 재생 중인지 확인 (최대 인스턴스 수를 40로 설정)
		const int MAX_SFX_INSTANCES = 40;
		int currentInstances = 0;

		if (m_pSystem && m_pSounds.find(_soundName) != m_pSounds.end())
		{
			for (auto& channel : m_pSFXChannels)
			{
				bool isPlaying = false;
				if (channel.second && channel.second->isPlaying(&isPlaying) == FMOD_OK && isPlaying)
				{
					currentInstances++;
				}
			}

			// 현재 재생 중인 인스턴스 수가 최대값을 넘지 않으면 재생
			if (currentInstances < MAX_SFX_INSTANCES)
			{
				FMOD::Channel* pChannel = nullptr;
				m_pSystem->playSound(m_pSounds[_soundName], m_pSFXGroup, false, &pChannel);
				if (pChannel)
				{
					m_pSFXChannels[_soundName] = pChannel;
				}
			}
		}
	}

	void StopSFX(const std::string& _soundName)
	{
		// 지정된 사운드의 모든 인스턴스를 정지
		if (m_pSFXChannels.find(_soundName) != m_pSFXChannels.end())
		{
			FMOD::Channel* pChannel = m_pSFXChannels[_soundName];
			if (pChannel)
			{
				pChannel->stop(); // 사운드 정지
				m_pSFXChannels.erase(_soundName); // 맵에서 채널 제거
			}
		}
	}

	void Play3DSound(const std::string& _soundName, FMOD_VECTOR _position)
	{
		if (m_pSystem && m_pSounds.find(_soundName) != m_pSounds.end())
		{
			FMOD::Channel* pChannel = nullptr;
			m_pSystem->playSound(m_pSounds[_soundName], 0, false, &pChannel);
			if (pChannel)
			{
				FMOD_VECTOR velocity = { 0.0f, 0.0f, 0.0f };
				pChannel->set3DAttributes(&_position, &velocity);
				pChannel->setPaused(false);
				m_pSFXChannels[_soundName] = pChannel;
			}
		}
	}

	void SetListenerPosition(FMOD_VECTOR _position)
	{
		if (m_pSystem)
		{
			FMOD_VECTOR velocity = { 0.0f, 0.0f, 0.0f };
			FMOD_VECTOR forward = { 0.0f, 0.0f, 1.0f };
			FMOD_VECTOR up = { 0.0f, 1.0f, 0.0f };
			m_pSystem->set3DListenerAttributes(0, &_position, &velocity, &forward, &up);
		}
	}

	void SetBGMVolume(float _volume)
	{
		_volume = std::clamp(_volume, 0.f, 1.f);
		m_originVolume = _volume;
		if (m_pBGMGroup)
		{
			m_pBGMGroup->setVolume(_volume);
		}
	}

	void SetSFXVolume(float _volume)
	{
		_volume = std::clamp(_volume, 0.f, 1.f);
		if (m_pSFXGroup)
		{
			m_pSFXGroup->setVolume(_volume);
		}
	}

	void SetMasterVolume(float _volume)
	{
		_volume = std::clamp(_volume, 0.f, 1.f);
		if (m_pMasterGroup)
		{
			m_pMasterGroup->setVolume(_volume);
		}
	}

	bool IsPlaying(const std::string& _soundName)
	{
		bool isPlaying = false;

		// 배경음 채널에서 확인
		if (m_pBGMChannels.find(_soundName) != m_pBGMChannels.end())
		{
			FMOD::Channel* pChannel = m_pBGMChannels[_soundName];
			isPlaying = false;
			if (pChannel && pChannel->isPlaying(&isPlaying) == FMOD_OK)
			{
				return isPlaying;
			}
		}

		// 효과음 채널에서 확인
		if (m_pSFXChannels.find(_soundName) != m_pSFXChannels.end())
		{
			FMOD::Channel* pChannel = m_pSFXChannels[_soundName];
			isPlaying = false;
			if (pChannel && pChannel->isPlaying(&isPlaying) == FMOD_OK)
			{
				return isPlaying;
			}
		}

		// 재생 중인 사운드가 없을 경우 false 반환
		return isPlaying;
	}

	std::string GetFMODVersion()
	{
		uint32_t version = FMOD_VERSION;

		// 버전 정보 출력
		uint32_t major = (version >> 16) & 0xFFFF;
		uint32_t minor = (version >> 8) & 0xFF;
		uint32_t patch = version & 0xFF;
		std::string strVersion = std::to_string(major) + "." + std::to_string(minor) + "." + std::to_string(patch);
		return strVersion;
	}

private:
	FMOD::System* m_pSystem;	// FMOD 시스템 객체
	FMOD::ChannelGroup* m_pMasterGroup;	// 모든 채널의 포함하는 채널 그룹
	FMOD::ChannelGroup* m_pBGMGroup;	// 배경음 채널들의 그룹
	FMOD::ChannelGroup* m_pSFXGroup;	// 효과음 채널들의 그룹
	FMOD::Channel* m_pCurrentBGMChannel;	// 현재 재생중인 배경음 채널

	// 시스템에 등록되어있는 사운드 객체들 <사운드의 문자열 키, 사운드 객체의 포인터>
	std::unordered_map<std::string, FMOD::Sound*> m_pSounds;

	// 시스템에 등록되어있는 사운드 객체들 <채널의 문자열 키, 채널 객체의 포인터>
	std::unordered_map<std::string, FMOD::Channel*> m_pBGMChannels;	// 배경음 채널 객체들

	// 시스템에 등록되어있는 사운드 객체들 <채널의 문자열 키, 채널 객체의 포인터>
	std::unordered_map<std::string, FMOD::Channel*> m_pSFXChannels;	// 효과음 채널 객체들

	float m_originVolume = 1.f;
};



SoundSystem::SoundSystem()
	:m_pImpl(std::make_unique<Impl>())
{
}

SoundSystem::~SoundSystem() = default;

bool SoundSystem::Initialize()
{
	return m_pImpl->Initialize();
}

void SoundSystem::Update()
{
	m_pImpl->Update();
}

void SoundSystem::Finalize()
{
	m_pImpl->Finalize();
}

bool SoundSystem::LoadBGM(const std::string& _filename, const std::string& _soundName)
{
	return m_pImpl->LoadBGM(_filename, _soundName);
}

void SoundSystem::PlayBGM(const std::string& _soundName)
{
	m_pImpl->PlayBGM(_soundName);
}

void SoundSystem::PauseBGM()
{
	m_pImpl->PauseBGM();
}

void SoundSystem::StopBGM()
{
	m_pImpl->StopBGM();
}

void SoundSystem::FadeInBGM(const std::string& _soundName, float _fadeTime)
{
	m_pImpl->FadeInBGM(_soundName, _fadeTime);
}

void SoundSystem::FadeOutBGM(float _fadeTime)
{
	m_pImpl->FadeOutBGM(_fadeTime);
}

bool SoundSystem::LoadSFX(const std::string& _filename, const std::string& _soundName, bool _loop)
{
	return m_pImpl->LoadSFX(_filename, _soundName, _loop);
}

bool SoundSystem::Load3DSound(const std::string& _filename, const std::string& _soundName, bool _loop)
{
	return m_pImpl->Load3DSound(_filename, _soundName, _loop);
}

void SoundSystem::PlaySFX(const std::string& _soundName)
{
	m_pImpl->PlaySFX(_soundName);
}

void SoundSystem::StopSFX(const std::string& _soundName)
{
	m_pImpl->StopSFX(_soundName);
}

void SoundSystem::Play3DSound(const std::string& _soundName, float _x, float _y, float _z)
{
	m_pImpl->Play3DSound(_soundName, FMOD_VECTOR(_x, _y, _z));
}

void SoundSystem::SetListenerPosition(float _x, float _y, float _z)
{
	m_pImpl->SetListenerPosition(FMOD_VECTOR(_x, _y, _z));
}

void SoundSystem::SetBGMVolume(float _volume)
{
	m_pImpl->SetBGMVolume(_volume);
}

void SoundSystem::SetSFXVolume(float _volume)
{
	m_pImpl->SetSFXVolume(_volume);
}

void SoundSystem::SetMasterVolume(float _volume)
{
	m_pImpl->SetMasterVolume(_volume);
}

bool SoundSystem::IsPlaying(const std::string& _soundName)
{
	return m_pImpl->IsPlaying(_soundName);
}

std::string SoundSystem::GetFMODVersion()
{
	return m_pImpl->GetFMODVersion();
}
