#include "framework.h"
#include "SoundSystem.h"

/// <summary>
/// DLL로 만든 사운드 시스템 모듈을 테스트하는 코드입니다.
/// </summary>

extern "C" SoundSystem * GetSoundSystem();

int main()
{
	// 	SoundSystem* pSoundSystem = GetSoundSystem();
	// 	if (!pSoundSystem)
	// 	{
	// 		std::cerr << "Failed to get sound system instance." << std::endl;
	// 	}
	// 
	// 	if (!pSoundSystem->LoadBGM("../../Resources/Sound/Seven Nation Army.mp3", "RB"))
	// 	{
	// 		std::cerr << "Failed to load Sound." << std::endl;
	// 	}
	// 
	// 	pSoundSystem->PlayBGM("RB");
	// 	pSoundSystem->SetBGMVolume(0.5f);
	// 
	// 	while (true)
	// 	{
	// 		pSoundSystem->Update();
	// 	}


	std::cout << "main: Getting SoundSystem instance from DLL." << std::endl;
	SoundSystem* soundSystem = GetSoundSystem();

	if (!soundSystem) {
		std::cerr << "main: Failed to get SoundSystem instance." << std::endl;
		return -1;
	}

	std::cout << "main: SoundSystem instance obtained successfully." << std::endl;

	// 배경음악 로드
	if (!soundSystem->LoadBGM("../../Resources/sound/Seven nation army.mp3", "BGM")) {
		std::cerr << "main: Failed to load background music." << std::endl;
		return -1;
	}

	if (!soundSystem->LoadBGM("../../Resources/sound/rather be.mp3", "BGM2")) {
		std::cerr << "main: Failed to load background music." << std::endl;
		return -1;
	}

	std::cout << "main: Background music loaded successfully." << std::endl;

	/*
	// 사운드 이펙트 로드
	if (!soundSystem->LoadSFX("path/to/sfx.wav", "SFX")) {
		std::cerr << "main: Failed to load sound effect." << std::endl;
		return -1;
	}

	std::cout << "main: Sound effect loaded successfully." << std::endl;
	*/

	// 배경음악 재생
// 	soundSystem->PlayBGM("BGM");
// 	std::cout << "main: Background music playing." << std::endl;

	// 볼륨 설정
	soundSystem->SetBGMVolume(1.f);
	std::cout << "main: Background music volume set to 0.5." << std::endl;

	/*
	// 사운드 이펙트 재생
	soundSystem->PlaySFX("SFX");
	std::cout << "main: Sound effect playing." << std::endl;
	*/

	// 배경음악 페이드 인
	soundSystem->FadeInBGM("BGM", 5.0f);
	std::cout << "main: Background music fade-in started." << std::endl;

	// 잠시 대기
	std::this_thread::sleep_for(std::chrono::seconds(10));
	std::cout << "main: Waited for 10 seconds." << std::endl;

	// 배경음악 페이드 아웃
	soundSystem->FadeOutBGM(5.0f);
	std::cout << "main: Background music fade-out started." << std::endl;

	// 배경음악 페이드 인
	soundSystem->FadeInBGM("BGM2", 5.f);
	std::cout << "main: Background music fade-in started." << std::endl;

	// 잠시 대기
	std::this_thread::sleep_for(std::chrono::seconds(10));
	std::cout << "main: Waited for 10 seconds." << std::endl;

	// 배경음악 페이드 아웃
	soundSystem->FadeOutBGM(5.0f);
	std::cout << "main: Background music fade-out started." << std::endl;


	/*
 	while (true)
	{
		// 시스템 업데이트
		soundSystem->Update();
		std::cout << "main: Sound system updated." << std::endl;
	}
	*/

	// 종료 전 대기
	std::cout << "Press Enter to exit...";
	std::cin.get();

	return 0;
}