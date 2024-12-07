#include "DearsWorldEditor.h"
#include "ImGuiProcessor.h"

EditorProcessor::EditorProcessor(HINSTANCE _hInstance)
	:BaseWindowsProcessor(_hInstance)
{
}

bool EditorProcessor::Initialize()
{
	if (!__super::Initialize())
	{
		return false;
	}

	m_pImGuiInitializer = new ImGuiProcessor();
	if (!m_pImGuiInitializer->Initialize(m_hWnd))
	{
		return false;
	}

	return true;
}

void EditorProcessor::Run()
{
	while (true)
	{
		if (PeekMessage(&m_msg, NULL, 0, 0, PM_REMOVE))
		{
			if (m_msg.message == WM_QUIT)
			{
				break;
			}

			if (!TranslateAccelerator(m_msg.hwnd, m_hAccelTable, &m_msg))
			{
				TranslateMessage(&m_msg);
				DispatchMessage(&m_msg);
			}
		}
		else
		{
			Update();
			Render();
		}
	}
}

void EditorProcessor::Finalize()
{
	m_pImGuiInitializer->Finalize();
	__super::Finalize();
}

LRESULT CALLBACK EditorProcessor::WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam)
{
	if (m_pImGuiInitializer->ImGui_ImplWin32_WndProcHandler(_hWnd, _message, _wParam, _lParam))
	{
		return true;
	}

	return __super::WndProc(_hWnd, _message, _wParam, _lParam);
}

void EditorProcessor::Update()
{
	__super::Update();
	m_pImGuiInitializer->Update();
}

void EditorProcessor::Render()
{
	__super::Render();
	m_pImGuiInitializer->Render();
}
