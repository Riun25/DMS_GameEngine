#pragma once
#include "resource.h"
#include "..\DearsGameEngine\BaseWindowsProcessor.h"

class TimeManager;
class GameEngine;
class ImGuiProcessor;

class EditorProcessor :
	public BaseWindowsProcessor
{
public:
	EditorProcessor(HINSTANCE _hInstance);
	~EditorProcessor() = default;

	bool Initialize() override;
	void Run();
	void Finalize() override;


private:
	LRESULT CALLBACK WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam);
	void Update() override;
	void Render() override;


private:
	ImGuiProcessor* m_pImGuiInitializer;

};