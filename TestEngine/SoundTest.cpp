#include "framework.h"
#include "SoundSystem.h"

/// <summary>
/// DLL�� ���� ���� �ý��� ����� �׽�Ʈ�ϴ� �ڵ��Դϴ�.
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

	// ������� �ε�
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
	// ���� ����Ʈ �ε�
	if (!soundSystem->LoadSFX("path/to/sfx.wav", "SFX")) {
		std::cerr << "main: Failed to load sound effect." << std::endl;
		return -1;
	}

	std::cout << "main: Sound effect loaded successfully." << std::endl;
	*/

	// ������� ���
// 	soundSystem->PlayBGM("BGM");
// 	std::cout << "main: Background music playing." << std::endl;

	// ���� ����
	soundSystem->SetBGMVolume(1.f);
	std::cout << "main: Background music volume set to 0.5." << std::endl;

	/*
	// ���� ����Ʈ ���
	soundSystem->PlaySFX("SFX");
	std::cout << "main: Sound effect playing." << std::endl;
	*/

	// ������� ���̵� ��
	soundSystem->FadeInBGM("BGM", 5.0f);
	std::cout << "main: Background music fade-in started." << std::endl;

	// ��� ���
	std::this_thread::sleep_for(std::chrono::seconds(10));
	std::cout << "main: Waited for 10 seconds." << std::endl;

	// ������� ���̵� �ƿ�
	soundSystem->FadeOutBGM(5.0f);
	std::cout << "main: Background music fade-out started." << std::endl;

	// ������� ���̵� ��
	soundSystem->FadeInBGM("BGM2", 5.f);
	std::cout << "main: Background music fade-in started." << std::endl;

	// ��� ���
	std::this_thread::sleep_for(std::chrono::seconds(10));
	std::cout << "main: Waited for 10 seconds." << std::endl;

	// ������� ���̵� �ƿ�
	soundSystem->FadeOutBGM(5.0f);
	std::cout << "main: Background music fade-out started." << std::endl;


	/*
 	while (true)
	{
		// �ý��� ������Ʈ
		soundSystem->Update();
		std::cout << "main: Sound system updated." << std::endl;
	}
	*/

	// ���� �� ���
	std::cout << "Press Enter to exit...";
	std::cin.get();

	return 0;
}