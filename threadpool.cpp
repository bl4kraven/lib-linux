#include <stdlib.h>
#include "threadpool.h"
#include "config.h"

ThreadPool::ThreadTask::ThreadTask(ThreadPool &pool)
:m_pool(pool)
{
}

void ThreadPool::ThreadTask::Run()
{
    try
    {
        while (m_pool.m_running)
        {
            ThreadPool::Task task;
            if (m_pool.Take(task))
            {
                task(NULL);
            }
        }
    }
    catch (...)
    {
        TRACE("exception caugth in threadpool");
        ::abort();
    }
}


ThreadPool::ThreadPool()
    :m_running(false)
{
}

ThreadPool::~ThreadPool()
{
    if (m_running)
    {
        Stop();
    }
}

void ThreadPool::Start(int numThreads)
{
    assert(numThreads > 0);
    assert(m_threads.empty());
    m_running = true;
    for (int i=0; i<numThreads; i++)
    {
        ThreadTask *pThread = new ThreadTask(*this);
        m_threads.push_back(pThread);
        pThread->Start();
    }
}

void ThreadPool::Stop()
{
    m_running = false;
    // wake up all threads from Take()
    for (unsigned int i=0; i<m_threads.size(); i++)
    {
        m_semaphore.Post();
    }

    for (std::vector<ThreadTask *>::iterator it=m_threads.begin(); it!=m_threads.end(); it++)
    {
        (*it)->Wait();
        delete (*it);
    }
    m_threads.clear();
}

void ThreadPool::Run(Task f)
{
    assert(m_running && "threadpool is not running");
    AutoLock lock(m_mutex);
    m_taskQueue.push_back(f);
    // add one task
    m_semaphore.Post();
}

bool ThreadPool::Take(Task &task)
{
    m_semaphore.Wait();
    {
        AutoLock lock(m_mutex);
        if (!m_taskQueue.empty())
        {
            task = m_taskQueue.front();
            m_taskQueue.pop_front();
            return true;
        }
        else
        {
            return false;
        }
    }
}
