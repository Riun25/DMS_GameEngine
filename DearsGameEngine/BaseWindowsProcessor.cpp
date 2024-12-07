#include "pch.h"
#include "BaseWindowsProcessor.h"
#include "Resource.h"
#include "GameEngine.h"

BaseWindowsProcessor::BaseWindowsProcessor(HINSTANCE _hInstance, const WCHAR* _szTitle = L"BaseApp", int _iconResourceId = IDI_ICONDMB)
	:m_hInstance(_hInstance)
	, m_hWnd()
	, m_msg()
	, m_hAccelTable()
	, m_iconResourceId(_iconResourceId)
{
	wcscpy_s(m_szTitle, _szTitle);
}

BaseWindowsProcessor::BaseWindowsProcessor(int _hInstance, const WCHAR* _szTitle = L"BaseApp", int _iconResourceId = IDI_ICONDMB)
	:m_hInstance(reinterpret_cast<HINSTANCE>(_hInstance))
	, m_hWnd()
	, m_msg()
	, m_hAccelTable()
	, m_iconResourceId(_iconResourceId)
{
	wcscpy_s(m_szTitle, _szTitle);
}

bool BaseWindowsProcessor::Initialize()
{
	// 기본 커서를 로드합니다 (IDC_ARROW).
// 	HCURSOR hArrowCursor = LoadCursor(NULL, IDC_ARROW);
// 
// 	// 커서 이미지를 복사하면서 크기를 변경
// 	HCURSOR hCousor = (HCURSOR)CopyImage(hArrowCursor, IMAGE_CURSOR, 64, 64, 0);

	WNDCLASSEXW wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = BaseWindowsProcessor::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hInstance;
	wcex.hIcon = LoadIcon(m_hInstance, MAKEINTRESOURCE(m_iconResourceId));
	wcex.hCursor = NULL/*hCousor*//*LoadCursor(nullptr, IDC_ARROW)*/;
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = 0;
	wcex.lpszClassName = m_szTitle;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(m_iconResourceId));

	RegisterClassExW(&wcex);
	// 원하는 스타일로 설정 (제목 표시줄, 테두리, 스크롤바 제거)
	DWORD dwStyle = WS_POPUP;

	RECT rect = { 0, 0, m_screenWidth, m_screenHeight };
	AdjustWindowRect(&rect, dwStyle, FALSE);
	int windowWidth = rect.right - rect.left;
	int windowHeight = rect.bottom - rect.top;

	m_hWnd = CreateWindowW(m_szTitle, m_szTitle, dwStyle,
		m_screenPosX, m_screenPosY, windowWidth, windowHeight, nullptr, nullptr, m_hInstance, nullptr);

	if (!m_hWnd)
	{
		return false;
	}

	ShowWindow(m_hWnd, SW_SHOWNORMAL);
	UpdateWindow(m_hWnd);

	m_hAccelTable = LoadAccelerators(m_hInstance, MAKEINTRESOURCE(IDI_ICONDMB));

	/// 시스템 생성 및 초기화
	// 디버그 모드에서 콘솔창 생성
#ifdef _DEBUG	
	CreateConsole();
#endif // _DEBUG

	return true;
}

void BaseWindowsProcessor::OnResize(uint16_t _width, uint16_t _height)
{
	m_screenWidth = _width;
	m_screenHeight = _height;
}

void BaseWindowsProcessor::Run()
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
		}
	}
}

void BaseWindowsProcessor::Finalize()
{
}

HWND BaseWindowsProcessor::GetHWND() const
{
	return m_hWnd;
}

LRESULT CALLBACK BaseWindowsProcessor::WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam)
{
	// 	BaseWindowsProcessor* processor = reinterpret_cast<BaseWindowsProcessor*>(GetWindowLongPtr(_hWnd, GWLP_USERDATA));
	switch (_message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(_wParam);
		// 메뉴 선택을 구문 분석합니다:
		switch (wmId)
		{
		case IDM_FULLSCREEN:

			break;
		case IDM_EXIT:
			DestroyWindow(_hWnd);
			break;
		default:
			return DefWindowProc(_hWnd, _message, _wParam, _lParam);
		}
	}
	break;
	// 		case WM_SIZE:
	// 			uint16_t width = LOWORD(_lParam);
	// 			uint16_t height = HIWORD(_lParam);
	// 			processor->OnResize(width, height);
	// 			break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CLOSE:
		DestroyWindow(_hWnd);
		break;
	default:
		return DefWindowProc(_hWnd, _message, _wParam, _lParam);
	}
	return 0;
}

void BaseWindowsProcessor::Update()
{
}

void BaseWindowsProcessor::CreateConsole()
{
	// 콘솔 창 할당
	AllocConsole();

	// 표준 입출력 리디렉션
	FILE* dummy;
	_wfreopen_s(&dummy, L"CONIN$", L"r", stdin);

	// 표준 출력 리디렉션
	_wfreopen_s(&dummy, L"CONOUT$", L"w", stdout);

	// 표준 에러 리디렉션
	_wfreopen_s(&dummy, L"CONOUT$", L"w", stderr);

	// 콘솔 창 핸들을 가져옴
	HWND consoleWindow = GetConsoleWindow();

	// 콘솔 창의 초기 위치 설정 (예: 좌상단에서 100, 100 위치로 설정)
	SetWindowPos(consoleWindow, NULL, 1500, 500, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}
