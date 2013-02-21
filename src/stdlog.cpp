#include <iostream>
#include <cstdio>
#include <ctime>
#include <assert.h>
#include <errno.h>
#include <string.h>
#include "stdlog.h"

namespace lib_linux
{
    // StdLogHandler
    StdLogHandler::StdLogHandler()
    {
    }

    StdLogHandler::~StdLogHandler()
    {
    }

    // StdLogOutHandler
    void StdLogOutHandler::Write(const char *pStr, int level)
    {
        std::cout<<pStr;
    }

    // ColorDecoratorHandler
    ColorDecoratorHandler::ColorDecoratorHandler(StdLogHandler *pHandler)
        :m_pHandler(pHandler)
    {
    }

    void ColorDecoratorHandler::Write(const char *pStr, int level)
    {
        assert(m_pHandler);
        switch (level)
        {
            case StdLog::LOG_LEVEL_ERROR:
                m_pHandler->Write("\033[1;31m", level);
                m_pHandler->Write(pStr, level);
                m_pHandler->Write("\033[0m", level);
                break;
            case StdLog::LOG_LEVEL_WARNING:
                m_pHandler->Write("\033[1;33m", level);
                m_pHandler->Write(pStr, level);
                m_pHandler->Write("\033[0m", level);
                break;
            case StdLog::LOG_LEVEL_INFO:
            case StdLog::LOG_LEVEL_DEBUG:
                m_pHandler->Write("\033[1;38m", level);
                m_pHandler->Write(pStr, level);
                m_pHandler->Write("\033[0m", level);
                break;
            default:
                assert(false);
        }
    }

    // StdLog
    StdLog::StdLog(StdLogHandler *pHandler)
    :m_pHandler(pHandler),
    m_level(LOG_LEVEL_INFO)
    {
    }

    void StdLog::SetHandler(StdLogHandler *pHandler)
    {
        m_pHandler = pHandler;
    }

    void StdLog::SetLevel(int level)
    {
        assert(level >= LOG_LEVEL_ERROR && level <= LOG_LEVEL_DEBUG);
        m_level = level;
    }

    void StdLog::Debug(const char * format, ...)
    {
        va_list args;
        ::va_start(args, format);
        Write(format, args, LOG_LEVEL_DEBUG);
        ::va_end(args);
    }

    void StdLog::Info(const char * format, ...)
    {
        va_list args;
        ::va_start(args, format);
        Write(format, args, LOG_LEVEL_INFO);
        ::va_end(args);
    }

    void StdLog::Warning(const char * format, ...)
    {
        va_list args;
        ::va_start(args, format);
        Write(format, args, LOG_LEVEL_WARNING);
        ::va_end(args);
    }

    void StdLog::Error(const char * format, ...)
    {
        va_list args;
        ::va_start(args, format);
        Write(format, args, LOG_LEVEL_ERROR);
        ::va_end(args);
    }

    void StdLog::Write(const char *format, va_list arg, int level)
    {
        int error = errno;
        if (level <= m_level)
        {
            assert(m_pHandler != NULL);
            assert(level >= LOG_LEVEL_ERROR && level <= LOG_LEVEL_DEBUG);
            const char *str_level[] = {"ERROR", "WARNING", "INFO", "DEBUG"};
            char buffer[256];

            // convert time
            time_t rawtime;
            struct tm * timeinfo;
            ::time(&rawtime);
            timeinfo = ::localtime(&rawtime);

            int nLen = ::snprintf(buffer, sizeof(buffer), "[%-7s] ", str_level[level]);
            nLen += strftime (buffer+nLen, sizeof(buffer)-nLen, "[%Y-%m-%d %H:%M:%S] ",timeinfo);
            if (error == 0)
            {
                nLen += ::snprintf(buffer+nLen, sizeof(buffer)-nLen, "[%s] ", "OK");
            }
            else
            {
                nLen += ::snprintf(buffer+nLen, sizeof(buffer)-nLen, "[%s] ", strerror(error));
            }
            ::vsnprintf(buffer+nLen, sizeof(buffer)-nLen, format, arg);
            m_pHandler->Write(buffer, level);
        }
    }
}
