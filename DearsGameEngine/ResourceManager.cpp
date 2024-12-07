#include "ResourceManager.h"
#include "FileManager.h"
#include "SoundManager.h"

ResourceManager::ResourceManager(std::shared_ptr<DearsGraphicsEngine> _pGraphicsEngine, SoundManager* _pSoundManager)
{
	mpGraphicsEngine = _pGraphicsEngine;
	mpSoundManager = _pSoundManager;
	mpFileManager = nullptr;
	mIsAlreadyFontLoaded = false;
}

bool ResourceManager::Initialize()
{
	mpFileManager = new FileManager();
	mpFileManager->Initialize();
	return true;
}

void ResourceManager::Finalize()
{
	RemoveFilesInDirTree("Animation");
	RemoveFilesInDirTree("Csv");
	RemoveFilesInDirTree("Dummy");
	RemoveFilesInDirTree("Fbx");
// 	RemoveFilesInDirTree("Font");
	RemoveFilesInDirTree("Graphics");
	RemoveFilesInDirTree("Sound");
	RemoveFilesInDirTree("Texture");

	mpFileManager->Finalize();
	delete mpFileManager;
}

void ResourceManager::AddResource(const std::string& _fileName)
{
	std::string basePath = mpFileManager->GetBasePath(_fileName);

	AddFunctionType selectedFunction = FunctionType(basePath);

	(mpGraphicsEngine.get()->*selectedFunction)(basePath, _fileName);
}

void ResourceManager::AddFilesInDir(const std::string& _dirName)
{
	std::list<std::string>& inDirFileList = mpFileManager->GetDirData(_dirName)->pInDirFiles;

	if (inDirFileList.empty())
	{
		DLOG(LOG_INFO, "File is Empty.");
		return;
	}

	// 디렉토리 내 첫 번째 파일 이름과 경로를 가져옴
	std::string fileName = inDirFileList.front();
	std::string basePath = mpFileManager->GetBasePath(fileName);

	// basePath에 따라 적절한 함수 포인터 선택
	AddFunctionType selectedFunction = FunctionType(basePath);

	// 선택된 함수가 있으면 현재 디렉토리의 모든 파일 처리
	if (selectedFunction)
	{
		for (const auto& file : inDirFileList)
		{
			(mpGraphicsEngine.get()->*selectedFunction)(basePath, file);
		}
	}
}

void ResourceManager::AddFilesInDir2D(const std::string& _dirName)
{
	std::list<std::string>& inDirFileList = mpFileManager->GetDirData(_dirName)->pInDirFiles;

	if (inDirFileList.empty())
	{
		DLOG(LOG_INFO, "File is Empty.");
		return;
	}

	// 디렉토리 내 첫 번째 파일 이름과 경로를 가져옴
	std::string fileName = inDirFileList.front();
	std::string basePath = mpFileManager->GetBasePath(fileName);


	// 선택된 함수가 있으면 현재 디렉토리의 모든 파일 처리
	for (const auto& file : inDirFileList)
	{
		mpGraphicsEngine->Add2DTexture(basePath, file);
	}
}

void ResourceManager::AddFilesInDir3D(const std::string& _dirName)
{
	std::list<std::string>& inDirFileList = mpFileManager->GetDirData(_dirName)->pInDirFiles;

	if (inDirFileList.empty())
	{
		DLOG(LOG_INFO, "File is Empty.");
		return;
	}

	// 디렉토리 내 첫 번째 파일 이름과 경로를 가져옴
	std::string fileName = inDirFileList.front();
	std::string basePath = mpFileManager->GetBasePath(fileName);


	// 선택된 함수가 있으면 현재 디렉토리의 모든 파일 처리
	for (const auto& file : inDirFileList)
	{
		mpGraphicsEngine->Add3DTexture(basePath, file);
	}
}

void ResourceManager::AddFilesInDirModel(const std::string& _dirName)
{
	std::list<std::string>& inDirFileList = mpFileManager->GetDirData(_dirName)->pInDirFiles;

	if (inDirFileList.empty())
	{
		DLOG(LOG_INFO, "File is Empty.");
		return;
	}

	// 디렉토리 내 첫 번째 파일 이름과 경로를 가져옴
	std::string fileName = inDirFileList.front();
	std::string basePath = mpFileManager->GetBasePath(fileName);


	// 선택된 함수가 있으면 현재 디렉토리의 모든 파일 처리
	for (const auto& file : inDirFileList)
	{
		mpGraphicsEngine->AddModel(basePath, file);
	}
}

void ResourceManager::AddFilesInDirAni(const std::string& _dirName)
{
	std::list<std::string>& inDirFileList = mpFileManager->GetDirData(_dirName)->pInDirFiles;

	if (inDirFileList.empty())
	{
		DLOG(LOG_INFO, "File is Empty.");
		return;
	}

	// 디렉토리 내 첫 번째 파일 이름과 경로를 가져옴
	std::string fileName = inDirFileList.front();
	std::string basePath = mpFileManager->GetBasePath(fileName);


	// 선택된 함수가 있으면 현재 디렉토리의 모든 파일 처리
	for (const auto& file : inDirFileList)
	{
		mpGraphicsEngine->AddAnimation(basePath, file);
	}
}

void ResourceManager::AddFilesInDirDDS(const std::string& _dirName)
{
	std::list<std::string>& inDirFileList = mpFileManager->GetDirData(_dirName)->pInDirFiles;

	if (inDirFileList.empty())
	{
		DLOG(LOG_INFO, "File is Empty.");
		return;
	}

	// 디렉토리 내 첫 번째 파일 이름과 경로를 가져옴
	std::string fileName = inDirFileList.front();
	std::string basePath = mpFileManager->GetBasePath(fileName);


	// 선택된 함수가 있으면 현재 디렉토리의 모든 파일 처리
	for (const auto& file : inDirFileList)
	{
		mpGraphicsEngine->AddDDSTexture(basePath, file);
	}
}

void ResourceManager::AddFilesInDirBGMSound(const std::string& _dirName)
{
	std::list<std::string>& inDirFileList = mpFileManager->GetDirData(_dirName)->pInDirFiles;

	if (inDirFileList.empty())
	{
		DLOG(LOG_INFO, "File is Empty.");
		return;
	}

	// 디렉토리 내 첫 번째 파일 이름과 경로를 가져옴
	std::string fileName = inDirFileList.front();
	std::string basePath = mpFileManager->GetBasePath(fileName);


	// 선택된 함수가 있으면 현재 디렉토리의 모든 파일 처리
	for (const auto& file : inDirFileList)
	{
		std::string soundName = std::filesystem::path(file).stem().string();
		mpSoundManager->LoadBGM(basePath + file, soundName);
	}
}

void ResourceManager::AddFilesInDirSFXSound(const std::string& _dirName)
{
	std::list<std::string>& inDirFileList = mpFileManager->GetDirData(_dirName)->pInDirFiles;

	if (inDirFileList.empty())
	{
		DLOG(LOG_INFO, "File is Empty.");
		return;
	}

	// 디렉토리 내 첫 번째 파일 이름과 경로를 가져옴
	std::string fileName = inDirFileList.front();
	std::string basePath = mpFileManager->GetBasePath(fileName);


	// 선택된 함수가 있으면 현재 디렉토리의 모든 파일 처리
	for (const auto& file : inDirFileList)
	{
		std::string soundName = std::filesystem::path(file).stem().string();
		mpSoundManager->LoadSFX(basePath + file, soundName);
	}
}

void ResourceManager::AddFilesInDirTree(const std::string& _dirName)
{
	// DirData 구조체에서 파일 리스트와 서브 디렉토리 정보를 가져옴
	auto pDirData = mpFileManager->GetDirData(_dirName);
	std::list<std::string>& inDirFileList = pDirData->pInDirFiles;
	auto* subDirs = &pDirData->subDirectories;

	// 파일이 있는 경우
	if (!inDirFileList.empty())
	{
		// 디렉토리 내 첫 번째 파일 이름과 경로를 가져옴
		std::string fileName = inDirFileList.front();
		std::string basePath = mpFileManager->GetBasePath(fileName);

		// basePath에 따라 적절한 함수 포인터 선택
		AddFunctionType selectedFunction = FunctionType(basePath);

		// 선택된 함수가 있으면 현재 디렉토리의 모든 파일 처리
		if (selectedFunction)
		{
			for (const auto& file : inDirFileList)
			{
				(mpGraphicsEngine.get()->*selectedFunction)(basePath, file);
			}
		}
	}

	// 하위 디렉토리도 재귀적으로 처리
	for (auto& subDir : *subDirs)
	{
		AddFilesInDirTree(subDir->dirName); // 재귀 호출로 하위 디렉토리 처리
	}
}

void ResourceManager::AddFont(const std::string& _fileName, float _size, bool _isKorean)
{
	std::string basePath = mpFileManager->GetBasePath(_fileName);

	mpGraphicsEngine->AddFont(basePath, _fileName, _size, _isKorean);
}

void ResourceManager::FontSetFinish()
{
	if (mIsAlreadyFontLoaded == false)
	{
		mpGraphicsEngine->FontSetFinish();
		mIsAlreadyFontLoaded = true;
	}
	else
	{
		std::cout << "이미 폰트 세팅을 호출했습니다. 추가 세팅이 무시됩니다." << std::endl;
	}
}

std::vector<std::vector<int>> ResourceManager::ParseMapCSV(const std::string& _fileName)
{
	std::vector<std::vector<int>> mMap = {};
	std::string filepath = mpFileManager->GetFullFilePath(_fileName);
	std::ifstream file(filepath);
	std::string line;

	// 파일 오픈 실패 시 빈 벡터 반환
	if (!file.is_open())
	{
		DLOG(LOG_INFO, "Failed to open file: " + filepath);
		return mMap;
	}

	int columnCount = 0;

	// 첫 번째 줄(헤더)을 읽어들임 -> 칼럼 개수 카운트
	if (std::getline(file, line))
	{
		std::stringstream ss(line);
		std::string column;

		// 쉼표로 구분된 각 필드를 세어 칼럼 개수 카운트
		while (std::getline(ss, column, ','))
		{
			columnCount++;
		}

		// 첫 번째 줄을 데이터로 변환해서 mMap에 추가
		std::stringstream ssFirstRow(line);
		std::string cell;
		std::vector<int> row;  // 여기서 string 대신 T 사용
		while (std::getline(ssFirstRow, cell, ','))
		{
			/*for (char c : line) { // BOM을 볼 수 있는 코드
				std::cout << "Character: '" << c << "', ASCII: " << static_cast<int>(c) << std::endl;
			}*/

			// 처음 3문자가 BOM이라고 해서 특이한 값이 들어간다. 제거해보려 했는데 안 되어서 try catch로 했다.
			try
			{
				row.push_back(std::stoi(cell));  // 문자열을 정수로 변환
			}
			catch (const std::invalid_argument& e)
			{
				DLOG(LOG_INFO, "Error: Failed to convert '" + cell + "' to integer. Skipping this value.");
			}
		}
		mMap.push_back(row);
	}

	// 두 번째 줄부터 다시 읽어준다.
	while (std::getline(file, line))
	{
		std::stringstream ss(line);
		std::string cell;
		std::vector<int> row;
		while (std::getline(ss, cell, ','))
		{
			row.push_back(std::stoi(cell));  // 문자열을 정수로 변환
		}

		// 각 행이 올바른 칼럼 개수를 갖는지 확인
		if (row.size() == columnCount)
		{  // 모든 칼럼이 있는지 확인
			mMap.emplace_back(row);
		}
		else
		{
			DLOG(LOG_INFO, "Incomplete row found and skipped.");
		}
	}
	return mMap;
}

FileManager* ResourceManager::GetFileManager()
{
	return mpFileManager;
}

Model* ResourceManager::Get_ModelInfo(std::string _modelName)
{
	return mpGraphicsEngine->Get_ModelInfo(_modelName);
}

Animation* ResourceManager::Get_Animation(std::string _animeName)
{
	return mpGraphicsEngine->Get_Animation(_animeName);
}

ComPtr<ID3D11ShaderResourceView> ResourceManager::Get_Textures(std::string _textureName)
{
	return mpGraphicsEngine->Get_Textures(_textureName);
}

ImFont* ResourceManager::Get_Font(std::string _fontName)
{
	return mpGraphicsEngine->Get_Font(_fontName);
}

int ResourceManager::Get_TargetModelBoneIndex(std::string _meshName, std::string _boneName)
{
	return mpGraphicsEngine->Get_TargetModelBoneIndex(_meshName, _boneName);
}

Matrix ResourceManager::GetTargetBoneMatrix(std::string _targetModelName, std::string _targetBoneName)
{
	return mpGraphicsEngine->GetTargetBoneMatrix(_targetModelName, _targetBoneName);
}

Matrix ResourceManager::GetTargetBoneAboveMatrix(std::string _targetModelName, std::string _targetBoneName, float _scale /*= 1.f*/)
{
	return mpGraphicsEngine->GetTargetBoneAboveMatrix(_targetModelName, _targetBoneName, _scale);
}

AABB ResourceManager::Get_AABB(std::string __targetModelName)
{
	return mpGraphicsEngine->Get_AABB(__targetModelName);
}

void ResourceManager::RemoveResource(const std::string& _fileName)
{
	std::string basePath = mpFileManager->GetBasePath(_fileName);

	RemoveFuctionType selectedFunction = EraseFunctionType(basePath);

	(mpGraphicsEngine.get()->*selectedFunction)(_fileName);
}

void ResourceManager::RemoveFilesInDir(const std::string& _dirName)
{
	std::list<std::string>& inDirFileList = mpFileManager->GetDirData(_dirName)->pInDirFiles;

	if (inDirFileList.empty())
	{
		std::cout << "빈 파일입니다." << std::endl;
		return;
	}

	// 디렉토리 내 첫 번째 파일 이름과 경로를 가져옴
	std::string fileName = inDirFileList.front();
	std::string basePath = mpFileManager->GetBasePath(fileName);

	// basePath에 따라 적절한 함수 포인터 선택
	RemoveFuctionType selectedFunction = EraseFunctionType(basePath);

	// 선택된 함수가 있으면 현재 디렉토리의 모든 파일 처리
	if (selectedFunction)
	{
		for (const auto& file : inDirFileList)
		{
			(mpGraphicsEngine.get()->*selectedFunction)(file);
		}
	}
}

void ResourceManager::RemoveFilesInDirTree(const std::string& _dirName)
{
	// DirData 구조체에서 파일 리스트와 서브 디렉토리 정보를 가져옴
	auto pDirData = mpFileManager->GetDirData(_dirName);
	std::list<std::string>& inDirFileList = pDirData->pInDirFiles;
	auto* subDirs = &pDirData->subDirectories;

	// 파일이 있는 경우
	if (!inDirFileList.empty())
	{
		// 디렉토리 내 첫 번째 파일 이름과 경로를 가져옴
		std::string fileName = inDirFileList.front();
		std::string basePath = mpFileManager->GetBasePath(fileName);

		// basePath에 따라 적절한 함수 포인터 선택
		RemoveFuctionType selectedFunction = EraseFunctionType(basePath);

		// 선택된 함수가 있으면 현재 디렉토리의 모든 파일 처리
		if (selectedFunction)
		{
			for (const auto& file : inDirFileList)
			{
				(mpGraphicsEngine.get()->*selectedFunction)(file);
			}
		}
	}

	// 하위 디렉토리도 재귀적으로 처리
	for (auto& subDir : *subDirs)
	{
		RemoveFilesInDirTree(subDir->dirName); // 재귀 호출로 하위 디렉토리 처리
	}
}

ResourceManager::AddFunctionType ResourceManager::FunctionType(const std::string& _basePath)
{
	// 함수 포인터 선언 및 초기화
	AddFunctionType selectedFunction = nullptr;

	// basePath에 따라 적절한 함수 포인터 선택
	if (_basePath.find("\\Texture2D\\") != std::string::npos)
	{
		selectedFunction = &DearsGraphicsEngine::Add2DTexture;
	}
	else if (_basePath.find("\\Texture3D\\") != std::string::npos)
	{
		selectedFunction = &DearsGraphicsEngine::Add3DTexture;
	}
	else if (_basePath.find("\\Fbx\\") != std::string::npos)
	{
		selectedFunction = &DearsGraphicsEngine::AddModel;
	}
	else if (_basePath.find("\\Animation\\") != std::string::npos)
	{
		selectedFunction = &DearsGraphicsEngine::AddAnimation;
	}
	else if (_basePath.find("\\CubeMap\\") != std::string::npos)
	{
		selectedFunction = &DearsGraphicsEngine::AddDDSTexture;
	}

	return selectedFunction;
}

ResourceManager::RemoveFuctionType ResourceManager::EraseFunctionType(const std::string& _basePath)
{
	// 함수 포인터 선언 및 초기화
	RemoveFuctionType selectedFunction = nullptr;

	// basePath에 따라 적절한 함수 포인터 선택
	if (_basePath.find("\\Texture\\") != std::string::npos)
	{
		selectedFunction = &DearsGraphicsEngine::Erase_Textures;
	}
	else if (_basePath.find("\\Fbx\\") != std::string::npos)
	{
		selectedFunction = &DearsGraphicsEngine::Erase_ModelInfo;
	}
	else if (_basePath.find("\\Animation\\") != std::string::npos)
	{
		selectedFunction = &DearsGraphicsEngine::Erase_Animation;
	}
	else if (_basePath.find("\\Font\\") != std::string::npos)
	{
		selectedFunction = &DearsGraphicsEngine::Erase_Font;
	}

	return selectedFunction;
}