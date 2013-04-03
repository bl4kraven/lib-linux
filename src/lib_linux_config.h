#ifndef __LIB_LINUX_CONFIG__
#define __LIB_LINUX_CONFIG__

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <errno.h>
#include <assert.h>
#include "stdlog.h"

namespace lib_linux
{
    // get system logger
    StdLog &GetLogger(void);
}

#define SET_LOG_LEVEL(level)       lib_linux::GetLogger().SetLevel(lib_linux::LOG_LEVEL_##level)
#define DEBUG(format, args...)     lib_linux::GetLogger().Debug(format"\n", ##args)
#define INFO(format, args...)      lib_linux::GetLogger().Info(format"\n", ##args)
#define WARNING(format, args...)   lib_linux::GetLogger().Warning(format"\n", ##args)
#define ERROR(format, args...)     lib_linux::GetLogger().Error(format"\n", ##args)

#endif
