#include "spdlog/cxlog.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_sinks.h"
#include "spdlog/async.h"
#include <stdarg.h>
#include <iostream>

#define MAX_LOG_LEN 128
namespace cxlog 
{
	CXLog &CXLog::Instance()
	{
	    static CXLog cxlog;
	    return cxlog;
	}
	
	CXLog::CXLog() 
	{
		mp_logger_ = spdlog::stdout_logger_mt("console");
	}
	
	CXLog::~CXLog() 
	{
	}
	
	void CXLog::InitCXLog(std::string file_name, int maxSize, int maxFiles, int log_level)
	{
	    if (hasInitLog)
	        return;
	    //������־�ȼ�
	    spdlog::set_level(static_cast<spdlog::level::level_enum>(log_level));
	    //������־Ϊ������־
	    mp_logger_ = spdlog::rotating_logger_mt("cxlog", file_name, maxSize, maxFiles);
	    //���������󼶱����ϵ�����ˢ�µ���־
	    mp_logger_->flush_on(spdlog::level::info);
	    //ÿ����ˢ��һ��
	    spdlog::flush_every(std::chrono::seconds(3));
	    hasInitLog = true;
	}
	
	void CXLog::info(const char *fmt, ...)
	{
	    //���޷����ݿɱ������spdloger,�Ƚ������ٴ�
	    char buf[MAX_LOG_LEN] = {0};
	    va_list args;
	    va_start(args, fmt);
	    vsprintf(buf, fmt, args);
	    mp_logger_->info(buf);
	    va_end(args);
	}
	
	void CXLog::error(const char *fmt, ...)
	{
	    char buf[MAX_LOG_LEN] = {0};
	    va_list args;
	    va_start(args, fmt);
	    vsprintf(buf, fmt, args);
	    mp_logger_->error(buf);
	    va_end(args);
	}
	
	void CXLog::debug(const char *fmt, ...)
	{
	    char buf[MAX_LOG_LEN] = {0};
	    va_list args;
	    va_start(args, fmt);
	    vsprintf(buf, fmt, args);
	    mp_logger_->debug(buf);
	    va_end(args);
	}
	
	void CXLog::warn(const char *fmt, ...)
	{
	    char buf[MAX_LOG_LEN] = {0};
	    va_list args;
	    va_start(args, fmt);
	    vsprintf(buf, fmt, args);
	    mp_logger_->warn(buf);
	    va_end(args);
	}
	
	void CXLog::critical(const char *fmt, ...)
	{
	    char buf[MAX_LOG_LEN] = {0};
	    va_list args;
	    va_start(args, fmt);
	    vsprintf(buf, fmt, args);
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

} // namespace cxlog
