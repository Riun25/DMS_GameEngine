#ifdef _DEBUG
#define  _CRTDBG_MAP_ALLOC
#endif // _DEBUG

#include "pch.h"
#include "DemoProcessor.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // _DEBUG
	std::unique_ptr<DemoProcessor> application = std::make_unique<DemoProcessor>(hInstance, L"DemoApp", 10);

	application->Initialize();
	application->Run();
	application->Finalize();

	return 0;
}