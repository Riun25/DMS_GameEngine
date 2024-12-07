#include "Utility.h"

int RandomUtil::RandomInt(int _min, int _max)
{
	std::random_device rd;
	std::mt19937 engine(rd());
	std::uniform_int_distribution<int> dist(_min, _max);

	return dist(engine);
}

float RandomUtil::RandomFloat(float _min, float _max)
{
	std::random_device rd;
	std::mt19937 engine(rd());
	std::uniform_real_distribution<float> dist(_min, _max);

	return dist(engine);
}

std::string Utility::ToString(const TCHAR* _value)
{
	size_t len = wcslen((wchar_t*)_value);
	char* temp = new char[2 * len + 1];
	wcstombs_s(0, temp, 2 * len + 1, _value, len);
	std::string res = temp;

	delete[] temp;

	return res;
}

std::string Utility::ToString(const std::wstring_view _value)
{
	std::string temp;
	temp.assign(_value.begin(), _value.end());

	return temp;
}

std::wstring Utility::ToWString(const std::string_view _value)
{
	std::wstring temp;
	temp.assign(_value.begin(), _value.end());

	return temp;
}

std::wstring Utility::OpenFileDialog()
{
	OPENFILENAME ofn;
	TCHAR filePath[MAX_FILE_LENGTH] = L"";
	TCHAR fileName[MAX_FILE_LENGTH] = L"";
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = GetForegroundWindow();
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_FILE_LENGTH;
	ofn.lpstrFilter = L"All\0*.*\0Text\0*.txt\0Model\0*.fbx\0Image\0*.png\0Json\0*.json\0Xml\0*.xml";
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn) == TRUE)
	{
		return ofn.lpstrFile;

		// 
// 			// 쓰기 권한 추가하려면 "GENERIC_WRITE" 주석 해제
// 			CreateFile(ofn.lpstrFile, GENERIC_READ /*| GENERIC_WRITE*/	 
// 				, 0, (LPSECURITY_ATTRIBUTES)0,
// 				OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, (HANDLE)0);
	}
}