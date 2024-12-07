#include <iostream>
#include <filesystem>
#include <vector>
#include <string>

/// <summary>
/// 파일 경로 관리, 수정하는 파일 매니저 클래스입니다. 테스트중.
/// </summary>

class FileManager
{
public:
	// 생성자: 선택적으로 루트 디렉토리를 설정
	FileManager(const std::string& rootPath = ".") : rootPath_(rootPath)
	{
		if (!std::filesystem::exists(rootPath_))
		{
			throw std::runtime_error("루트 경로가 존재하지 않습니다");
		}
	}

	// 현재 디렉토리의 파일 및 디렉토리 목록 가져오기
	std::vector<std::filesystem::directory_entry> listDirectory(const std::string& path = "") const 
	{
		std::filesystem::path dirPath = rootPath_ / path;
		if (!std::filesystem::exists(dirPath) || !std::filesystem::is_directory(dirPath)) 
		{
			throw std::runtime_error("경로가 존재하지 않거나 디렉토리가 아닙니다");
		}

		std::vector<std::filesystem::directory_entry> entries;
		for (const auto& entry : std::filesystem::directory_iterator(dirPath)) 
		{
			entries.push_back(entry);
		}
		return entries;
	}

	// 파일 존재 여부 확인
	bool fileExists(const std::string& filePath) const 
	{
		return std::filesystem::exists(rootPath_ / filePath);
	}

	// 파일 크기 가져오기
	uintmax_t getFileSize(const std::string& filePath) const 
	{
		std::filesystem::path fullPath = rootPath_ / filePath;
		if (!std::filesystem::exists(fullPath) || std::filesystem::is_directory(fullPath))
		{
			throw std::runtime_error("파일이 존재하지 않거나 디렉토리입니다");
		}
		return std::filesystem::file_size(fullPath);
	}

	// 새로운 디렉토리 생성
	bool createDirectory(const std::string& dirPath) const 
	{
		std::filesystem::path fullPath = rootPath_ / dirPath;
		return std::filesystem::create_directory(fullPath);
	}

	// 파일 또는 디렉토리 제거
	bool remove(const std::string& path) const 
	{
		std::filesystem::path fullPath = rootPath_ / path;
		return std::filesystem::remove(fullPath);
	}

private:
	std::filesystem::path rootPath_;
};

int main() 
{
	try 
	{
		FileManager fm("test_dir");

		// 디렉토리 내용 나열
		for (const auto& entry : fm.listDirectory())
		{
			std::cout << entry.path() << std::endl;
		}

		// 파일 존재 여부 확인
		std::string filePath = "example.txt";
		if (fm.fileExists(filePath)) {
			std::cout << filePath << " 존재, 크기: " << fm.getFileSize(filePath) << " 바이트" << std::endl;
		}
		else {
			std::cout << filePath << " 존재하지 않음" << std::endl;
		}

		// 새로운 디렉토리 생성
		if (fm.createDirectory("new_dir")) 
		{
			std::cout << "디렉토리 생성: new_dir" << std::endl;
		}

		// 파일 또는 디렉토리 제거
		if (fm.remove("new_dir"))
		{
			std::cout << "제거됨: new_dir" << std::endl;
		}

	}
	catch (const std::exception& e)
	{
		std::cerr << "오류: " << e.what() << std::endl;
	}

	return 0;
}
