#pragma once
#include "..\DearsGameEngine\BaseWindowsProcessor.h"

class TimeManager;
class GameEngine;
class World;
class Scene;

class DemoProcessor :
    public BaseWindowsProcessor
{
public:
    DemoProcessor(HINSTANCE _hInstance, const WCHAR* _szTitle, int _iconResourceId);
    ~DemoProcessor() = default;

    bool Initialize() override;
    void Run();
    void Finalize() override;


private:
	LRESULT CALLBACK WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam);
    void Update() override;

public:


private:
    GameEngine* m_pGameEngine;

    std::shared_ptr<World> m_pWorld;
    std::shared_ptr<Scene> m_pScene1;
	std::shared_ptr<Scene> m_pScene2;

};