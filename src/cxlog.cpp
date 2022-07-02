#include "spdlog/cxlog.h"
#include <spdlog/common.h>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/android_sink.h"
#include "spdlog/async.h"
#include <stdarg.h>
#include <string>
#include <sstream>
#include <ctime>

#include "ccglobal/platform.h"

#if __ANDROID__
#    include <android/log.h>
#    define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "NativeCC", __VA_ARGS__)
#else
#    define LOGI(...)
#endif

#define MAX_LOG_LEN 10240

#define FORMAT_VAR_ID()                                                                                                                    \
    char buf[MAX_LOG_LEN] = {0};                                                                                                           \
    va_list args;                                                                                                                          \
    va_start(args, fmt);                                                                                                                   \
    vsprintf(buf, fmt, args);                                                                                                              \
    if (logSortId > 0)                                                                                                                     \
    {                                                                                                                                      \
        std::stringstream ss;                                                                                                              \
        ss << "[" << logSortId << "] ";                                                                                                    \
        ss << buf;                                                                                                                         \
        strncpy(buf, ss.str().c_str(), ss.str().length());                                                                                 \
    }                                                                                                                                      \
    va_end(args)

#define FORMAT_VAR()                                                                                                                       \
 long long logSortId = -1;                                                                                                        \
    if (m_logSortId > 0)                                                                                                                       \
         logSortId = m_logSortId;                                                                                                                       \
    FORMAT_VAR_ID()

namespace cxlog 
{
	CXLog &CXLog::Instance()
	{
	    static CXLog cxlog;
	    return cxlog;
	}
	
	void CXLog::InitCXLog(std::string file_name, int maxSize, int maxFiles, int log_level)
	{
	    if (hasInitLog)
	        return;
	    //设置日志等级
	
	    spdlog::set_level(static_cast<spdlog::level::level_enum>(log_level));
	    //设置日志为滚动日志
		LoggerPtr logger = spdlog::rotating_logger_mt("cxlog", file_name, maxSize, maxFiles);
	    //当遇到错误级别以上的马上刷新到日志
		if (logger)
		{
			logger->flush_on(spdlog::level::trace);
			addLogger(logger);
		}
	    //每三秒刷新一次
	    spdlog::flush_every(std::chrono::seconds(3));
	    hasInitLog = true;
	}
	
	void CXLog::setDirectory(const std::string &directory)
	{
		if (hasDirectoryLog)
			return;

	    m_directory = directory;
	    if (m_directory.empty())
	        m_directory = ".";
	    if (_cc_access(m_directory.c_str(), 0))
	    {
	        LOGI("setDirectory error. [%s]", m_directory.c_str());
	        m_directory = ".";
	    }
	
	    checkLogger();
	}
	
	void CXLog::setColorConsole()
	{
		if (hasConsoleLog)
			return;

        spdlog::flush_every(std::chrono::seconds(3));
#if CC_SYSTEM_ANDROID
        LOGI("CXLog::setColorConsole.");
		LoggerPtr logger = spdlog::android_logger_mt("cxlog-console", "NativeCC");
#else
		LoggerPtr logger = spdlog::stdout_color_mt("cxlog", spdlog::color_mode::automatic);
#endif

		if (logger)
		{
			addLogger(logger);
			logger->flush_on(spdlog::level::main);
			hasConsoleLog = true;
		}
	}

	void CXLog::addLogger(LoggerPtr logger)
	{
		m_mutex.lock();
		m_loggers.push_back(logger);
		m_mutex.unlock();
	}

	void CXLog::_log(CXLOG_Level level, const char* msg)
	{
		m_mutex.lock();
		for (LoggerPtr logger : m_loggers)
		{
			switch (level)
			{
			case CXLOG_Level::cxlog_verbose:
				logger->trace(msg);
				break;
			case CXLOG_Level::cxlog_debug:
				logger->debug(msg);
				break;
			case CXLOG_Level::cxlog_info:
				logger->info(msg);
				break;
			case CXLOG_Level::cxlog_warn:
				logger->warn(msg);
				break;
			case CXLOG_Level::cxlog_err:
				logger->error(msg);
				break;
			case CXLOG_Level::cxlog_critical:
				logger->critical(msg);
				break;
			case CXLOG_Level::cxlog_main:
				logger->main(msg);
				break;
			default:
				break;
			}
		}

		m_mutex.unlock();
	}
	
	void CXLog::checkLogger()
	{
	    if (!hasDirectoryLog)
	    {
	        auto f = [](const char *name) -> std::string {
	            time_t n = time(0);
	            tm *t = localtime(&n);
	
	            char buffer[512] = {0};
	            sprintf(buffer, "cxsw3d_%d%02d%02d.text", (1900 + t->tm_year), (1 + t->tm_mon), t->tm_mday);
	            return std::string(buffer);
	        };
	
            spdlog::flush_every(std::chrono::seconds(3));
	        std::string fileName = m_directory + "/" + (nameFunc ? nameFunc("") : f(""));
	
	        LOGI("CXLog::checkLogger [%s]", fileName.c_str());
            errno = 0;

	        LoggerPtr logger = spdlog::basic_logger_mt("cxlog-file", fileName);
	        if(errno != 0)
            {
				logger = nullptr;
				errno = 0;
            }

			if (logger)
			{
				logger->flush_on(spdlog::level::main);
				addLogger(logger);
				hasDirectoryLog = true;
			}
	    }
	}
	
	void CXLog::verbose(const char *fmt, ...)
	{
	    FORMAT_VAR();
		_log(CXLOG_Level::cxlog_verbose, buf);
	}
	
	void CXLog::verbose(const long long logSortId, const char *fmt, ...)
	{
	    FORMAT_VAR_ID();
		_log(CXLOG_Level::cxlog_verbose, buf);
	}
	
	void CXLog::debug(const char *fmt, ...)
	{
	    FORMAT_VAR();
		_log(CXLOG_Level::cxlog_debug, buf);
	}
	
	void CXLog::debug(const long long logSortId, const char *fmt, ...)
	{
	    FORMAT_VAR_ID();
		_log(CXLOG_Level::cxlog_debug, buf);
	}
	
	void CXLog::info(const char *fmt, ...)
	{
	    FORMAT_VAR();
		_log(CXLOG_Level::cxlog_info, buf);
	}
	
	void CXLog::info(const long long logSortId, const char *fmt, ...)
	{
	    FORMAT_VAR_ID();
		_log(CXLOG_Level::cxlog_info, buf);
	}
	
	void CXLog::warn(const char *fmt, ...)
	{
	    FORMAT_VAR();
		_log(CXLOG_Level::cxlog_warn, buf);
	}
	
	void CXLog::warn(const long long logSortId, const char *fmt, ...)
	{
	    FORMAT_VAR_ID();
		_log(CXLOG_Level::cxlog_warn, buf);
	}
	
	void CXLog::error(const char *fmt, ...)
	{
	    FORMAT_VAR();
		_log(CXLOG_Level::cxlog_err, buf);
	}
	
	void CXLog::error(const long long logSortId, const char *fmt, ...)
	{
	    FORMAT_VAR_ID();
		_log(CXLOG_Level::cxlog_err, buf);
	}
	
	void CXLog::critical(const char *fmt, ...)
	{
	    FORMAT_VAR();
		_log(CXLOG_Level::cxlog_critical, buf);
	}
	
	void CXLog::critical(const long long logSortId, const char *fmt, ...)
	{
	    FORMAT_VAR_ID();
		_log(CXLOG_Level::cxlog_critical, buf);
	}
	
	void CXLog::main(const char *fmt, ...)
	{
	    FORMAT_VAR();
		_log(CXLOG_Level::cxlog_main, buf);
	}
	
	void CXLog::main(const long long logSortId, const char *fmt, ...)
	{
	    FORMAT_VAR_ID();
		_log(CXLOG_Level::cxlog_main, buf);
	}
	
	void CXLog::EndLog()
	{
		for (LoggerPtr logger : m_loggers)
		{
			logger->flush();
			logger.reset();
	    }
	    hasInitLog = false;
	    spdlog::shutdown();
	}
	
	void CXLog::SetLevel(int level)
	{
	    if (level < 0)
	        level = 0;
	    if (level > 7)
	        level = 7;
	
	    spdlog::set_level(static_cast<spdlog::level::level_enum>(level));
	}
	
	void CXLog::setNameFunc(logNameFunc func)
	{
	    nameFunc = func;
	}
} // namespace cxlog
