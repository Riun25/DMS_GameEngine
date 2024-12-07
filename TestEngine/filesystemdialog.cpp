#include "framework.h"
#include "commdlg.h"

/// <summary>
/// Windows�� dialog box �׽�Ʈ�ڵ��Դϴ�.
/// ����� file dialog boxâ�� �����˴ϴ�.
/// </summary>

int main()
{
	OPENFILENAME ofn;       // ���� ���̾�α� �ڽ� ����ü
	wchar_t szFile[260];       // ���� �̸��� ������ �迭

	// OPENFILENAME ����ü �ʱ�ȭ
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;  // ���̾�α��� �θ� ������, NULL�̸� ����ũ���� �θ� �����찡 �˴ϴ�
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = L'\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"All files\0*.*\0Text Files\0*.TXT\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// ���� ���� ���̾�α� ǥ��
	if (GetOpenFileName(&ofn) == TRUE) 
	{
		// ���� �̸� ���
		std::wcout << "Selected file: " << ofn.lpstrFile << std::endl;
	}


	return 0;
}