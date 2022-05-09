#include "spdlog/cxlog.h"
#include <spdlog/common.h>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
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
    const long long logSortId = -1;                                                                                                        \
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
	    mp_logger_ = spdlog::rotating_logger_mt("cxlog", file_name, maxSize, maxFiles);
	    //当遇到错误级别以上的马上刷新到日志
	    mp_logger_->flush_on(spdlog::level::trace);
	    //每三秒刷新一次
	    spdlog::flush_every(std::chrono::seconds(3));
	    hasInitLog = true;
	}
	
	void CXLog::setDirectory(const std::string &directory)
	{
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
        spdlog::set_level(static_cast<spdlog::level::level_enum>(spdlog::level::warn));
        spdlog::flush_every(std::chrono::seconds(3));
        mp_logger_ = spdlog::stdout_color_mt("cxlog", spdlog::color_mode::automatic);
		if(mp_logger_)
			mp_logger_->flush_on(spdlog::level::main);
		hasInitLog = true;
	}
	
	bool CXLog::checkLogger()
	{
	    if (hasInitLog)
	    {
	        if (mp_logger_)
	            return true;
	        return false;
	    }
	    if (!mp_logger_)
	    {
	        auto f = [](const char *name) -> std::string {
	            time_t n = time(0);
	            tm *t = localtime(&n);
	
	            char buffer[64] = {0};
	            sprintf(buffer, "cxsw3d_%d%02d%02d.text", (1900 + t->tm_year), (1 + t->tm_mon), t->tm_mday);
	            return std::string(buffer);
	        };
	
			spdlog::set_level(static_cast<spdlog::level::level_enum>(spdlog::level::warn));
            spdlog::flush_every(std::chrono::seconds(3));
	        std::string fileName = m_directory + "/" + (nameFunc ? nameFunc("") : f(""));
	
	        LOGI("CXLog::checkLogger [%s]", fileName.c_str());
	        mp_logger_ = spdlog::basic_logger_mt("cxlog", fileName);
	        LOGI("CXLog::basic_logger_mt end.");
			if(mp_logger_)
				mp_logger_->flush_on(spdlog::level::main);
	        LOGI("CXLog::checkLogger end.");
	        hasInitLog = true;
	    }
	
	    if (mp_logger_)
	    {
	        return true;
	    }
	    return false;
	}
	
	void CXLog::verbose(const char *fmt, ...)
	{
	    if (!checkLogger())
	        return;
	
	    FORMAT_VAR();
	    mp_logger_->trace(buf);
	}
	
	void CXLog::verbose(const long long logSortId, const char *fmt, ...)
	{
	    if (!checkLogger())
	        return;
	
	    FORMAT_VAR_ID();
	    mp_logger_->trace(buf);
	}
	
	void CXLog::debug(const char *fmt, ...)
	{
	    if (!checkLogger())
	        return;
	
	    FORMAT_VAR();
	    mp_logger_->debug(buf);
	}
	
	void CXLog::debug(const long long logSortId, const char *fmt, ...)
	{
	    if (!checkLogger())
	        return;
	
	    FORMAT_VAR_ID();
	    mp_logger_->debug(buf);
	}
	
	void CXLog::info(const char *fmt, ...)
	{
	    if (!checkLogger())
	        return;
	
	    FORMAT_VAR();
	    mp_logger_->info(buf);
	}
	
	void CXLog::info(const long long logSortId, const char *fmt, ...)
	{
	    if (!checkLogger())
	        return;
	
	    FORMAT_VAR_ID();
	    mp_logger_->info(buf);
	}
	
	void CXLog::warn(const char *fmt, ...)
	{
	    if (!checkLogger())
	        return;
	
	    FORMAT_VAR();
	    mp_logger_->warn(buf);
	}
	
	void CXLog::warn(const long long logSortId, const char *fmt, ...)
	{
	    if (!checkLogger())
	        return;
	
	    FORMAT_VAR_ID();
	    mp_logger_->warn(buf);
	}
	
	void CXLog::error(const char *fmt, ...)
	{
	    if (!checkLogger())
	        return;
	
	    FORMAT_VAR();
	    mp_logger_->error(buf);
	}
	
	void CXLog::error(const long long logSortId, const char *fmt, ...)
	{
	    if (!checkLogger())
	        return;
	
	    FORMAT_VAR_ID();
	    mp_logger_->warn(buf);
	}
	
	void CXLog::critical(const char *fmt, ...)
	{
	    if (!checkLogger())
	        return;
	
	    FORMAT_VAR();
	    mp_logger_->critical(buf);
	}
	
	void CXLog::critical(const long long logSortId, const char *fmt, ...)
	{
	    if (!checkLogger())
	        return;
	
	    FORMAT_VAR_ID();
	    mp_logger_->critical(buf);
	}
	
	void CXLog::main(const char *fmt, ...)
	{
	    if (!checkLogger())
	        return;
	
	    FORMAT_VAR();
	    mp_logger_->main(buf);
	}
	
	void CXLog::main(const long long logSortId, const char *fmt, ...)
	{
	    if (!checkLogger())
	        return;
	
	    FORMAT_VAR_ID();
	    mp_logger_->main(buf);
	}
	
	void CXLog::EndLog()
	{
	    mp_logger_.reset();
	    hasInitLog = false;
	    spdlog::shutdown();
	}
	
	void CXLog::SetLevel(int level)
	{
	    checkLogger();
	
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
