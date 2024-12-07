#include "FileManager.h"

FileManager::FileManager()
{
}

FileManager::~FileManager()
{
}

void FileManager::Initialize()
{
	// 현재 경로 얻어오기
	std::filesystem::path mCurrentPath = std::filesystem::current_path();

	// 리소스 위치로 이동하기
	std::filesystem::path targetDir = mCurrentPath.parent_path().parent_path() / "Resources";	///절대경로
	mResourceBaseDir = std::filesystem::relative(targetDir, mCurrentPath);						// 상대경로

	// 디렉토리 구조를 출력하는 함수
	//printFolderStructure(m_resourceBaseDir, 0);
	// 루트 디렉토리 구조체 생성
	mRootDirData = new DirData{ mResourceBaseDir.string(), {}, {} };

	// 디렉토리 및 파일 구조 저장
	SaveFolderStructure(mResourceBaseDir, mRootDirData);
}

void FileManager::Finalize()
{
	// 트리 구조를 탐색하며 동적으로 할당된 메모리 해제
	std::list<DirData*> toDeleteList; // 삭제해야할 디렉토리 목록 저장 리스트 생성
	toDeleteList.push_back(mRootDirData); // mRootDirData를 삽입
	while (!toDeleteList.empty())
	{
		DirData* current = toDeleteList.front(); // 삭제 목록의 첫 번째 디렉토리를 가져온다.
		toDeleteList.pop_front(); // 방금 가져온 디렉토리 삭제

		for (auto subDir : current->subDirectories)
		{
			toDeleteList.push_back(subDir);  // 하위 디렉토리를 삭제 목록에 추가
		}
		delete current;  // 현재 디렉토리 삭제
	}

}

FileManager::DirData* FileManager::GetRootDirData()
{
	return mRootDirData;
}

FileManager::DirData* FileManager::GetDirData(const std::string& _dirName)
{
	return SearchDirName(mRootDirData, _dirName);
}

FileManager::DirData* FileManager::GetDirDataFromFile(const std::string& _fileName)
{
	return SearchDirDataFromFile(mRootDirData, _fileName);
}

std::string FileManager::GetDirName(const std::string& _fileName)
{
	return SearchDirDataFromFile(mRootDirData, _fileName)->dirName;
}

std::string FileManager::SearchBasePath(DirData* _currentDir, const std::string& _fileName, const std::string& _parentPath)
{
	// 현재 디렉토리 경로를 parentPath에 누적
	std::string currentPath = mResourceBaseDir.string();
	if (currentPath == _parentPath)
	{
		currentPath = currentPath + "\\";
	}
	else
	{
		currentPath = _parentPath + _currentDir->dirName + "\\";
	}

	// 현재 디렉토리에서 파일을 검색
	for (const auto& file : _currentDir->pInDirFiles)
	{
		if (file == _fileName)
		{
			return currentPath; // 파일 경로 반환
		}
	}

	// 하위 디렉토리에서 재귀적으로 파일을 탐색
	for (auto& subDir : _currentDir->subDirectories)
	{
		std::string result = SearchBasePath(subDir, _fileName, currentPath);
		if (!result.empty())
		{
			return result; // 하위 디렉토리에서 파일을 찾으면 전체 basedPath 반환
		}
	}
	return std::string(); // 파일을 찾지 못한 경우 빈 문자열 반환
}

std::string FileManager::GetBasePath(const std::string& _fileName)
{
	return SearchBasePath(mRootDirData, _fileName, mResourceBaseDir.string());
}

void FileManager::PrintDirStructure(DirData* _currentDir, int _depth /*= 0*/)
{
	for (int i = 0; i < _depth; i++)
	{
		std::cout << "    "; // 들여쓰기
	}
	std::cout << "[Dir] " << _currentDir->dirName << std::endl;

	for (auto& subDir : _currentDir->subDirectories)
	{
		PrintDirStructure(subDir, _depth + 1);
	}
}

void FileManager::PrintDirNFileStructure(DirData* _currentDir, int _depth /*= 0*/)
{
	for (int i = 0; i < _depth; i++)
	{
		std::cout << "    "; // 들여쓰기
	}
	std::cout << "[Dir] " << _currentDir->dirName << std::endl;

	for (const auto& file : _currentDir->pInDirFiles)
	{
		for (int i = 0; i < _depth; i++)
		{
			std::cout << "    " << file << std::endl; // 파일 출력시 더 깊게 들여쓰기
		}
	}

	for (auto& subDir : _currentDir->subDirectories)
	{
		PrintDirNFileStructure(subDir, _depth + 1);
	}
}

void FileManager::SaveFolderStructure(const std::filesystem::path& _directory, DirData* _currentDir)
{
	for (const auto& entry : std::filesystem::directory_iterator(_directory))
	{
		if (entry.is_directory())
		{
			// 새로운 하위 디렉토리를 생성하고, 현재 디렉토리의 하위 목록에 추가
			DirData* newDir = new DirData{ entry.path().filename().string(), {}, {} };
			_currentDir->subDirectories.push_back(newDir);

			// 하위 디렉토리 재귀 호출
			SaveFolderStructure(entry.path(), newDir);
		}
		else if (entry.is_regular_file()) // 파일인 경우
		{
			// 파일은 현재 디렉토리의 파일 목록에 추가
			_currentDir->pInDirFiles.push_back(entry.path().filename().string());
		}
	}
}

std::list<std::string> FileManager::GetAllFilePathList()
{
	std::list<std::string> filePathList;

	// 트리 구조를 순회하면서 모든 파일 경로를 가져옴
	SearchAllFilePaths(mRootDirData, mResourceBaseDir.string() + "\\", filePathList);

	return filePathList;
}

std::list<std::string> FileManager::GetOnlySubDirNameList(DirData* _dirData)
{
	std::list<std::string> dirNameList;

	for (auto& e : _dirData->subDirectories)
	{
		dirNameList.push_back(e->dirName);
	}
	return dirNameList;
}

std::list<std::string> FileManager::GetAllFilesInDirList(DirData* _dirData, bool _isOnlySubDir)
{
	std::list<std::string> allFilesInDirList;

	for (const auto& file : _dirData->pInDirFiles)
	{
		allFilesInDirList.push_back(file);
	}

	if (_isOnlySubDir == false)
	{
		SearchAllFilesInDir(_dirData, allFilesInDirList);
	}

	return allFilesInDirList;
}

void FileManager::PrintAll(bool mIsOnlyDir)
{
	if (mRootDirData)
	{
		if (mIsOnlyDir == true)
		{
			PrintDirStructure(mRootDirData); // 루트 디렉토리부터 출력 시작
		}
		else
		{
			PrintDirNFileStructure(mRootDirData);
		}
	}
}

std::string FileManager::GetFullFilePath(const std::string& _fileName)
{
	// 루트 디렉토리에서 검색을 시작
	std::string dirPath = SearchBasePath(mRootDirData, _fileName, mResourceBaseDir.string());
	if (!dirPath.empty())
	{
		return dirPath + "//" + _fileName;  // 전체 경로와 파일 이름을 합쳐서 반환
	}
	return "";  // 파일을 찾지 못한 경우 빈 문자열 반환
}

std::string FileManager::ProcessFilePath(const std::filesystem::path& _filePath)
{
	std::cout << "File Path: " << _filePath << std::endl;
	return _filePath.string();
}

void FileManager::SearchAllFilePaths(DirData* _currentDir, const std::string& _parentPath, std::list<std::string>& _filePathList)
{
	// 현재 디렉토리 경로를 구성
	std::string currentPath = _parentPath + _currentDir->dirName + "\\";

	// 현재 디렉토리 내의 파일 경로를 리스트에 추가
	for (const auto& file : _currentDir->pInDirFiles)
	{
		_filePathList.push_back(currentPath + file);
	}

	// 하위 다렉토리에서 재귀적으로 파일 경로를 추가
	for (auto& subDir : _currentDir->subDirectories)
	{
		SearchAllFilePaths(subDir, currentPath, _filePathList);
	}
}

FileManager::DirData* FileManager::SearchDirName(DirData* _currentDir, const std::string& _DirName)
{
	// 현재 디렉토리 이름이 찾는 디렉토리 이름과 같다면 반환
	if (_currentDir->dirName == _DirName)
	{
		return _currentDir;
	}

	// 하위 디렉토리를 순회하며 재귀적으로 탐색
	for (auto& subDir : _currentDir->subDirectories)
	{
		// 하위 디렉토리에서 찾은 결과를 저장
		DirData* foundDir = SearchDirName(subDir, _DirName);

		// 찾았다면 그 결과를 반환
		if (foundDir != nullptr)
		{
			return foundDir;
		}
	}

	// 찾지 못하면 nullptr 반환
	return nullptr;
}

FileManager::DirData* FileManager::SearchDirDataFromFile(DirData* _currentDir, const std::string& _fileName)
{
	// 현재 파일을 현재 디렉토리에 있는지 확인한다.
	for (auto& file : _currentDir->pInDirFiles)
	{
		if (file == _fileName)
		{
			return _currentDir;
		}
	}

	// 하위 디렉토리를 순회하며 재귀적으로 탐색
	for (auto& subDir : _currentDir->subDirectories)
	{
		// 하위 디렉토리에서 찾은 결과를 저장
		DirData* foundDir = SearchDirName(subDir, _fileName);

		// 찾았다면 그 결과를 반환
		if (foundDir != nullptr)
		{
			return foundDir;
		}
	}

	// 찾지 못하면 nullptr 반환
	return nullptr;
}

void FileManager::SearchAllFilesInDir(DirData* _dirData, std::list<std::string>& _fileNameList)
{
	// 하위 디렉토리도 재귀적으로 탐색하여 파일을 추가
	for (auto& subDir : _dirData->subDirectories)
	{
		// 하위 디렉토리의 파일을 리스트에 추가
		for (const auto& file : subDir->pInDirFiles)
		{
			_fileNameList.push_back(file);
		}

		// 재귀적으로 하위 디렉토리 탐색
		SearchAllFilesInDir(subDir, _fileNameList);
	}
}
