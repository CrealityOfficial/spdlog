#ifndef _CXLOG_MACRO_H
#define _CXLOG_MACRO_H
#include "spdlog/cxlog.h"

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

#define CXLogVerbose(...) cxlog::CXLog::Instance().verbose(__VA_ARGS__)
#define CXLogVerbose(logSortId, ...) cxlog::CXLog::Instance().verbose(logSortId, ##__VA_ARGS__)

#define CXLogDebug(...) cxlog::CXLog::Instance().debug(__VA_ARGS__)
#define CXLogDebug(logSortId, ...) cxlog::CXLog::Instance().debug(logSortId, ##__VA_ARGS__)

#define CXLogInfo(...)  cxlog::CXLog::Instance().info(__VA_ARGS__)
#define CXLogInfo(logSortId, ...) cxlog::CXLog::Instance().info(logSortId, ##__VA_ARGS__)

#define CXLogWarn(...) cxlog::CXLog::Instance().warn(__VA_ARGS__)
#define CXLogWarn(logSortId, ...) cxlog::CXLog::Instance().warn(logSortId, ##__VA_ARGS__)

#define CXLogError(...)  cxlog::CXLog::Instance().error(__VA_ARGS__)
#define CXLogError(logSortId, ...) cxlog::CXLog::Instance().error(logSortId, ##__VA_ARGS__)

#define CXLogCritical(...)  cxlog::CXLog::Instance().critical(__VA_ARGS__)
#define CXLogCritical(logSortId,...)  cxlog::CXLog::Instance().critical(logSortId,##__VA_ARGS__)

#define CXLogMain(...) cxlog::CXLog::Instance().main(__VA_ARGS__)
#define CXLogMain(logSortId, ...) cxlog::CXLog::Instance().main(logSortId, ##__VA_ARGS__)
#endif // _CXLOG_MACRO_H