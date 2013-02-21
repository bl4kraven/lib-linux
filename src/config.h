#ifndef __CONFIG__
#define __CONFIG__

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

    #ifdef _DEBUG
            #define TRACE(format, args...)     GetLogger().Debug(format, ##args);
            #define INFO(format, args...)      GetLogger().Info(format, ##args);
            #define WARNING(format, args...)   GetLogger().Warning(format, ##args);
            #define ERROR(format, args...)     GetLogger().Error(format, ##args);
    #else
            #define TRACE(format, args...)
            #define INFO(format, args...)
            #define WARNING(format, args...)
            #define ERROR(format, args...)
    #endif 
}

#endif
