/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            Profiler header             */
/*                                        */
/*                                        */


#ifndef FROST_PROFILER_H
#define FROST_PROFILER_H

#include "frost_utils.h"
#include "frost_utils_str.h"
#include "frost_utils_uint.h"
#include "frost_utils_double.h"
#include "frost_utils_bool.h"
#include "frost_utils_ptr.h"

namespace Frost
{
    /// The profiling class
    /** A profiler should be assigned to a single
    *   function or portion of code. It will accumulate
    *   timings from every Chrono created from it.
    */
    class Profiler
    {
    public :

        /// Default constructor.
        /** \param iGroup  The group to which it belongs (used to group prints later)
        *   \param sName   The name of this Profiler (usualy the name of the function
        *                it profiles)
        *   \param bRecord Store all timings in a list to print them later
        */
        Profiler(const s_uint& iGroup, const s_str& sName, const s_bool& bRecord);

        /// Adds a timing.
        /** \param dTiming The timing
        */
        void          AddTiming(const s_double& dTiming);

        /// Prints timing infos.
        /** \param dTotal The total profiling time, used to calculate percentages.
        */
        void          PrintProfile(const s_double& dTotal);

        /// Switches timings recording.
        void          SetKeepRecords(const s_bool& bRecord);

        /// Returns this profiler's group.
        /** \return This Profiler's group
        */
        const s_uint& GetGroup();

        static const s_str CLASS_NAME;

    private :

        s_str    sName_;
        s_uint   uiGroup_;
        s_double dStartTime_;
        s_double dTotalTime_;
        s_double dHighestTime_;
        s_double dLowestTime_;
        s_uint   uiCallNbr_;
        s_uint   uiZeroTimings_;
        s_bool   bRecord_;

        std::vector<s_double> lRecordStack_;
    };

    /// An automatic timer
    /** This timer starts when you create it and ends
    *   at destruction or when you call Stop().<br>
    *   So, to use it, the simplest way is to call :<br>
    *       Chrono c = Chrono(TimeManager::GetProfiler(...));<br>
    *   And when c will be destroyed, it will add a timing
    *   to the profiler you gave in the constructor.<br>
    *   Note : This class should only be used for profiling
    *   purposes.
    */
    class Chrono
    {
    public :

        /// Constructor.
        /** \param pProfiler The profiler that will be linked to this Chrono
        *   \param bStart    'true' to make it start immediately
        */
        Chrono(s_ptr<Profiler> pProfiler, const s_bool& bStart = true);

        /// Destructor.
        ~Chrono();

        /// Returns the time elapsed since its creation.
        /** \return Elapsed time since its creation
        */
        s_double GetTime();

        /// Restarts a timer after it has been stopped.
        void     Start();

        /// Stops this timer and add its timing to the profiler.
        /** \param bSend If set to false, the timer will only be
        *   paused and won't send data to the profiler.
        */
        void     Stop(const s_bool& bSend = true);

        static const s_str CLASS_NAME;

    private :

        s_ptr<Profiler> pParent_;
        s_double        dStart_;
        s_double        dTotal_;
        s_bool          bStopped_;
    };
}

#endif
