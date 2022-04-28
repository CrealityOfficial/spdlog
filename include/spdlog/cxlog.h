#ifndef _CXLOG_H
#define _CXLOG_H
#include "ccglobal/export.h"
#include <memory>
#include <string>
#include <functional>

#ifdef CXLOG_DLL
#    define CXLOG_API CC_DECLARE_EXPORT
#else
#    define CXLOG_API CC_DECLARE_IMPORT
#endif

namespace spdlog
{
	class logger;
}

typedef std::function<std::string(const char*)> logNameFunc;
namespace cxlog
{
	class CXLOG_API CXLog
	{
	public:
		static CXLog& Instance();

		void InitCXLog(std::string file_name="cxslice.log", int maxSize=1024*10, int maxFiles=4, int log_level=0);
		void setDirectory(const std::string& directory);
        void setColorConsole();

		void verbose(const char *fmt, ...);
        void verbose(const long long logSortId, const char *fmt, ...);

		void debug(const char *fmt, ...);
        void debug(const long long logSortId, const char *fmt, ...);

		void info(const char* fmt, ...);
		void info(const long long logSortId,const char* fmt,...);

		void warn(const char *fmt, ...);
		void warn(const long long logSortId, const char *fmt, ...);

		void error(const char* fmt,...);
		void error(const long long logSortId, const char* fmt, ...);

		void critical(const char* fmt, ...);
		void critical(const long long logSortId, const char* fmt, ...);

		void main(const char *fmt, ...);
        void main(const long long logSortId, const char *fmt, ...);

		void EndLog();

		void SetLevel(int level = 0);
        void setNameFunc(logNameFunc func);
	protected:
		CXLog() {};
		~CXLog() {};
		CXLog(const CXLog& other) = delete;
		CXLog& operator=(const CXLog& other) = delete;

		bool checkLogger();
	private:
		std::shared_ptr<spdlog::logger> mp_logger_;
		bool hasInitLog = false;

		std::string m_directory = ".";
        logNameFunc nameFunc;
	};
}

#endif