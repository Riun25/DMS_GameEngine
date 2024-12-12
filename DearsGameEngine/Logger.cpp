#include "Logger.h"

// 전역 로거 포인터 정의
ILogger* gpLogger = nullptr;

void InitializeLogger()
{
#ifdef _DEBUG
	gpLogger = new ConsoleLogger();
#elif defined(NDEBUG)
	gpLogger = new ConsoleLogger();
	//g_pLogger = new FileLogger();
#endif
}

void PrintLoggerInfo()
{
	std::string strVersion = std::to_string(SPDLOG_VER_MAJOR) + "." + std::to_string(SPDLOG_VER_MINOR) + "." + std::to_string(SPDLOG_VER_PATCH);

	DLOG(LOG_INFO, "Spdlog version: " + strVersion);
	DLOG(LOG_INFO, "Log Level: " + std::to_string(gpLogger->GetLogLevel()));
}

void CleanupLogger()
{
	delete gpLogger;
	gpLogger = nullptr;
}