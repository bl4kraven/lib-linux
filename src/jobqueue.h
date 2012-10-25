#ifndef __JOBQUEUE_H__
#define __JOBQUEUE_H__

#include <deque>
#include "condition.h"
#include "Mutex.h"
#include "config.h"

// job queue for thread communication.
template <class T>
class JobQueue
{
    public:
        JobQueue()
        {}

        ~JobQueue()
        {
            assert(m_queue.size() == 0);
        }

        int Size()
        {
            AutoLock lock(m_mutex);
            return m_queue.size();
        }

        void Push(T elem)
        {
            {
                // only enqueue in WaitPop called. 
                // see condition.h explain it.
                AutoLock lock(m_mutex);
                m_queue.push_back(elem);
            }

            // this only active one thread.
            m_condition.Signal();
        }

        bool WaitPop(unsigned int unMilSecs, T &ret)
        {
            AutoLock lock(m_mutex);
            if (m_queue.size() == 0)
            {
                // call this will unlock mutex, so thread can push only this time.
                if (!m_condition.Wait(m_mutex, unMilSecs))
                {
                    return false;
                }
            }

            ret = m_queue.front();
            m_queue.pop_front();
            return true;
        }

    private:
        Mutex m_mutex;
        std::deque<T> m_queue;
        Condition m_condition;
};

#endif
