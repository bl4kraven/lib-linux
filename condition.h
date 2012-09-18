#ifndef __CONDITION_H__
#define __CONDITION_H__

#include <pthread.h>
#include "Mutex.h"

class Condition
{
    public:
        Condition(Mutex &mutex)
            :m_mutex(mutex)
        {
            ::pthread_cond_init(&m_cond, NULL);
        }

        ~Condition()
        {
            ::pthread_cond_destroy(&m_cond);
        }
        
        // wait block condition and unlock mutex. so mutex and conditon are this:
        // condition:
        //               ____________
        // ______________            ______________
        //
        // mutex:
        // ______________            ______________
        //               ____________
        // so the wait condition will not miss.
        void Wait()
        {
            ::pthread_cond_wait(&m_cond, m_mutex.GetMutex());
        }

        void Signal()
        {
            ::pthread_cond_signal(&m_cond);
        }

        void SignalAll()
        {
            ::pthread_cond_broadcast(&m_cond);
        }

    private:
        Mutex &m_mutex;
        pthread_cond_t m_cond;
};

#endif
