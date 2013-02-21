#include <iostream>
#include "config.h"
#include "stdlog.h"
#include "stdnetlog.h"
using namespace std;
using namespace lib_linux;

int main(int argc, char **argv)
{
    StdLogNetHandler handler("127.0.0.1", 8080);
    ColorDecoratorHandler colorHandler(&handler);
    StdLog log(&colorHandler);

    log.SetLevel(StdLog::LOG_LEVEL_INFO);
    log.Debug("%s %d\n", "debug", 1);
    log.Info("%s %d\n", "info", 2);
    log.Warning("%s %d\n", "warning", 3);
    log.Error("%s %d\n", "error", 4);

    TRACE("%s %d\n", "debug", 1);
    INFO("%s %d\n", "info", 2);
    WARNING("%s %d\n", "warning", 3);
    ERROR("%s %d\n", "error", 4);
    return 0;
}
