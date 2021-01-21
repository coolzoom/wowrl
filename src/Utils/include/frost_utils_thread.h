/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              Thread header             */
/*                                        */
/*                                        */

#ifndef FROST_UTILS_THREAD_H
#define FROST_UTILS_THREAD_H

#include "frost_utils_types.h"

#define NO_BOOST_THREAD_H
#include "boost_thread.h"
#undef NO_BOOST_THREAD_H

namespace Frost
{
    class Lock;

    /// Prevents threads from accessing some code at the same time.
    /** Mutex stands for MUTual EXclusion.
    *   This object should be a member of the class requiering the mutex.
    *   Ideally you should have a mutex for each function, but not
    *   neccessarilly.
    *   \note This class should be used along with the Lock class.
    */
    class Mutex
    {
    friend class Lock;
    public :

        /// Constructor.
        Mutex();

        /// Destructor.
        ~Mutex();

    private :

        s_refptr<boost::mutex> pMutex_;
    };

    /// Utility to lock a mutex and release it while going out of scope.
    /** This object should be instanciated before each call to
    *   state modifying code that can be executed by two threads
    *   at the same time.
    */
    class Lock
    {
    public :

        /// Constructor.
        /** \param mMutex The mutex to lock
        */
        Lock(Mutex& mMutex);

        /// Destructor.
        /** \note Unlocks the mutex.
        */
        ~Lock();

    private :

        Mutex& mMutex_;
    };

    /// Wrapper around boost::thread to allow simple threading.
    /** \note Interface is inspired from SFML :
    *         http://www.sfml-dev.org
    */
    class Thread
    {
    public :

        /// Constructor.
        Thread();

        /// Constructor.
        /** \param pFunction The function to execute in the thread
        *   \note You still have to call Launch() to start the thread.
        */
        Thread(void (*pFunction)());

        /// Constructor.
        /** \param pFunction The member function to execute in the thread
        *   \param p         The object to call the function on
        *   \note You still have to call Launch() to start the thread.
        */
        template<class T>
        Thread(void (T::*pFunction)(), T* p) : mFunction_(boost::bind(pFunction, p))
        {
        }

        /// Constructor.
        /** \param mFunc The boost::function to execute in the thread
        *   \note You still have to call Launch() to start the thread.
        */
        Thread(const boost::function<void()>& mFunc);

        /// Destructor.
        /** \note Kills the thread, but waits for the function to return.
        */
        ~Thread();

        /// Sets the function to call in the thread.
        /** \param pFunction The function to execute in the thread
        *   \note You still have to call Launch() to start the thread.
        *   \note If the thread was already active, it waits until
        *         the end of the current function, then sets this
        *         new one.
        */
        void SetFunction(void (*pFunction)());

        /// Sets the function to call in the thread.
        /** \param pFunction The member function to execute in the thread
        *   \param p         The object to call the function on
        *   \note You still have to call Launch() to start the thread.
        *   \note If the thread was already active, it waits until
        *         the end of the current function, then sets this
        *         new one.
        */
        template<class T>
        void SetFunction(void (T::*pFunction)(), T* p)
        {
            Wait();

            mFunction_ = boost::bind(pFunction, p);
        }

        /// Sets the function to call in the thread.
        /** \param mFunc The boost::function to execute in the thread
        *   \note You still have to call Launch() to start the thread.
        *   \note If the thread was already active, it waits until
        *         the end of the current function, then sets this
        *         new one.
        */
        void SetFunction(const boost::function<void()>& mFunc);

        /// Starts the thread.
        /** \note Does nothing if no function was defined.
        */
        void Launch();

        /// Asks the thread to end execution.
        /** \note Just tells the thread it should stop, but its down
        *         to the thread function to actually return.
        *   \note When this function is called, IsRunning() returns
        *         'false', and when the threads eventually ends,
        *         IsActive() returns 'false'.
        */
        void Shutdown();

        /// Locks the current thread and wait for this one to end.
        /** \note Call this to ensure the whole content of this thread
        *         gets executed immediately.
        */
        void Wait();

        /// Checks if this thread is launched.
        /** \return 'true' if this thread is launched
        *   \note See Shutdown().
        */
        s_bool IsActive();

        /// Checks if this thread is being shutdown.
        /** \return 'true' if this thread is being shutdown
        *   \note See Shutdown().
        */
        s_bool IsRunning();

    private :

        boost::function<void()> mFunction_;
        s_refptr<boost::thread> pThread_;
        s_bool                  bIsRunning_;
    };
}

#endif
