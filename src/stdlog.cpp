#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstdio>
#include <ctime>
#include <assert.h>
#include <errno.h>
#include <string.h>
#include "stdlog.h"

namespace lib_linux
{
    ///////////////////////////////////////////
    // StdLogHandler
    StdLogHandler::StdLogHandler()
    {
    }

    StdLogHandler::~StdLogHandler()
    {
    }

    void StdLogHandler::WriteString(int level, const char *format, ...)
    {
        va_list args;
        ::va_start(args, format);
        Write(level, format, args);
        ::va_end(args);
    }

    ///////////////////////////////////////////
    // StdLogOutHandler
    // default log will add \n to flush all
    void StdLogOutHandler::Write(int level, const char *format, va_list arg)
    {
        if (level == LOG_LEVEL_ERROR)
        {
            vfprintf(stderr, format, arg);
        }
        else
        {
            vfprintf(stdout, format, arg);
        }
    }


    //////////////////////////////////////////
    // ColorDecoratorHandler
    ColorDecoratorHandler::ColorDecoratorHandler(StdLogHandler *pHandler)
        :m_pHandler(pHandler)
    {
    }

    void ColorDecoratorHandler::Write(int level, const char *format, va_list arg)
    {
        assert(m_pHandler);
        switch (level)
        {
            case LOG_LEVEL_ERROR:
                m_pHandler->WriteString(level, "%s", "\033[1;31m");
                m_pHandler->Write(level, format, arg);
                m_pHandler->WriteString(level, "%s", "\033[0m");
                break;
            case LOG_LEVEL_WARNING:
                m_pHandler->WriteString(level, "%s", "\033[1;33m");
                m_pHandler->Write(level, format, arg);
                m_pHandler->WriteString(level, "%s", "\033[0m");
                break;
            case LOG_LEVEL_INFO:
                m_pHandler->WriteString(level, "%s", "\033[1;38m");
                m_pHandler->Write(level, format, arg);
                m_pHandler->WriteString(level, "%s", "\033[0m");
                break;
            case LOG_LEVEL_DEBUG:
                m_pHandler->WriteString(level, "%s", "\033[1;38m");
                m_pHandler->Write(level, format, arg);
                m_pHandler->WriteString(level, "%s", "\033[0m");
                break;
            default:
                assert(0 && "illegal level");
        }
    }


    /////////////////////////////////////////////////
    // StdLog
    StdLog::StdLog(StdLogHandler *pHandler)
        :m_pHandler(pHandler),
        m_level(LOG_LEVEL_DEBUG)
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
        Write(LOG_LEVEL_DEBUG, format, args);
        ::va_end(args);
    }

    void StdLog::Debug_HEX(const char *pData, int nLen)
    {
        std::stringstream stream_hex;

        int begin = 0;
        int end = nLen;
        stream_hex<<std::uppercase<<std::hex;
        for (int iter=begin; iter != end; iter++)
        {
            stream_hex<<std::setfill('0')<<std::setw(2)<<(int)(unsigned char)(pData[iter])<<" ";
        }

        Debug("%s", stream_hex.str().c_str());
    }

    void StdLog::Info(const char * format, ...)
    {
        va_list args;
        ::va_start(args, format);
        Write(LOG_LEVEL_INFO, format, args);
        ::va_end(args);
    }

    void StdLog::Warning(const char * format, ...)
    {
        va_list args;
        ::va_start(args, format);
        Write(LOG_LEVEL_WARNING, format, args);
        ::va_end(args);
    }

    void StdLog::Error(const char * format, ...)
    {
        va_list args;
        ::va_start(args, format);
        Write(LOG_LEVEL_ERROR, format, args);
        ::va_end(args);
    }

    void StdLog::Write(int level, const char *format, va_list arg)
    {
        int error = errno;
        if (level <= m_level)
        {
            assert(m_pHandler != NULL);
            assert(level >= LOG_LEVEL_ERROR && level <= LOG_LEVEL_DEBUG);
            const char *str_level[] = {"ERROR", "WARNING", "INFO", "DEBUG"};

            // convert time
            time_t rawtime;
            struct tm *timeinfo;
            ::time(&rawtime);
            timeinfo = ::localtime(&rawtime);
            m_pHandler->WriteString(level, "[%-7s] [%d-%d-%d %d:%d:%d] [%s] ",
                                           str_level[level],
                                           timeinfo->tm_year, 
                                           timeinfo->tm_mon,
                                           timeinfo->tm_mday,
                                           timeinfo->tm_hour,
                                           timeinfo->tm_min,
                                           timeinfo->tm_sec,
                                           (error == 0)?"OK":strerror(error));
            m_pHandler->Write(level, format, arg);
        }
    }
}
