/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*             Thread source              */
/*                                        */
/*                                        */

#include "frost_utils_thread.h"

#include <boost/thread.hpp>

using namespace std;

namespace Frost
{
    Mutex::Mutex() : pMutex_(s_refptr<boost::mutex>(new boost::mutex()))
    {
    }

    Mutex::~Mutex()
    {
    }

    Lock::Lock( Mutex& mMutex ) : mMutex_(mMutex)
    {
        mMutex_.pMutex_->lock();
    }

    Lock::~Lock()
    {
        mMutex_.pMutex_->unlock();
    }

    Thread::Thread()
    {
    }

    Thread::Thread(void (*pFunction)()) : mFunction_(pFunction)
    {
    }

    Thread::~Thread()
    {
        if (pThread_)
            pThread_->join();
    }

    void Thread::SetFunction(void (*pFunction)())
    {
        Wait();

        mFunction_ = pFunction;
    }

    void Thread::Launch()
    {
        if (pThread_)
            pThread_->join();

        if (!mFunction_.empty())
        {
            bIsRunning_ = true;
            pThread_ = s_refptr<boost::thread>(new boost::thread(mFunction_));
        }
    }

    void Thread::Shutdown()
    {
        bIsRunning_ = false;
    }

    void Thread::Wait()
    {
        if (pThread_)
            pThread_->join();

        pThread_ = nullptr;
        bIsRunning_ = false;
    }

    s_bool Thread::IsActive()
    {
        return (pThread_ != nullptr);
    }

    s_bool Thread::IsRunning()
    {
        return bIsRunning_;
    }
}
