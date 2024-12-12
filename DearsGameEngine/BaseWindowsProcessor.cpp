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
	// �⺻ Ŀ���� �ε��մϴ� (IDC_ARROW).
// 	HCURSOR hArrowCursor = LoadCursor(NULL, IDC_ARROW);
// 
// 	// Ŀ�� �̹����� �����ϸ鼭 ũ�⸦ ����
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
	// ���ϴ� ��Ÿ�Ϸ� ���� (���� ǥ����, �׵θ�, ��ũ�ѹ� ����)
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

	/// �ý��� ���� �� �ʱ�ȭ
	// ����� ��忡�� �ܼ�â ����
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
		// �޴� ������ ���� �м��մϴ�:
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
	// �ܼ� â �Ҵ�
	AllocConsole();

	// ǥ�� ����� ���𷺼�
	FILE* dummy;
	_wfreopen_s(&dummy, L"CONIN$", L"r", stdin);

	// ǥ�� ��� ���𷺼�
	_wfreopen_s(&dummy, L"CONOUT$", L"w", stdout);

	// ǥ�� ���� ���𷺼�
	_wfreopen_s(&dummy, L"CONOUT$", L"w", stderr);

	// �ܼ� â �ڵ��� ������
	HWND consoleWindow = GetConsoleWindow();

	// �ܼ� â�� �ʱ� ��ġ ���� (��: �»�ܿ��� 100, 100 ��ġ�� ����)
	SetWindowPos(consoleWindow, NULL, 1500, 500, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}
