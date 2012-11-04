#include "thread_wrap.h"
#include "utility.h"
#include "config.h"

pthread_key_t Thread::s_MainKey;

void Thread::Initialize()
{
    ::pthread_key_create(&Thread::s_MainKey, NULL);
}

Thread::Thread(bool bAutoStart)
:m_bRelease(false)
{
    ::pthread_attr_init(&m_attr);
    // set detached thread
    ::pthread_attr_setdetachstate(&m_attr, PTHREAD_CREATE_DETACHED);
    if (::pthread_create(&m_threadID, &m_attr, &ThreadFuntion, this) != 0)
    {
        TRACE("pthread_create create fail");
    }

    if (bAutoStart)
    {
        m_sem.Post();
        m_bRelease = true;
    }
}

Thread::~Thread()
{
    ::pthread_attr_destroy(&m_attr);
}

void *Thread::ThreadFuntion(void *his)
{
    // Sleep here to avid crash. 
    // because dervied thread class not constructor to setup vtable,
    // and call pure virtual function.
    Utility::Sleep(5);

    Thread *h=(Thread *)his;
    h->m_sem.Wait();
    h->Run();
    h->m_semWait.Post();
    return NULL;
}

void Thread::Start()
{
    if (!m_bRelease)
    {
        m_sem.Post();
        m_bRelease = true;
    }
    else
    {
        assert("Thread: Duplication call Start");
    }
}

void Thread::Wait()
{
    m_semWait.Wait();
}

pthread_t Thread::GetThreadID()
{
    return m_threadID;
}
