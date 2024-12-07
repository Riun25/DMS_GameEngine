#include "ImGuiProcessor.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

ImGuiProcessor::ImGuiProcessor()
	: m_pd3dDevice(nullptr),
	m_pd3dDeviceContext(nullptr),
	m_pSwapChain(nullptr),
	m_ResizeWidth(0),
	m_ResizeHeight(0),
	m_mainRenderTargetView(nullptr),
	clear_color(ImVec4(0.45f, 0.55f, 0.60f, 1.00f)),
	my_float(0.0f),
	my_int(0),
	my_bool(false)
{
	memset(my_text, 0, sizeof(my_text));
}

bool ImGuiProcessor::Initialize(HWND _hWnd)
{
	if (!CreateDeviceD3D(_hWnd))
	{
		CleanupDeviceD3D();
		return false;
	}

	// Dear ImGui 컨텍스트 설정
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); 
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	// Dear ImGui 스타일 설정
	ImGui::StyleColorsDark();

	// 플랫폼/렌더러 백엔드 설정
	if (!ImGui_ImplWin32_Init(_hWnd) || !ImGui_ImplDX11_Init(m_pd3dDevice, m_pd3dDeviceContext))
	{
		return false;
	}

	return true;
}

void ImGuiProcessor::Update()
{
	// Dear ImGui 프레임 시작
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// ImGui 예제 창 표시
	ImGui::ShowDemoWindow();

	// Property Grid 창 생성
	ImGui::Begin("Property Grid");

	// Float 속성
	ImGui::Text("Float Property");
	ImGui::SameLine();
	ImGui::PushItemWidth(-1);
	ImGui::DragFloat("##float", &my_float);
	ImGui::PopItemWidth();

	// Integer 속성
	ImGui::Text("Integer Property");
	ImGui::SameLine();
	ImGui::PushItemWidth(-1);
	ImGui::DragInt("##int", &my_int);
	ImGui::PopItemWidth();

	// Boolean 속성
	ImGui::Text("Boolean Property");
	ImGui::SameLine();
	ImGui::Checkbox("##bool", &my_bool);

	// Text 속성
	ImGui::Text("Text Property");
	ImGui::SameLine();
	ImGui::PushItemWidth(-1);
	ImGui::InputText("##text", my_text, IM_ARRAYSIZE(my_text));
	ImGui::PopItemWidth();

	ImGui::End();

	ImGui::Render();
}

void ImGuiProcessor::Render()
{
	const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
	m_pd3dDeviceContext->OMSetRenderTargets(1, &m_mainRenderTargetView, nullptr);
	m_pd3dDeviceContext->ClearRenderTargetView(m_mainRenderTargetView, clear_color_with_alpha);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	m_pSwapChain->Present(1, 0); // vsync와 함께 프레젠트
}

void ImGuiProcessor::Finalize()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	CleanupDeviceD3D();
}

LRESULT ImGuiProcessor::ImGui_ImplWin32_WndProcHandler(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam)
{
	return ::ImGui_ImplWin32_WndProcHandler(_hWnd, _msg, _wParam, _lParam);
}

bool ImGuiProcessor::CreateDeviceD3D(HWND _hWnd)
{
	// 스왑 체인 설정
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 2;
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = _hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	UINT createDeviceFlags = 0;
	D3D_FEATURE_LEVEL featureLevel;
	const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
	HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags
		, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &m_pSwapChain, &m_pd3dDevice, &featureLevel, &m_pd3dDeviceContext);
	if (res == DXGI_ERROR_UNSUPPORTED)
	{
		res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2
			, D3D11_SDK_VERSION, &sd, &m_pSwapChain, &m_pd3dDevice, &featureLevel, &m_pd3dDeviceContext);
	}
	if (res != S_OK)
	{
		return false;
	}

	CreateRenderTarget();
	return true;
}

void ImGuiProcessor::CleanupDeviceD3D()
{
	CleanupRenderTarget();
	if (m_pSwapChain)
	{
		m_pSwapChain->Release();
		m_pSwapChain = nullptr;
	}
	if (m_pd3dDeviceContext)
	{
		m_pd3dDeviceContext->Release();
		m_pd3dDeviceContext = nullptr;
	}
	if (m_pd3dDevice)
	{
		m_pd3dDevice->Release();
		m_pd3dDevice = nullptr;
	}
}

void ImGuiProcessor::CreateRenderTarget()
{
	ID3D11Texture2D* pBackBuffer;
	m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	m_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_mainRenderTargetView);
	pBackBuffer->Release();
}

void ImGuiProcessor::CleanupRenderTarget()
{
	if (m_mainRenderTargetView)
	{
		m_mainRenderTargetView->Release();
		m_mainRenderTargetView = nullptr;
	}
}
