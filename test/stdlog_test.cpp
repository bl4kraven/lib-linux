#include <iostream>
#include "lib_linux_config.h"
#include "stdlog.h"
#include "stdnetlog.h"
using namespace std;
using namespace lib_linux;

int main(int argc, char **argv)
{
    try
    {
        StdLogNetHandler handler("127.0.0.1", 8080);
        ColorDecoratorHandler colorHandler(&handler);
        StdLog log(&colorHandler);

        log.SetLevel(LOG_LEVEL_DEBUG);
        log.Debug("%s %d\n", "debug", 1);
        log.Info("%s %d\n", "info", 2);
        log.Warning("%s %d\n", "warning", 3);
        log.Error("%s %d\n", "error", 4);

        SET_LOG_LEVEL(ERROR);
        DEBUG("%s %d\n", "debug", 1);
        INFO("%s %d\n", "info", 2);
        WARNING("%s %d\n", "warning", 3);
        ERROR("%s %d\n", "error", 4);
    }
    catch (const char *pStr)
    {
        cout<<pStr<<endl;
        return -1;
    }
    return 0;
}
