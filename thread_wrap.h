#ifndef __THREAD_WRAP__
#define __THREAD_WRAP__
#include <pthread.h>
#include "semaphore_wrap.h"

class Thread
{
    public:
        Thread(bool bAutoStart=false);
        virtual ~Thread();

        void Start();
        void Wait();
        pthread_t GetThreadID();

    protected:
        virtual void Run() =0;

    private:
        Thread(const Thread &);
        const Thread &operator=(const Thread &);

        static void *ThreadFuntion(void *his);
    private:
        // thread id
        pthread_t m_threadID;

        // set detach thread, clean up automatically when it terminates.
        // so don't join, but no way to synchronize on its completion and obtain its return value.
        pthread_attr_t m_attr;

        Semaphore m_sem;
        // wait for thread to exit
        Semaphore m_semWait;

        // is semphore increase to release
        bool m_bRelease;
};

#endif
