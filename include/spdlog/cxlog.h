#ifndef _CXLOG_H
#define _CXLOG_H
#include "ccglobal/export.h"
#include <memory>
#include <string>
#include <functional>
#include <mutex>
#include <vector>

enum class CXLOG_Level
{
	cxlog_verbose = 0,
	cxlog_debug = 1,
	cxlog_info = 2,
	cxlog_warn = 3,
	cxlog_err = 4,
	cxlog_critical = 5,
	cxlog_main = 6    //main log, not error, but level is high  
};

//#ifdef CXLOG_DLL
//#    define CXLOG_API CC_DECLARE_EXPORT
//#else
//#    define CXLOG_API CC_DECLARE_IMPORT
//#endif

#if USE_CXLOG_DLL
#    define CXLOG_API CC_DECLARE_IMPORT
#elif USE_CXBIN_STATIC
#    define CXLOG_API CC_DECLARE_STATIC
#else
#ifdef CXLOG_DLL
#    define CXLOG_API CC_DECLARE_EXPORT
#else
#        define CXLOG_API CC_DECLARE_STATIC
#endif
#endif

namespace spdlog
{
	class logger;
}

typedef std::shared_ptr<spdlog::logger> LoggerPtr;
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
        void setLogSortId(long long logSortId) {m_logSortId = logSortId;}
	protected:
		CXLog() {};
		~CXLog() {};
		CXLog(const CXLog& other) = delete;
		CXLog& operator=(const CXLog& other) = delete;

		void checkLogger();
		void addLogger(LoggerPtr logger);
		void _log(CXLOG_Level level, const char* msg);
	private:
		std::vector<LoggerPtr> m_loggers;
		bool hasInitLog = false;

		bool hasDirectoryLog = false;
		bool hasConsoleLog = false;
		std::string m_directory = ".";
        logNameFunc nameFunc;

		std::mutex m_mutex;
		long long m_logSortId = 0;
	};
}

#endif