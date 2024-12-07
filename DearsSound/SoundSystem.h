#pragma once
#include "framework.h"

#ifdef SOUNDSYSTEM_EXPORTS
#define SOUNDSYSTEM_API __declspec(dllexport)
#else
#define SOUNDSYSTEM_API __declspec(dllimport)
#endif

/// <summary>
/// 사운드 시스템 클래스입니다.
/// 캡슐화를 위해 Impl 객체에 내부 로직을 숨겼습니다.
/// dll main에서 하나의 프로세스에서 하나만 생성해서 사용합니다.
/// 배경음, 효과음, 3D효과음, 음량 조절 기능등을 지원합니다.
/// 2024.05.21 _Doyo;
/// </summary>
class SOUNDSYSTEM_API SoundSystem
{
public:
	SoundSystem();
	~SoundSystem();

	/// <summary>
	/// SoundSystem 초기화
	/// </summary>
	/// <returns></returns>
	bool Initialize();

	/// <summary>
	/// SoundSystem 업데이트
	/// 오디오 버퍼, 3D 사운드 처리, 효과 및 DSP 처리, 채널 관리, 콜백 처리 등의 역할을 합니다.
	/// </summary>
	void Update();

	/// <summary>
	/// SoundSystem의 리소스들을 정리합니다.
	/// </summary>
	void Finalize();

	/// <summary>
	/// 배경음으로 사용할 사운드를 오디오 버퍼에 저장합니다.
	/// </summary>
	/// <param name="_filename">파일명</param>
	/// <param name="_soundName">문자열 키</param>
	/// <returns></returns>
	bool LoadBGM(const std::string& _filename, const std::string& _soundName);

	/// <summary>
	/// 효과음으로 사용할 사운드를 오디오 버퍼에 저장합니다.
	/// </summary>
	/// <param name="_filename">파일명</param>
	/// <param name="_soundName">문자열 키</param>
	/// <returns></returns>
	bool LoadSFX(const std::string& _filename, const std::string& _soundName, bool _loop);

	/// <summary>
	/// 3D 효과음으로 사용할 사운드를 오디오 버퍼에 저장합니다.
	/// </summary>
	/// <param name="_filename">파일명</param>
	/// <param name="_soundName">문자열 키</param>
	/// <returns></returns>
	bool Load3DSound(const std::string& _filename, const std::string& _soundName, bool _loop);

	/// <summary>
	/// 배경음을 반복재생합니다.
	/// </summary>
	/// <param name="_soundName">문자열 키</param>
	void PlayBGM(const std::string& _soundName);

	/// <summary>
	/// 현재 재생중인 배경음을 일시 정지합니다.
	/// </summary>
	void PauseBGM();

	/// <summary>
	/// 현재 재생중인 배경음을 정지합니다.
	/// </summary>
	void StopBGM();

	/// <summary>
	/// 배경음을 페이드 인 합니다.
	/// </summary>
	/// <param name="_soundName">문자열 키</param>
	/// <param name="_fadeTime">페이드 인 되는 시간</param>
	void FadeInBGM(const std::string& _soundName, float _fadeTime);

	/// <summary>
	/// 현재 재생중인 배경음을 페이드 아웃 합니다.
	/// </summary>
	/// <param name="_fadeTime">페이드 인 되는 시간</param>
	void FadeOutBGM(float _fadeTime);

	/// <summary>
	/// 효과음을 재생합니다.
	/// </summary>
	/// <param name="_soundName">문자열 키</param>
	void PlaySFX(const std::string& _soundName);

	/// <summary>
	/// 효과음을 정지합니다.
	/// </summary>
	/// <param name="_soundName"></param>
	void StopSFX(const std::string& _soundName);

	/// <summary>
	/// 3D 효과음을 재생합니다.
	/// </summary>
	/// <param name="_soundName">문자열 키</param>
	/// <param name="_x">효과음의 x좌표</param>
	/// <param name="_y">효과음의 y좌표</param>
	/// <param name="_z">효과음의 z좌표</param>
	void Play3DSound(const std::string& _soundName, float _x, float _y, float _z);

	/// <summary>
	/// 3D 효과음을 설정하기 위해 리스너의 위치를 설정합니다.
	/// </summary>
	/// <param name="_x">리스너의 x좌표</param>
	/// <param name="_y">리스너의 y좌표</param>
	/// <param name="_z">리스너의 z좌표</param>
	void SetListenerPosition(float _x, float _y, float _z);

	/// <summary>
	/// 배경음의 볼륨을 설정합니다.
	/// </summary>
	/// <param name="_volume">볼륨크기 (0.0 ~ 1.0)</param>
	void SetBGMVolume(float _volume);

	/// <summary>
	/// 효과음의 볼륨을 설정합니다.
	/// </summary>
	/// <param name="_volume">볼륨크기 (0.0 ~ 1.0)</param>
	void SetSFXVolume(float _volume);

	/// <summary>
	/// 마스터 볼륨을 설정합니다.
	/// </summary>
	/// <param name="_volume">볼륨크기 (0.0 ~ 1.0)</param>
	void SetMasterVolume(float _volume);

	/// <summary>
	/// 특정 사운드가 재생중인지 확인합니다.
	/// </summary>
	/// <param name="_soundName">확인할 사운드 이름</param>
	/// <returns>재생중이면 true 반환</returns>
	bool IsPlaying(const std::string& _soundName);

	/// <summary>
	/// 현재 사용중인 FMOD 라이브러리의 버전을 가져옵니다.
	/// </summary>
	/// <returns>FMOD의 버전 문자열</returns>
	std::string GetFMODVersion();

private:
	class Impl;
	std::unique_ptr<Impl> m_pImpl;	// 사운드 시스템 캡슐화를 위한 Impl 객체
};

extern "C"
{
	SOUNDSYSTEM_API SoundSystem* GetSoundSystem();
	SOUNDSYSTEM_API void ReleaseSoundSystem();
}