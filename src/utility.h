#ifndef __UTILITY_H__
#define __UTILITY_H__
#include <string>

#include "config.h"

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
};
#endif
