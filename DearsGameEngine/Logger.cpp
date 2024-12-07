#include "Logger.h"

// 전역 로거 포인터 정의
ILogger* g_pLogger = nullptr;

void InitializeLogger()
{
#ifdef _DEBUG
	g_pLogger = new ConsoleLogger();
#elif defined(NDEBUG)
	g_pLogger = new ConsoleLogger();
	//g_pLogger = new FileLogger();
#endif
}

void PrintLoggerInfo()
{
	std::string strVersion = std::to_string(SPDLOG_VER_MAJOR) + "." + std::to_string(SPDLOG_VER_MINOR) + "." + std::to_string(SPDLOG_VER_PATCH);

	DLOG(LOG_INFO, "Spdlog version: " + strVersion);
	DLOG(LOG_INFO, "Log Level: " + std::to_string(g_pLogger->GetLogLevel()));
}

void CleanupLogger()
{
	delete g_pLogger;
	g_pLogger = nullptr;
}