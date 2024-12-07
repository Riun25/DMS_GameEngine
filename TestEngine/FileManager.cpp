#include <iostream>
#include <filesystem>
#include <vector>
#include <string>

/// <summary>
/// ���� ��� ����, �����ϴ� ���� �Ŵ��� Ŭ�����Դϴ�. �׽�Ʈ��.
/// </summary>

class FileManager
{
public:
	// ������: ���������� ��Ʈ ���丮�� ����
	FileManager(const std::string& rootPath = ".") : rootPath_(rootPath)
	{
		if (!std::filesystem::exists(rootPath_))
		{
			throw std::runtime_error("��Ʈ ��ΰ� �������� �ʽ��ϴ�");
		}
	}

	// ���� ���丮�� ���� �� ���丮 ��� ��������
	std::vector<std::filesystem::directory_entry> listDirectory(const std::string& path = "") const 
	{
		std::filesystem::path dirPath = rootPath_ / path;
		if (!std::filesystem::exists(dirPath) || !std::filesystem::is_directory(dirPath)) 
		{
			throw std::runtime_error("��ΰ� �������� �ʰų� ���丮�� �ƴմϴ�");
		}

		std::vector<std::filesystem::directory_entry> entries;
		for (const auto& entry : std::filesystem::directory_iterator(dirPath)) 
		{
			entries.push_back(entry);
		}
		return entries;
	}

	// ���� ���� ���� Ȯ��
	bool fileExists(const std::string& filePath) const 
	{
		return std::filesystem::exists(rootPath_ / filePath);
	}

	// ���� ũ�� ��������
	uintmax_t getFileSize(const std::string& filePath) const 
	{
		std::filesystem::path fullPath = rootPath_ / filePath;
		if (!std::filesystem::exists(fullPath) || std::filesystem::is_directory(fullPath))
		{
			throw std::runtime_error("������ �������� �ʰų� ���丮�Դϴ�");
		}
		return std::filesystem::file_size(fullPath);
	}

	// ���ο� ���丮 ����
	bool createDirectory(const std::string& dirPath) const 
	{
		std::filesystem::path fullPath = rootPath_ / dirPath;
		return std::filesystem::create_directory(fullPath);
	}

	// ���� �Ǵ� ���丮 ����
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

		// ���丮 ���� ����
		for (const auto& entry : fm.listDirectory())
		{
			std::cout << entry.path() << std::endl;
		}

		// ���� ���� ���� Ȯ��
		std::string filePath = "example.txt";
		if (fm.fileExists(filePath)) {
			std::cout << filePath << " ����, ũ��: " << fm.getFileSize(filePath) << " ����Ʈ" << std::endl;
		}
		else {
			std::cout << filePath << " �������� ����" << std::endl;
		}

		// ���ο� ���丮 ����
		if (fm.createDirectory("new_dir")) 
		{
			std::cout << "���丮 ����: new_dir" << std::endl;
		}

		// ���� �Ǵ� ���丮 ����
		if (fm.remove("new_dir"))
		{
			std::cout << "���ŵ�: new_dir" << std::endl;
		}

	}
	catch (const std::exception& e)
	{
		std::cerr << "����: " << e.what() << std::endl;
	}

	return 0;
}
