#ifndef _CXLOG_MACRO_H
#define _CXLOG_MACRO_H
#include "spdlog/cxlog.h"

#define CXLogInfo(...)  cxlog::CXLog::Instance().info(__VA_ARGS__)
#define CXLogDebug(...)  cxlog::CXLog::Instance().debug(__VA_ARGS__)
#define CXLogWarn(...) cxlog::CXLog::Instance().warn(__VA_ARGS__)
#define CXLogError(...)  cxlog::CXLog::Instance().error(__VA_ARGS__)
#define CXLogCritical(...)  cxlog::CXLog::Instance().critical(__VA_ARGS__)

#endif // _CXLOG_MACRO_H