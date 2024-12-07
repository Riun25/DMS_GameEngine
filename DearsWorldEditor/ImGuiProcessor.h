#pragma once
#include "framework.h"

class ImGuiProcessor
{
public:
	ImGuiProcessor();
	~ImGuiProcessor() = default;

	bool Initialize(HWND _hWnd);
	void Update();
	void Render();
	void Finalize();

	LRESULT ImGui_ImplWin32_WndProcHandler(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam);

private:
	bool CreateDeviceD3D(HWND _hWnd);
	void CleanupDeviceD3D();
	void CreateRenderTarget();
	void CleanupRenderTarget();

private:
	ID3D11Device* m_pd3dDevice;
	ID3D11DeviceContext* m_pd3dDeviceContext;
	IDXGISwapChain* m_pSwapChain;
	UINT m_ResizeWidth;
	UINT m_ResizeHeight;
	ID3D11RenderTargetView* m_mainRenderTargetView;

	ImVec4 clear_color;

	float my_float;
	int my_int;
	bool my_bool;
	char my_text[128];
};

