#include "spdlog/cxlog_macro.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/async.h"
#include <stdarg.h> 
#include <string>
#include <sstream>
#include <ctime>
#include <stdio.h>
#include <io.h>

#define MAX_LOG_LEN 10240

#define FORMAT_VAR_ID() \
char buf[MAX_LOG_LEN] = {0}; \
va_list args; \
va_start(args, fmt); \
vsprintf(buf, fmt, args); \
if (logSortId > 0) \
{                  \
    std::stringstream ss;   \
    ss << "[" << logSortId << "] ";  \
    ss << buf;   \
    strncpy(buf, ss.str().c_str(), ss.str().length()); \
}    \
va_end(args)

#define FORMAT_VAR() \
const long long logSortId = -1; \
FORMAT_VAR_ID()

namespace cxlog 
{
	CXLog& CXLog::Instance()
	{
		static CXLog cxlog;
		return cxlog;
	}

	void CXLog::InitCXLog(std::string file_name, int maxSize, int maxFiles,int log_level)
	{
		if (hasInitLog)return;
		//设置日志等级

		spdlog::set_level(static_cast<spdlog::level::level_enum>(log_level));
		//设置日志为滚动日志
		mp_logger_ =spdlog::rotating_logger_mt("cxlog", file_name,maxSize,maxFiles);
		//当遇到错误级别以上的马上刷新到日志
		mp_logger_->flush_on(spdlog::level::trace);
		//每三秒刷新一次
		spdlog::flush_every(std::chrono::seconds(3));
		hasInitLog = true;
	}

	void CXLog::setDirectory(const std::string &directory)
	{
		m_directory = directory;
		if(m_directory.empty())
			m_directory = ".";
        if (access(m_directory.c_str(), 0))
            m_directory = ".";

		checkLogger();
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
			time_t n = time(0);
            tm *t = gmtime(&n);
			
			char buffer[64] = {0};
            sprintf(buffer, "%d-%d-%d.cxlog", t->tm_year, t->tm_mon, t->tm_mday);

			std::string fileName = m_directory + "/" + buffer;
            mp_logger_ = spdlog::basic_logger_mt("cxlog", fileName);
            spdlog::set_level(static_cast<spdlog::level::level_enum>(spdlog::level::warn));
			mp_logger_->flush_on(spdlog::level::main);
			spdlog::flush_every(std::chrono::seconds(3));
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

	void CXLog::info(const long long logSortId,const char* fmt, ...)
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

	void CXLog::error(const char* fmt, ...)
	{
		if (!checkLogger())
		    return;
		
		FORMAT_VAR();
		mp_logger_->error(buf);
	}

	void CXLog::error(const long long logSortId, const char* fmt, ...)
	{
		if (!checkLogger())
		    return;
		
		FORMAT_VAR_ID();
		mp_logger_->warn(buf);
	}

	void CXLog::critical(const char* fmt, ...)
	{
		if (!checkLogger())
		    return;
		
		FORMAT_VAR();
        mp_logger_->critical(buf);
	}

	void CXLog::critical(const long long logSortId, const char* fmt, ...)
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

		if(level < 0)
			level = 0;
		if(level > 7)
			level = 7;

		spdlog::set_level(static_cast<spdlog::level::level_enum>(level));
	}
}



