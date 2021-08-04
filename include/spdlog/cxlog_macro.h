#ifndef _CXLOG_MACRO_H
#define _CXLOG_MACRO_H
#include "spdlog/cxlog.h"

#define MAX_LOG_LEN 10240

//#define CXLogInfo(...)  cxlog::CXLog::Instance().info(__VA_ARGS__)
#define CXLogInfo(logSortId, ...) cxlog::CXLog::Instance().info(logSortId, ##__VA_ARGS__)

//#define CXLogDebug(...)  cxlog::CXLog::Instance().debug(__VA_ARGS__)
#define CXLogDebug(logSortId, ...) cxlog::CXLog::Instance().debug(logSortId, ##__VA_ARGS__)

//#define CXLogWarn(...) cxlog::CXLog::Instance().warn(__VA_ARGS__)
#define CXLogWarn(logSortId, ...) cxlog::CXLog::Instance().warn(logSortId, ##__VA_ARGS__)

//#define CXLogError(...)  cxlog::CXLog::Instance().error(__VA_ARGS__)
#define CXLogError(logSortId, ...) cxlog::CXLog::Instance().error(logSortId, ##__VA_ARGS__)

//#define CXLogCritical(...)  cxlog::CXLog::Instance().critical(__VA_ARGS__)
#define CXLogCritical(logSortId,...)  cxlog::CXLog::Instance().critical(logSortId,##__VA_ARGS__)

#endif // _CXLOG_MACRO_H