#ifndef _STDLOG_H_
#define _STDLOG_H_
#include <cstdarg>

namespace lib_linux
{
    class StdLogHandler
    {
        public:
            StdLogHandler();
            virtual ~StdLogHandler();
            virtual void Write(const char *pStr, int level)=0;
    };

    class StdLogOutHandler:public StdLogHandler
    {
        public:
            // output to console
            void Write(const char *pStr, int level);
    };

    class ColorDecoratorHandler:public StdLogHandler
    {
        public:
            ColorDecoratorHandler(StdLogHandler *pHandler);
            // color output to console
            void Write(const char *pStr, int level);
        private:
            StdLogHandler *m_pHandler;
    };

    class StdLog
    {
        public:
            enum
            {
                LOG_LEVEL_ERROR = 0,
                LOG_LEVEL_WARNING,
                LOG_LEVEL_INFO,
                LOG_LEVEL_DEBUG
            };

        public:
            StdLog(StdLogHandler *pHandler=NULL);

            void SetHandler(StdLogHandler *pHandler);
            void SetLevel(int level);

            void Debug(const char *format, ...);
            void Info(const char *format, ...);
            void Warning(const char *format, ...);
            void Error(const char *format, ...);

        protected:
            void Write(const char *format, va_list arg, int level);

        private:
            StdLogHandler *m_pHandler;

            // print level less or equal than this
            int m_level;
    };
}

#endif
