#ifndef _CXLOG_MACRO_H
#define _CXLOG_MACRO_H
#include "spdlog/cxlog.h"

#define CXLogVerbose(...) cxlog::CXLog::Instance().verbose(__VA_ARGS__)
#define CXLogVerboseID(logSortId, ...) cxlog::CXLog::Instance().verbose(logSortId, ##__VA_ARGS__)

#define CXLogDebug(...) cxlog::CXLog::Instance().debug(__VA_ARGS__)
#define CXLogDebugID(logSortId, ...) cxlog::CXLog::Instance().debug(logSortId, ##__VA_ARGS__)

#define CXLogInfo(...)  cxlog::CXLog::Instance().info(__VA_ARGS__)
#define CXLogInfoID(logSortId, ...) cxlog::CXLog::Instance().info(logSortId, ##__VA_ARGS__)

#define CXLogWarn(...) cxlog::CXLog::Instance().warn(__VA_ARGS__)
#define CXLogWarnID(logSortId, ...) cxlog::CXLog::Instance().warn(logSortId, ##__VA_ARGS__)

#define CXLogError(...)  cxlog::CXLog::Instance().error(__VA_ARGS__)
#define CXLogErrorID(logSortId, ...) cxlog::CXLog::Instance().error(logSortId, ##__VA_ARGS__)

#define CXLogCritical(...)  cxlog::CXLog::Instance().critical(__VA_ARGS__)
#define CXLogCriticalID(logSortId, ...) cxlog::CXLog::Instance().critical(logSortId, ##__VA_ARGS__)

#define CXLogMain(...) cxlog::CXLog::Instance().main(__VA_ARGS__)
#define CXLogMainID(logSortId, ...) cxlog::CXLog::Instance().main(logSortId, ##__VA_ARGS__)
#endif // _CXLOG_MACRO_H