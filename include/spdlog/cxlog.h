#ifndef _CXLOG_H
#define _CXLOG_H
#include<memory>
#include<string>
#include <spdlog/common.h>

namespace spdlog
{
	class logger;
}

namespace cxlog
{
	class SPDLOG_API CXLog
	{
	public:
		static CXLog& Instance();

		void InitCXLog(std::string file_name="cxslice.log", int maxSize=1024*10, int maxFiles=4, int log_level=0);

		void info(const char* fmt, ...);
		void info(const long long logSortId,const char* fmt,...);

		void error(const char* fmt,...);
		void error(const long long logSortId, const char* fmt, ...);

		void debug(const char* fmt, ...);
		void debug(const long long logSortId, const char* fmt, ...);

		void warn(const char* fmt, ...);
		void warn(const long long logSortId, const char* fmt, ...);

		void critical(const char* fmt, ...);
		void critical(const long long logSortId, const char* fmt, ...);

		void EndLog();

		void SetLevel(int level = 0);
		auto GetLogger()
		{
			return mp_logger_;
		}

	protected:
		CXLog() {};
		~CXLog() {};
		CXLog(const CXLog& other) = delete;
		CXLog& operator=(const CXLog& other) = delete;
	private:
		std::shared_ptr<spdlog::logger> mp_logger_;
		bool hasInitLog = false;
	};
}

#endif