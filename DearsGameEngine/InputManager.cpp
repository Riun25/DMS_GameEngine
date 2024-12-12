#include "InputManager.h"
#include "InputDevice.h"

// 매니저 초기화 클래스입니다. 여러 디바이스들을 추가합니다.
bool InputManager::Initialize()
{
	std::shared_ptr<IInputDevice> keyboard = std::make_shared<Keyboard>();
	std::shared_ptr<IInputDevice> mouse = std::make_shared<Mouse>();

	AddDevice(keyboard);
	AddDevice(mouse);

	return true;
}

// 업데이트 함수입니다. 관리중인 디바이스들을 업데이트합니다.
void InputManager::Update(float _dTime)
{
	for (auto& device : mpDevices)
	{
		device->Update();
	}

	// LBUTTON 키 상태 확인
	//KEY_STATE state = GetKeyState(KEY::LBUTTON);

	//// LBUTTON 키 상태를 디버그 창에 출력
	//switch (state)
	//{
	//case KEY_STATE::NONE:
	//	DLOG(LOG_INFO, "NONE");
	//	break;
	//case KEY_STATE::AWAY:
	//	DLOG(LOG_INFO, "AWA");
	//	break;
	//case KEY_STATE::TAP:
	//	DLOG(LOG_INFO, "TAP");
	//	break;
	//case KEY_STATE::HOLD:
	//	DLOG(LOG_INFO, "HOLD");
	//	break;
	//}
}

// 관리중인 디바이스들의 메모리 할당을 해제합니다.
void InputManager::Finalize()
{
}

/// <summary>
/// 특정 키의 상태를 반환합니다.
/// </summary>
/// <param name="_key">상태를 반환할 키</param>
/// <returns>인자로 받은 키의 상태</returns>
KEY_STATE InputManager::GetKeyState(KEY _key)
{
	KEY_STATE keyState;
	for (const auto& device : mpDevices)
	{
		auto info = device->GetKeyInfo(_key);
		keyState = info.keyState;
	}
	return keyState;
}

bool InputManager::GetKey(KEY _key)
{
	return GetKeyState(_key) == KEY_STATE::HOLD;
}

bool InputManager::GetKeyDown(KEY _key)
{
	return GetKeyState(_key) == KEY_STATE::TAP;
}

bool InputManager::GetKeyUp(KEY _key)
{
	return GetKeyState(_key) == KEY_STATE::AWAY;
}

// 마우스의 위치를 반환합니다.
Vector2 InputManager::GetMousePos() const
{
	for (const auto& device : mpDevices)
	{
		// 관리중인 디바이스에 마우스가 있을 경우
		if (auto mouse = std::dynamic_pointer_cast<Mouse>(device))
		{
			return mouse->GetMousePos();
		}
	}
	DLOG(LOG_WARN, "There is no mouse device in process.");
	return Vector2();
}

// 이전 프레임과 현재 프레임의 마우스의 위치 차이를 반환합니다.
Vector2 InputManager::GetDeltaMousePos() const
{
	for (const auto& device : mpDevices)
	{
		// 관리중인 디바이스에 마우스가 있을 경우
		if (auto mouse = std::dynamic_pointer_cast<Mouse>(device))
		{
			return mouse->GetDeltaMousePos();
		}
	}
	DLOG(LOG_WARN, "There is no mouse device in process.");
	return Vector2();
}

void InputManager::AddMouseCursor(const std::string& _file)
{
	std::wstring wFile = std::wstring(_file.begin(), _file.end());
	HCURSOR cursor = (HCURSOR)LoadImage(NULL, wFile.c_str(), IMAGE_CURSOR, 32, 32, LR_LOADFROMFILE);
	mCursors.push_back(cursor);
}

void InputManager::SetMouseCursor(int _cursorName)
{
	mCurrentCursor = mCursors[_cursorName];
}

void InputManager::SetMouseCursor()
{
	SetCursor(mCurrentCursor);
}

HCURSOR InputManager::GetCurrentCursor() const
{
	return mCurrentCursor;
}
