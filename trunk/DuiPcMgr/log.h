/** @file
    ���ļ��ڶ������־��¼�ĺ�.
    ���к��չ������LOG4CPLUSһ����ʹ��ͬ���Ķ��忪�أ�ֻ�Ƕ�Logger�Ļ�ȡ��
    ����õ�Logger������ʹ�������Զ�������
	this file is edit by shaoyuan
	note:shaoyuan1943@gmail.com
*/
#pragma once;
#ifndef _XXLIB_LOGGING_MACROS_HEADER_
#define _XXLIB_LOGGING_MACROS_HEADER_

#include "log4cplus\logger.h"
#include "log4cplus\fileappender.h"
#include "log4cplus\consoleappender.h"
#include "log4cplus\configurator.h"
#include "log4cplus\layout.h"
#include "log4cplus\loggingmacros.h"

using namespace log4cplus;
using namespace log4cplus::helpers;
#if defined(LOG4CPLUS_DISABLE_FATAL) && !defined(LOG4CPLUS_DISABLE_ERROR)
#define LOG4CPLUS_DISABLE_ERROR
#endif
#if defined(LOG4CPLUS_DISABLE_ERROR) && !defined(LOG4CPLUS_DISABLE_WARN)
#define LOG4CPLUS_DISABLE_WARN
#endif
#if defined(LOG4CPLUS_DISABLE_WARN) && !defined(LOG4CPLUS_DISABLE_INFO)
#define LOG4CPLUS_DISABLE_INFO
#endif
#if defined(LOG4CPLUS_DISABLE_INFO) && !defined(LOG4CPLUS_DISABLE_DEBUG)
#define LOG4CPLUS_DISABLE_DEBUG
#endif
#if defined(LOG4CPLUS_DISABLE_DEBUG) && !defined(LOG4CPLUS_DISABLE_TRACE)
#define LOG4CPLUS_DISABLE_TRACE
#endif
/**
    @def DECLARE_SUBLOGITEM(SubLogItemName)  ����ĳģ����־��ʵ��.

    �ú���������һ��ר�е������ռ䣬���ڴ������ռ䴴��һ����ģ���Logger��ʵ����
    ���Զ�ʹ�ø������ռ䡣�ú���������LOG_�����������ʹ�ã��Լ򻯼�¼��־�Ĳ���.
*/

#ifndef LOG4CPLUS_DISABLE_FATAL
#ifdef UNICODE
#define DECLARE_SUBLOGITEM(SubLogItemName) \
	static log4cplus::Logger _rolly_logger = log4cplus::Logger::getInstance(helpers::towstring(#SubLogItemName))
	

//֧��class1.module��ʽ,Ŀǰ���ڶ�ģ��ģʽ����һ�����⣬��ʹ�������DECLARE_SUBLOGITEM
#define DECLARE_SUBLOGITEM2(SubLogItemName1, SubLogItemName2) \
	static log4cplus::Logger _rolly_logger = log4cplus::Logger::getInstance(helpers::towstring(#SubLogItemName1)"." helpers::towstring(#SubLogItemName2))

//֧��class1.module.submodule��ʽ
#define DECLARE_SUBLOGITEM3(SubLogItemName1, SubLogItemName2, SubLogItemName3) \
	static log4cplus::Logger _rolly_logger = log4cplus::Logger::getInstance(helpers::towstring(#SubLogItemName1)"." helpers::towstring(#SubLogItemName2)"." helpers::towstring(#SubLogItemName3))
#else
#define DECLARE_SUBLOGITEM(SubLogItemName) \
	static log4cplus::Logger _rolly_logger = log4cplus::Logger::getInstance(#SubLogItemName)
#define DECLARE_SUBLOGITEM2(SubLogItemName1,SubLogItemName2) \
	static log4cplus::Logger _rolly_logger = log4cplus::Logger::getInstance(#SubLogItemName1"."#SubLogItemName2)
#define DECLARE_SUBLOGITEM3(SubLogItemName1,SubLogItemName2,SubLogItemName3) \
	static log4cplus::Logger _rolly_logger = log4cplus::Logger::getInstance(#SubLogItemName1"."#SubLogItemName2"."#SubLogItemName)
#endif
#else
#define DECLARE_SUBLOGITEM(SubLogItemName)
#define DECLARE_SUBLOGITEM2(SubLogItemName1, SubLogItemName2)
#define DECLARE_SUBLOGITEM3(SubLogItemName1, SubLogItemName2, SubLogItemName3)
#endif

/**
    @def LOG_TRACE_METHOD(logEvent)  ����һ��TraceLogger
	__FUNCTION__:const char[]
 */
#if !defined(LOG4CPLUS_DISABLE_TRACE)
#define LOG_TRACE_METHOD() \
    log4cplus::TraceLogger _log4cplus_trace_logger(_rolly_logger, helpers::towstring(__FUNCTION__), __FILE__, __LINE__)
#define LOG_TRACE(logEvent) \
    do { \
        if(_rolly_logger.isEnabledFor(log4cplus::TRACE_LOG_LEVEL)) { \
            log4cplus::tostringstream _log4cplus_buf; \
            _log4cplus_buf << logEvent; \
            _rolly_logger.forcedLog(log4cplus::TRACE_LOG_LEVEL, _log4cplus_buf.str(), __FILE__, __LINE__); \
        } \
    } while(0)
#define LOG_TRACE_STR(logEvent) \
    do { \
        if(_rolly_logger.isEnabledFor(log4cplus::TRACE_LOG_LEVEL)) { \
            _rolly_logger.forcedLog(log4cplus::TRACE_LOG_LEVEL, logEvent, __FILE__, __LINE__); \
        } \
    } while(0)
#else
#define LOG_TRACE_METHOD()
#define LOG_TRACE(logEvent)
#define LOG_TRACE_STR(logEvent)
#endif

/**
 * @def LOG_DEBUG(logger, logEvent)  ������¼һ��DEBUG_LOG_LEVEL��Ϣ�ĺ�
 */
#if !defined(LOG4CPLUS_DISABLE_DEBUG)
#define LOG_DEBUG(logEvent) \
    do { \
        if(_rolly_logger.isEnabledFor(log4cplus::DEBUG_LOG_LEVEL)) { \
            log4cplus::tostringstream _log4cplus_buf; \
            _log4cplus_buf << logEvent; \
            _rolly_logger.forcedLog(log4cplus::DEBUG_LOG_LEVEL, _log4cplus_buf.str(), __FILE__, __LINE__); \
        } \
    } while(0)
#define LOG_DEBUG_STR(logEvent) \
    do { \
        if(_rolly_logger.isEnabledFor(log4cplus::DEBUG_LOG_LEVEL)) { \
            _rolly_logger.forcedLog(log4cplus::DEBUG_LOG_LEVEL, logEvent, __FILE__, __LINE__); \
        } \
    } while(0)
#else
#define LOG_DEBUG(logEvent)
#define LOG_DEBUG_STR(logEvent)
#endif

/**
 * @def LOG_INFO(logger, logEvent)  ������¼һ��INFO_LOG_LEVEL��Ϣ�ĺ�
 */
#if !defined(LOG4CPLUS_DISABLE_INFO)
#define LOG_INFO(logEvent) \
    do { \
        if(_rolly_logger.isEnabledFor(log4cplus::INFO_LOG_LEVEL)) { \
            log4cplus::tostringstream _log4cplus_buf; \
            _log4cplus_buf << logEvent; \
            _rolly_logger.forcedLog(log4cplus::INFO_LOG_LEVEL, _log4cplus_buf.str(), __FILE__, __LINE__); \
        } \
    } while(0)
#define LOG_INFO_STR(logEvent) \
    do { \
        if(_rolly_logger.isEnabledFor(log4cplus::INFO_LOG_LEVEL)) { \
            _rolly_logger.forcedLog(log4cplus::INFO_LOG_LEVEL, logEvent, __FILE__, __LINE__); \
        } \
    } while(0)
#else
#define LOG_INFO(logEvent)
#define LOG_INFO_STR(logEvent)
#endif

/**
 * @def LOG_WARN(logger, logEvent)  ������¼һ��WARN_LOG_LEVEL��Ϣ�ĺ�
 */
#if !defined(LOG4CPLUS_DISABLE_WARN)
#define LOG_WARN(logEvent) \
    do { \
        if(_rolly_logger.isEnabledFor(log4cplus::WARN_LOG_LEVEL)) { \
            log4cplus::tostringstream _log4cplus_buf; \
            _log4cplus_buf << logEvent; \
            _rolly_logger.forcedLog(log4cplus::WARN_LOG_LEVEL, _log4cplus_buf.str(), __FILE__, __LINE__); \
        } \
    } while(0)
#define LOG_WARN_STR(logEvent) \
    do { \
        if(_rolly_logger.isEnabledFor(log4cplus::WARN_LOG_LEVEL)) { \
            _rolly_logger.forcedLog(log4cplus::WARN_LOG_LEVEL, logEvent, __FILE__, __LINE__); \
        } \
    } while(0)
#else
#define LOG_WARN(logEvent)
#define LOG_WARN_STR(logEvent)
#endif

/**
 * @def LOG_ERROR(logger, logEvent)  ������¼һ��ERROR_LOG_LEVEL��Ϣ�ĺ�
 */
#if !defined(LOG4CPLUS_DISABLE_ERROR)
#define LOG_ERROR(logEvent) \
    do { \
        if(_rolly_logger.isEnabledFor(log4cplus::ERROR_LOG_LEVEL)) { \
            log4cplus::tostringstream _log4cplus_buf; \
            _log4cplus_buf << logEvent; \
            _rolly_logger.forcedLog(log4cplus::ERROR_LOG_LEVEL, _log4cplus_buf.str(), __FILE__, __LINE__); \
        } \
    } while(0)
#define LOG_ERROR_STR(logEvent) \
    do { \
        if(_rolly_logger.isEnabledFor(log4cplus::ERROR_LOG_LEVEL)) { \
            _rolly_logger.forcedLog(log4cplus::ERROR_LOG_LEVEL, logEvent, __FILE__, __LINE__); \
        } \
    } while(0)
#else
#define LOG_ERROR(logEvent)
#define LOG_ERROR_STR(logEvent)
#endif

/**
 * @def LOG_FATAL(logger, logEvent)  ������¼һ��FATAL_LOG_LEVEL��Ϣ�ĺ�
 */
#if !defined(LOG4CPLUS_DISABLE_FATAL)
#define LOG_FATAL(logEvent) \
    do { \
        if(_rolly_logger.isEnabledFor(log4cplus::FATAL_LOG_LEVEL)) { \
            log4cplus::tostringstream _log4cplus_buf; \
            _log4cplus_buf << logEvent; \
            _rolly_logger.forcedLog(log4cplus::FATAL_LOG_LEVEL, _log4cplus_buf.str(), __FILE__, __LINE__); \
        } \
    } while(0)
#define LOG_FATAL_STR(logEvent) \
    do { \
        if(_rolly_logger.isEnabledFor(log4cplus::FATAL_LOG_LEVEL)) { \
            _rolly_logger.forcedLog(log4cplus::FATAL_LOG_LEVEL, logEvent, __FILE__, __LINE__); \
        } \
    } while(0)
#else
#define LOG_FATAL(logEvent)
#define LOG_FATAL_STR(logEvent)
#endif

#endif /* _XXLIB_LOGGING_MACROS_HEADER_ */

