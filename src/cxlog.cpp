#include "spdlog/cxlog_macro.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/async.h"
#include <stdarg.h> 
#include<string>
#include<sstream>

namespace cxlog {

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

	void CXLog::info(const long long logSortId,const char* fmt, ...)
	{
		if (!mp_logger_)
		{
			return;
		}

		//暂无法传递可变参数到spdloger,先解析完再传
		char buf[MAX_LOG_LEN] = { 0 };
		va_list args;
		va_start(args, fmt);
		vsprintf(buf, fmt, args);
		if (logSortId > 0)//日志分类ID
		{
			std::stringstream ss;
			ss << "[" << logSortId << "] ";
			ss << buf;
			strncpy(buf, ss.str().c_str(), ss.str().length());
		}

		mp_logger_->info(buf);
		va_end(args);
	}
	void CXLog::info(const char* fmt, ...)
	{
            if (!mp_logger_)
            {
                return;
            }
		//暂无法传递可变参数到spdloger,先解析完再传
		char buf[MAX_LOG_LEN] = { 0 };
		va_list args;
		va_start(args, fmt);
		vsprintf(buf, fmt, args);
		mp_logger_->info(buf);
		va_end(args);
	}
	void CXLog::error(const char* fmt, ...)
	{
            if (!mp_logger_)
            {
                return;
            }
		char buf[MAX_LOG_LEN] = { 0 };
		va_list args;
		va_start(args, fmt);
		vsprintf(buf, fmt, args);
		mp_logger_->error(buf);
		va_end(args);
	}
	void CXLog::error(const long long logSortId, const char* fmt, ...)
	{
            if (!mp_logger_)
            {
                return;
            }
		char buf[MAX_LOG_LEN] = { 0 };
		va_list args;
		va_start(args, fmt);
		vsprintf(buf, fmt, args);
		if (logSortId > 0)//日志分类ID
		{
			std::stringstream ss;
			ss << "[" << logSortId << "] ";
			ss << buf;
			strncpy(buf, ss.str().c_str(), ss.str().length());
		}
		mp_logger_->error(buf);
		va_end(args);
	}
	void CXLog::debug(const char* fmt, ...)
	{
            if (!mp_logger_)
            {
                return;
            }
		char buf[MAX_LOG_LEN] = { 0 };
		va_list args;
		va_start(args, fmt);
		vsprintf(buf, fmt, args);
		mp_logger_->debug(buf);
		va_end(args);
	}
	void CXLog::debug(const long long logSortId, const char* fmt, ...)
	{
            if (!mp_logger_)
            {
                return;
            }
		char buf[MAX_LOG_LEN] = { 0 };
		va_list args;
		va_start(args, fmt);
		vsprintf(buf, fmt, args);
		if (logSortId > 0)//日志分类ID
		{
			std::stringstream ss;
			ss << "[" << logSortId << "] ";
			ss << buf;
			strncpy(buf, ss.str().c_str(), ss.str().length());
		}
		mp_logger_->debug(buf);
		va_end(args);
	}
	void CXLog::warn(const char* fmt, ...)
	{
            if (!mp_logger_)
            {
                return;
            }
		char buf[MAX_LOG_LEN] = { 0 };
		va_list args;
		va_start(args, fmt);
		vsprintf(buf, fmt, args);
		mp_logger_->warn(buf);
		va_end(args);
	}
	void CXLog::warn(const long long logSortId, const char* fmt, ...)
	{
            if (!mp_logger_)
            {
                return;
            }
		char buf[MAX_LOG_LEN] = { 0 };
		va_list args;
		va_start(args, fmt);
		vsprintf(buf, fmt, args);
		if (logSortId > 0)//日志分类ID
		{
			std::stringstream ss;
			ss << "[" << logSortId << "] ";
			ss << buf;
			strncpy(buf, ss.str().c_str(), ss.str().length());
		}
		mp_logger_->warn(buf);
		va_end(args);
	}

	void CXLog::critical(const char* fmt, ...)
	{
            if (!mp_logger_)
            {
                return;
            }
		char buf[MAX_LOG_LEN] = { 0 };
		va_list args;
		va_start(args, fmt);
		vsprintf(buf, fmt, args);
		mp_logger_->critical(buf);
		va_end(args);
	}

	void CXLog::critical(const long long logSortId, const char* fmt, ...)
	{
        if (!mp_logger_)
        {
            return;
        }
		char buf[MAX_LOG_LEN] = { 0 };
		va_list args;
		va_start(args, fmt);
		vsprintf(buf, fmt, args);
		if (logSortId > 0)//日志分类ID
		{
			std::stringstream ss;
			ss << "[" << logSortId << "] ";
			ss << buf;
			strncpy(buf, ss.str().c_str(), ss.str().length());
		}
		mp_logger_->critical(buf);
		va_end(args);
	}
	void CXLog::EndLog()
	{
		spdlog::shutdown();
	}
	void CXLog::SetLevel(int level)
	{
		spdlog::set_level(static_cast<spdlog::level::level_enum>(level));
	}
}



