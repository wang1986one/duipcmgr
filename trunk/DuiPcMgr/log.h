/** @file
    本文件内定义简化日志记录的宏.
    所有宏的展开均与LOG4CPLUS一样，使用同样的定义开关，只是对Logger的获取及
    所获得的Logger变量的使用做了自动化处理。
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
    @def DECLARE_SUBLOGITEM(SubLogItemName)  声明某模块日志类实例.

    该宏首先声明一个专有的命名空间，在在此命名空间创建一个该模块的Logger类实例，
    并自动使用该命名空间。该宏与其它的LOG_×××宏配合使用，以简化记录日志的操作.
*/

#ifndef LOG4CPLUS_DISABLE_FATAL
#ifdef UNICODE
#define DECLARE_SUBLOGITEM(SubLogItemName) \
	static log4cplus::Logger _rolly_logger = log4cplus::Logger::getInstance(helpers::towstring(#SubLogItemName))
	

//支持class1.module格式,目前对于多模块模式还有一点问题，请使用上面的DECLARE_SUBLOGITEM
#define DECLARE_SUBLOGITEM2(SubLogItemName1, SubLogItemName2) \
	static log4cplus::Logger _rolly_logger = log4cplus::Logger::getInstance(helpers::towstring(#SubLogItemName1)"." helpers::towstring(#SubLogItemName2))

//支持class1.module.submodule格式
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
    @def LOG_TRACE_METHOD(logEvent)  创建一个TraceLogger
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
 * @def LOG_DEBUG(logger, logEvent)  用来记录一个DEBUG_LOG_LEVEL信息的宏
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
 * @def LOG_INFO(logger, logEvent)  用来记录一个INFO_LOG_LEVEL信息的宏
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
 * @def LOG_WARN(logger, logEvent)  用来记录一个WARN_LOG_LEVEL信息的宏
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
 * @def LOG_ERROR(logger, logEvent)  用来记录一个ERROR_LOG_LEVEL信息的宏
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
 * @def LOG_FATAL(logger, logEvent)  用来记录一个FATAL_LOG_LEVEL信息的宏
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

