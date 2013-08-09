#include "stdafx.h"
#include "Log4cplus.h"

#include "log4cplus\layout.h"
#include "log4cplus\fileappender.h"
#include "log4cplus\win32debugappender.h"
#include "log4cplus\configurator.h"

#define MAX_LOG_SIZE 8192

Utils::CLogger::CLogger()
{
	log4cplus::initialize();
#ifdef _DEBUG
	m_DefaultLevel= TRACE_LOG_LEVEL;	// Debug模式输出所有信息
#else
	m_DefaultLevel= OFF_LOG_LEVEL;		// Release模式下默认关闭所有输出
#endif

	SetLogLevel(m_DefaultLevel);
	m_strPattern = _T("%D{%y-%m-%d %H:%M:%S:%Q}|%t|%-16c|%m%n");
	m_strDebugPattern = _T("%D{%H:%M:%S:%Q}|%t|%-16c|%m%n");;

	m_strLogPath.Format(_T("%s\\Log"),AppGetSystemInfo()->GetModuleDir());
}

Utils::CLogger::~CLogger()
{

}

void Utils::CLogger::InitLogger( )
{
#ifdef _UNICODE
	std::locale::global(std::locale("chs"));
#endif

	// 获取RootLogger并设置，子Logger自动继承
	log4cplus::Logger& rootLogger = log4cplus::Logger::getRoot();
	rootLogger.addAppender(GetAppender());

#ifdef _DEBUG		// 调试版全部输出到控制台
	rootLogger.addAppender(GetDebugAppender());
#endif
}

CString Utils::CLogger::FormatString( LPCTSTR pstrFormat, ... )
{
	int nSize = 0;
	CString strTemp;
	va_list args;
	va_start(args, pstrFormat);
	nSize = _vsntprintf_s( strTemp.GetBufferSetLength(MAX_LOG_SIZE),MAX_LOG_SIZE -1, MAX_LOG_SIZE-2, pstrFormat, args);
	strTemp.ReleaseBufferSetLength(nSize);
	va_end(args);
	return strTemp;
}

void Utils::CLogger::SetLogPath( LPCTSTR lpszLogPath )
{
	m_strLogPath = lpszLogPath;
	m_strLogPath.Append(_T("\\Log"));
}

void Utils::CLogger::LogBy( LogLevel level,LPCTSTR lpInstance,LPCTSTR pstrFormat, ... )
{
	log4cplus::Logger logger = log4cplus::Logger::getInstance(lpInstance);
	if ( logger.isEnabledFor(level))
	{
		int nSize = 0;
		CString strTemp;
		va_list args;
		va_start(args, pstrFormat);
		nSize = _vsntprintf_s( strTemp.GetBufferSetLength(MAX_LOG_SIZE),MAX_LOG_SIZE -1, MAX_LOG_SIZE-2, pstrFormat, args);
		strTemp.ReleaseBufferSetLength(nSize);
		va_end(args);
		logger.forcedLog(level, strTemp.GetString());
	}
	log4cplus::threadCleanup();
}

void Utils::CLogger::Log( LogLevel level, LPCTSTR pstrFormat, ... )
{
	log4cplus::Logger rootLogger = log4cplus::Logger::getRoot();
	if ( rootLogger.isEnabledFor(level))
	{
		int nSize = 0;
		CString strTemp;
		va_list args;
		va_start(args, pstrFormat);
		nSize = _vsntprintf_s( strTemp.GetBufferSetLength(MAX_LOG_SIZE),MAX_LOG_SIZE -1, MAX_LOG_SIZE-2, pstrFormat, args);
		strTemp.ReleaseBufferSetLength(nSize);
		va_end(args);
		rootLogger.forcedLog(level, strTemp.GetString());
	}
	log4cplus::threadCleanup();
}

void Utils::CLogger::Debug( LPCTSTR pstrFormat, ... )
{
	log4cplus::Logger rootLogger = log4cplus::Logger::getRoot();
	if ( rootLogger.isEnabledFor(DEBUG_LOG_LEVEL))
	{
		int nSize = 0;
		CString strTemp;
		va_list args;
		va_start(args, pstrFormat);
		nSize = _vsntprintf_s( strTemp.GetBufferSetLength(MAX_LOG_SIZE),MAX_LOG_SIZE -1, MAX_LOG_SIZE-2, pstrFormat, args);
		strTemp.ReleaseBufferSetLength(nSize);
		va_end(args);
		rootLogger.forcedLog(DEBUG_LOG_LEVEL, strTemp.GetString());
	}
	log4cplus::threadCleanup();
}

void Utils::CLogger::Info( LPCTSTR pstrFormat, ... )
{
	log4cplus::Logger& rootLogger = log4cplus::Logger::getRoot();
	if ( rootLogger.isEnabledFor(INFO_LOG_LEVEL))
	{
		int nSize = 0;
		CString strTemp;
		va_list args;
		va_start(args, pstrFormat);
		nSize = _vsntprintf_s( strTemp.GetBufferSetLength(MAX_LOG_SIZE),MAX_LOG_SIZE -1, MAX_LOG_SIZE-2, pstrFormat, args);
		strTemp.ReleaseBufferSetLength(nSize);
		va_end(args);
		rootLogger.forcedLog(INFO_LOG_LEVEL, strTemp.GetString());
	}
	log4cplus::threadCleanup();
}

void Utils::CLogger::Warn( LPCTSTR pstrFormat, ... )
{
	log4cplus::Logger& rootLogger = log4cplus::Logger::getRoot();
	if ( rootLogger.isEnabledFor(WARN_LOG_LEVEL))
	{
		int nSize = 0;
		CString strTemp;
		va_list args;
		va_start(args, pstrFormat);
		nSize = _vsntprintf_s( strTemp.GetBufferSetLength(MAX_LOG_SIZE),MAX_LOG_SIZE -1, MAX_LOG_SIZE-2, pstrFormat, args);
		strTemp.ReleaseBufferSetLength(nSize);
		va_end(args);
		rootLogger.forcedLog(WARN_LOG_LEVEL, strTemp.GetString());
	}
	log4cplus::threadCleanup();
}

void Utils::CLogger::Error( LPCTSTR pstrFormat, ... )
{
	log4cplus::Logger& rootLogger = log4cplus::Logger::getRoot();
	if ( rootLogger.isEnabledFor(ERROR_LOG_LEVEL))
	{
		int nSize = 0;
		CString strTemp;
		va_list args;
		va_start(args, pstrFormat);
		nSize = _vsntprintf_s( strTemp.GetBufferSetLength(MAX_LOG_SIZE),MAX_LOG_SIZE -1, MAX_LOG_SIZE-2, pstrFormat, args);
		strTemp.ReleaseBufferSetLength(nSize);
		va_end(args);
		rootLogger.forcedLog(ERROR_LOG_LEVEL, strTemp.GetString());
	}
	log4cplus::threadCleanup();
}

void Utils::CLogger::Fatal( LPCTSTR pstrFormat, ... )
{
	log4cplus::Logger& rootLogger = log4cplus::Logger::getRoot();
	if ( rootLogger.isEnabledFor(FATAL_LOG_LEVEL))
	{
		int nSize = 0;
		CString strTemp;
		va_list args;
		va_start(args, pstrFormat);
		nSize = _vsntprintf_s( strTemp.GetBufferSetLength(MAX_LOG_SIZE),MAX_LOG_SIZE -1, MAX_LOG_SIZE-2, pstrFormat, args);
		strTemp.ReleaseBufferSetLength(nSize);
		va_end(args);
		rootLogger.forcedLog(FATAL_LOG_LEVEL, strTemp.GetString());
	}
	log4cplus::threadCleanup();
}

void Utils::CLogger::DebugBy( LPCTSTR lpInstance,LPCTSTR pstrFormat, ... )
{
	log4cplus::Logger& logger = log4cplus::Logger::getInstance(lpInstance);
	if ( logger.isEnabledFor(DEBUG_LOG_LEVEL))
	{
		int nSize = 0;
		CString strTemp;
		va_list args;
		va_start(args, pstrFormat);
		nSize = _vsntprintf_s( strTemp.GetBufferSetLength(MAX_LOG_SIZE),MAX_LOG_SIZE -1, MAX_LOG_SIZE-2, pstrFormat, args);
		strTemp.ReleaseBufferSetLength(nSize);
		va_end(args);
		logger.forcedLog(DEBUG_LOG_LEVEL, strTemp.GetString());
	}
	log4cplus::threadCleanup();
}

void Utils::CLogger::InfoBy( LPCTSTR lpInstance,LPCTSTR pstrFormat, ... )
{
	log4cplus::Logger& logger = log4cplus::Logger::getInstance(lpInstance);
	if ( logger.isEnabledFor(ERROR_LOG_LEVEL))
	{
		int nSize = 0;
		CString strTemp;
		va_list args;
		va_start(args, pstrFormat);
		nSize = _vsntprintf_s( strTemp.GetBufferSetLength(MAX_LOG_SIZE),MAX_LOG_SIZE -1, MAX_LOG_SIZE-2, pstrFormat, args);
		strTemp.ReleaseBufferSetLength(nSize);
		va_end(args);
		logger.forcedLog(INFO_LOG_LEVEL, strTemp.GetString());
	}
	log4cplus::threadCleanup();
}

void Utils::CLogger::WarnBy( LPCTSTR lpInstance,LPCTSTR pstrFormat, ... )
{
	log4cplus::Logger& logger = log4cplus::Logger::getInstance(lpInstance);
	if ( logger.isEnabledFor(ERROR_LOG_LEVEL))
	{
		int nSize = 0;
		CString strTemp;
		va_list args;
		va_start(args, pstrFormat);
		nSize = _vsntprintf_s( strTemp.GetBufferSetLength(MAX_LOG_SIZE),MAX_LOG_SIZE -1, MAX_LOG_SIZE-2, pstrFormat, args);
		strTemp.ReleaseBufferSetLength(nSize);
		va_end(args);
		logger.forcedLog(WARN_LOG_LEVEL, strTemp.GetString());
	}
	log4cplus::threadCleanup();
}

void Utils::CLogger::ErrorBy( LPCTSTR lpInstance,LPCTSTR pstrFormat, ... )
{
	log4cplus::Logger& logger = log4cplus::Logger::getInstance(lpInstance);
	if ( logger.isEnabledFor(ERROR_LOG_LEVEL))
	{
		int nSize = 0;
		CString strTemp;
		va_list args;
		va_start(args, pstrFormat);
		nSize = _vsntprintf_s( strTemp.GetBufferSetLength(MAX_LOG_SIZE),MAX_LOG_SIZE -1, MAX_LOG_SIZE-2, pstrFormat, args);
		strTemp.ReleaseBufferSetLength(nSize);
		va_end(args);
		logger.forcedLog(ERROR_LOG_LEVEL, strTemp.GetString());
	}
	log4cplus::threadCleanup();
}

void Utils::CLogger::FatalBy( LPCTSTR lpInstance,LPCTSTR pstrFormat, ... )
{
	log4cplus::Logger& logger = log4cplus::Logger::getInstance(lpInstance);
	if ( logger.isEnabledFor(FATAL_LOG_LEVEL))
	{
		int nSize = 0;
		CString strTemp;
		va_list args;
		va_start(args, pstrFormat);
		nSize = _vsntprintf_s( strTemp.GetBufferSetLength(MAX_LOG_SIZE),MAX_LOG_SIZE -1, MAX_LOG_SIZE-2, pstrFormat, args);
		strTemp.ReleaseBufferSetLength(nSize);
		va_end(args);
		logger.forcedLog(FATAL_LOG_LEVEL, strTemp.GetString());
	}
	log4cplus::threadCleanup();
}

void Utils::CLogger::SetLogLevel( LogLevel ll )
{
	// 通过设置Root的LogLevel,让所有子对象按照此规则输出
	log4cplus::Logger rootLogger = log4cplus::Logger::getRoot();
	rootLogger.setLogLevel(ll);
}

void Utils::CLogger::SetInternalDebugging( bool bInternalDebugging/*=false*/ )
{
	log4cplus::helpers::LogLog::getLogLog()->setInternalDebugging(bInternalDebugging);
}

void Utils::CLogger::SetQuietMode( bool bQuietMode/*=false*/ )
{
	log4cplus::helpers::LogLog::getLogLog()->setQuietMode(bQuietMode);
}

void Utils::CLogger::SetPatternString( LPCTSTR lpszPattern )
{
	m_strPattern = lpszPattern;
}

void Utils::CLogger::SetDebugPatternString( LPCTSTR lpszPattern )
{
	m_strDebugPattern = lpszPattern;
}

log4cplus::LogLevel Utils::CLogger::GetLogLevel()
{
	return m_DefaultLevel;
}

CString& Utils::CLogger::GetPatternString()
{
	return m_strPattern;
}

CString& Utils::CLogger::GetDebugPatternString()
{
	return m_strDebugPattern;
}

void Utils::CLogger::SetInstanceFilter( VString& filterList )
{
	// 删除Root的Appender，等于不输出任何信息
	log4cplus::Logger& rootLogger = log4cplus::Logger::getRoot();
	rootLogger.removeAllAppenders();

	// 设置要求输出的Instance
	size_t iCount=filterList.size();
	for ( size_t i=0; i<iCount; i++)
	{
		log4cplus::Logger logger = log4cplus::Logger::getInstance(filterList[i].GetString());
		logger.addAppender(GetAppender());
#ifdef _DEBUG
		logger.addAppender(GetDebugAppender());
#endif // _DEBUG
	}
}

log4cplus::SharedAppenderPtr Utils::CLogger::GetAppender()
{
	//////////////////////////////////////////////////////////////////////////
	// 创建 Appender
	SharedAppenderPtr fileAppender(new FileAppender(m_strLogPath.GetString()));
	std::auto_ptr<Layout> pFileLayout(new PatternLayout(m_strPattern.GetString()));
	fileAppender->setLayout(pFileLayout);
	fileAppender->setName(_T("fileAppender"));
	return fileAppender;
}

log4cplus::SharedAppenderPtr Utils::CLogger::GetDebugAppender()
{
	SharedAppenderPtr consoleAppender(new Win32DebugAppender);
	consoleAppender->setName(_T("consoleAppender"));
	std::wstring strConsolePatternLayout	=	_T("%D{%y-%m-%d %H:%M:%S:%Q}|%t|%-16c|%-5p|%m%n");
	std::auto_ptr<Layout> pConsoleLayout(new PatternLayout(m_strDebugPattern.GetString()));
	consoleAppender->setLayout(pConsoleLayout);
	return consoleAppender;
}

void Utils::CLogger::Clear()
{
	// 删除Root的Appender，等于不输出任何信息
	log4cplus::Logger& rootLogger = log4cplus::Logger::getRoot();
	rootLogger.removeAllAppenders();
}

void Utils::CLogger::ClearInstanceFilter()
{
	m_InstanceFilterList.clear();
}

void Utils::CLogger::AddInstanceFilter( LPCTSTR lpszNewFilter )
{
	m_InstanceFilterList.push_back(lpszNewFilter);
}

void Utils::CLogger::UpdateInstanceFilter()
{
	SetInstanceFilter(m_InstanceFilterList);
}

