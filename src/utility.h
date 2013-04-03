#ifndef __UTILITY_H__
#define __UTILITY_H__
#include <sys/sysinfo.h>
#include <limits.h>
#include <string>
#include "lib_linux_config.h"

namespace lib_linux
{
    class Utility
    {
        public:
            // sleep unit is ms
            static void Sleep(int ms)
            {
                // don't use usleep, because it's use SIGALRM may be confuse other's alarm signal.
                //usleep(ms*1000);
                struct timeval tv;
                tv.tv_sec = ms / 1000;
                tv.tv_usec = (ms % 1000) * 1000;
                ::select(0, NULL, NULL, NULL, &tv);
            }

            static unsigned long GetUptime()
            {
                struct sysinfo s_info;
                if(::sysinfo(&s_info) != 0)
                {
                    ERROR("call sysinfo get uptime fail");
                    return 0;
                }
                return s_info.uptime;
            }

            static std::string Strip(const std::string &str)
            {
                size_t pos_first = str.find_first_not_of(' ');
                size_t pos_last = str.find_last_not_of(' ');

                if (pos_first == std::string::npos ||
                        pos_last == std::string::npos)
                {
                    return std::string("");
                }
                else
                {
                    assert(pos_last >= pos_first);
                    return std::string(str, pos_first, pos_last-pos_first+1);
                }
            }

            static bool StrToInt(const std::string &str, long &nRet)
            {
                char *endptr = NULL;
                long val = ::strtol(str.c_str(), &endptr, 10);

                /* Check for various possible errors */
                if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN)) ||
                        (errno != 0) || 
                        (endptr == str.c_str())
                   ) 
                {
                    return false;
                }

                nRet = val;
                return true;
            }
    };
}

#endif
