#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__
#include <functional>
#include <deque>
#include <vector>

#include "thread_wrap.h"
#include "Mutex.h"

class ThreadPool
{
    public:
        typedef std::pointer_to_unary_function<void *, void> Task;

    protected:
        // task thread to fetch task from thread pool.
        class ThreadTask:public Thread
        {
            public:
                ThreadTask(ThreadPool &pool);
            protected:
                void Run();
            private:
                ThreadPool &m_pool;
        };

        friend class ThreadTask;
    public:
        ThreadPool();
        ~ThreadPool();

        void Start(int numThreads);
        void Stop();

        void Run(Task f);

    private:
        bool Take(Task &task);

    private:
        std::deque<Task> m_taskQueue;
        std::vector<ThreadTask *> m_threads;
        Mutex m_mutex;
        Semaphore m_semaphore; 
        bool m_running;
};
#endif
