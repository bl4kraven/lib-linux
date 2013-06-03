#include "lib_linux_config.h"
#include "syslog_log.h"
#include "singleton.h"

namespace lib_linux
{
    static StdLog &GetLogger(int flag)
    {
        if (flag & FLAG_CON)
        {
            // stdout console
            Singleton<StdLog>::Instance()->SetHandler(Singleton<StdLogOutHandler>::Instance());
        }
        else
        {
            // syslog
            Singleton<StdLog>::Instance()->SetHandler(Singleton<SyslogHandler>::Instance());
        }

        if (flag & FLAG_COLOR)
        {
            Singleton<ColorDecoratorHandler>::Instance()->SetHandler(Singleton<StdLog>::Instance()->GetHandler());
            Singleton<StdLog>::Instance()->SetHandler(Singleton<ColorDecoratorHandler>::Instance());
        }

        return *Singleton<StdLog>::Instance();
    }

    // default
    static int g_flag = FLAG_CON | FLAG_COLOR;
    void SetLogger(int flag, int level)
    {
        assert((g_flag & FLAG_CON) || (g_flag & FLAG_SYSLOG));
        g_flag = flag;
        GetLogger(g_flag).SetLevel(level);
    }

    StdLog &GetCurLogger()
    {
        return GetLogger(g_flag);
    }
}
