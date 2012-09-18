#ifndef __CONFIG__
#define __CONFIG__

#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#ifdef _DEBUG
    #include <cstdio>
    #define TRACE(format, args...) printf("TRACE:%s ", strerror(errno));printf(format, ##args);printf("\n");
#else
    #define TRACE(format, args...)
#endif 

#endif
