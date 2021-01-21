/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

// Boost headers generate lots of warnings in GCC, so these
// are wrapped into a single header that is flagged as
// "system_header", and as such no longer emits any warning.
#pragma GCC system_header

#ifndef NO_BOOST_THREAD_H
#include <boost/thread.hpp>
#else
namespace boost
{
    class thread;
    class mutex;
}
#endif
#include <boost/function.hpp>
#include <boost/bind.hpp>
