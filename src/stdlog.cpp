#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstdio>
#include <ctime>
#include <sys/time.h>
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

    void ColorDecoratorHandler::SetHandler(StdLogHandler *pHandler)
    {
        m_pHandler = pHandler;
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
        m_bufSize = BUFFER_SIZE;
        m_pBuffer = new char[m_bufSize];
        memset(m_pBuffer, 0, m_bufSize);
    }
    
    StdLog::~StdLog()
    {
        delete m_pBuffer;
        m_pBuffer = NULL;
    }

    void StdLog::SetHandler(StdLogHandler *pHandler)
    {
        m_pHandler = pHandler;
    }

    StdLogHandler *StdLog::GetHandler()
    {
        return m_pHandler;
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

        Debug("%s\n", stream_hex.str().c_str());
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
            const char *str_level[] = {"ERROR", "WARN", "INFO", "DEBUG"};

            // convert time
            struct timeval millsecond;
            struct tm *timeinfo;
            ::gettimeofday(&millsecond, 0);
            timeinfo = ::localtime(&millsecond.tv_sec);

            while (true)
            {
                int nFreeSize = m_bufSize;
                int nLen = snprintf(m_pBuffer, nFreeSize, 
                                    "[%-5s] [%02d:%02d:%02d:%03ld] [%s] ",
                                    str_level[level],
                                    timeinfo->tm_hour,
                                    timeinfo->tm_min,
                                    timeinfo->tm_sec,
                                    millsecond.tv_usec/1000,
                                    (error == 0)?"OK":strerror(error));

                if (nLen > 0 && nLen < nFreeSize)
                {
                    nFreeSize -= nLen;
                    nLen = vsnprintf(m_pBuffer+nLen, nFreeSize, format, arg);
                    if (nLen > 0 && nLen < nFreeSize)
                    {
                        break;
                    }
                }

                if (nLen <= 0)
                {
                    assert(0);
                    return;
                }

                m_bufSize *= 2;
                delete m_pBuffer;
                m_pBuffer = new char[m_bufSize];
                m_pHandler->WriteString(level, "stdlog line buffer size append %d\n", m_bufSize);
            }

            m_pHandler->WriteString(level, "%s", m_pBuffer);
        }
    }
}
