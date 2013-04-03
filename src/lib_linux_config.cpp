#include "lib_linux_config.h"

namespace lib_linux
{
    StdLog &GetLogger(void)
    {
        static StdLogOutHandler handler;
        static ColorDecoratorHandler colorHandler(&handler);
        static StdLog log(&colorHandler);
        return log;
    }
}
