#include "pch.h"
#include "BaseWindowsProcessor.h"
#include "Resource.h"
#include "GameEngine.h"

BaseWindowsProcessor::BaseWindowsProcessor(HINSTANCE _hInstance, const WCHAR* _szTitle = L"BaseApp", int _iconResourceId = IDI_ICONDMB)
	:mHInstance(_hInstance)
	, mHWnd()
	, mMsg()
	, mHAccelTable()
	, mIconResourceId(_iconResourceId)
{
	wcscpy_s(mSzTitle, _szTitle);
}

BaseWindowsProcessor::BaseWindowsProcessor(int _hInstance, const WCHAR* _szTitle = L"BaseApp", int _iconResourceId = IDI_ICONDMB)
	:mHInstance(reinterpret_cast<HINSTANCE>(_hInstance))
	, mHWnd()
	, mMsg()
	, mHAccelTable()
	, mIconResourceId(_iconResourceId)
{
	wcscpy_s(mSzTitle, _szTitle);
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
	wcex.hInstance = mHInstance;
	wcex.hIcon = LoadIcon(mHInstance, MAKEINTRESOURCE(mIconResourceId));
	wcex.hCursor = NULL/*hCousor*//*LoadCursor(nullptr, IDC_ARROW)*/;
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = 0;
	wcex.lpszClassName = mSzTitle;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(mIconResourceId));

	RegisterClassExW(&wcex);
	// 원하는 스타일로 설정 (제목 표시줄, 테두리, 스크롤바 제거)
	DWORD dwStyle = WS_POPUP;

	RECT rect = { 0, 0, mScreenWidth, mScreenHeight };
	AdjustWindowRect(&rect, dwStyle, FALSE);
	int windowWidth = rect.right - rect.left;
	int windowHeight = rect.bottom - rect.top;

	mHWnd = CreateWindowW(mSzTitle, mSzTitle, dwStyle,
		mScreenPosX, mScreenPosY, windowWidth, windowHeight, nullptr, nullptr, mHInstance, nullptr);

	if (!mHWnd)
	{
		return false;
	}

	ShowWindow(mHWnd, SW_SHOWNORMAL);
	UpdateWindow(mHWnd);

	mHAccelTable = LoadAccelerators(mHInstance, MAKEINTRESOURCE(IDI_ICONDMB));

	/// 시스템 생성 및 초기화
	// 디버그 모드에서 콘솔창 생성
#ifdef _DEBUG	
	CreateConsole();
#endif // _DEBUG

	return true;
}

void BaseWindowsProcessor::OnResize(uint16_t _width, uint16_t _height)
{
	mScreenWidth = _width;
	mScreenHeight = _height;
}

void BaseWindowsProcessor::Run()
{
	while (true)
	{
		if (PeekMessage(&mMsg, NULL, 0, 0, PM_REMOVE))
		{
			if (mMsg.message == WM_QUIT)
			{
				break;
			}

			if (!TranslateAccelerator(mMsg.hwnd, mHAccelTable, &mMsg))
			{
				TranslateMessage(&mMsg);
				DispatchMessage(&mMsg);
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
	return mHWnd;
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
