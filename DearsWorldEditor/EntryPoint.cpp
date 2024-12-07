#define  _CRTDBG_MAP_ALLOC
#include <windows.h>
#include <memory.h>
#include "DearsWorldEditor.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	std::unique_ptr<EditorProcessor> application = std::make_unique<EditorProcessor>(hInstance);

	application->Initialize();
	application->Run();
	application->Finalize();

	return 0;
}