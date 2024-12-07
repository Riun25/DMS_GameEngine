#include "framework.h"
#include "commdlg.h"

/// <summary>
/// Windows의 dialog box 테스트코드입니다.
/// 실행시 file dialog box창이 생성됩니다.
/// </summary>

int main()
{
	OPENFILENAME ofn;       // 공통 다이얼로그 박스 구조체
	wchar_t szFile[260];       // 파일 이름을 저장할 배열

	// OPENFILENAME 구조체 초기화
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;  // 다이얼로그의 부모 윈도우, NULL이면 데스크톱이 부모 윈도우가 됩니다
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = L'\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"All files\0*.*\0Text Files\0*.TXT\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// 파일 열기 다이얼로그 표시
	if (GetOpenFileName(&ofn) == TRUE) 
	{
		// 파일 이름 출력
		std::wcout << "Selected file: " << ofn.lpstrFile << std::endl;
	}


	return 0;
}