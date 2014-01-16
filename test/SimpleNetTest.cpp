#include <iostream>
#include <sstream>
#include <signal.h>
#include <unistd.h>
#include "lib_linux_config.h"
#include "snet.h"

using namespace std;
using namespace lib_linux;

class MySession:public ISession
{
    public:
    protected:
        void OnConnect(SessionManager *pSessionManager)
        {
            cout<<get_peer_addr().get_ip().c_str()<<"new session join in, online sessions:"
                <<pSessionManager->GetSessionCount()<<endl;

            std::string str("Welcome to lbzhung's chat server:D\r\n");
            send(str.c_str(), str.length());
        }

        void OnDisconnect(SessionManager *pSessionManager)
        {
            cout<<get_peer_addr().get_ip().c_str()<<"exit, online sessions:"
                <<pSessionManager->GetSessionCount()-1<<endl;
        }

        void OnRead(SessionManager *pSessionManager)
        {
            char buffer[101];
            string strCur;

            while (true)
            {
                int nLen = recv(buffer, 100);
                if (nLen <= 0)
                {
                    break;
                }

                buffer[nLen] = '\0';
                strCur += buffer;
            }

            pSessionManager->Broadcast(strCur.c_str(), strCur.size(), this);
        }

        void OnError(SessionManager *pSessionManager, int nErrorCode, const char *pStr)
        {
            ERROR("ErrorCode:%d  %s", nErrorCode, pStr);
        }
};

volatile static bool is_exit = false;
static void onsig(int dummy)
{
    is_exit = true;
}

int main(int argc, char* argv[])
{
    ::signal(SIGHUP, onsig);
    ::signal(SIGINT, onsig);
    ::signal(SIGTERM, onsig);

    SessionFactoryImp<MySession> sm;
    SessionManager manager(10);

    // Start chat server and listen at port 7777.
    // You can login by "telnet localhost 7777".
    if (!manager.StartupServer(&sm, "0.0.0.0", 7777))
    {
        return 1;
    }

    while (!is_exit)
    {
        timeval timeout = { 1, 0 };
        manager.Poll(timeout);
        usleep(10*1000);
    }

    return 0;
}
