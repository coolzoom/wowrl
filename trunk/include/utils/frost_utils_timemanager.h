/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            TimeManager header          */
/*                                        */
/*                                        */
/*                                        */


#ifndef FROST_TIMEMANAGER_H
#define FROST_TIMEMANAGER_H

#include "frost_utils_config.h"
#include "frost_utils_manager.h"
#include "frost_utils_types.h"

namespace Ogre
{
    class Timer;
}

namespace Frost
{
    class Profiler;

    /// Keeper of time :)
    /** This class manages everything that is related
    *   to time : FPS, date and profiling.
    */
    class TimeManager : public Manager<TimeManager>
    {
    friend class Manager<TimeManager>;
    public :

        /// Initializes this manager
        /** \note Should be called just before entering
        *         the main loop.
        */
        void          Initialize();

        /// Updates delta time and FPS.
        void          Update();

        /// Returns the time elapsed since last frame.
        /** \return Elapsed time since last frame
        */
        s_double      GetDelta() const;

        /// Returns the current Frame Per Second (FPS) count.
        /** \return Average number of frame per second over a second
        */
        const s_uint& GetFPS() const;

        /// Returns average frame per second since game launch.
        /** \return Average frame per second since game launch
        */
        s_uint        GetAverageFPS() const;

        /// Returns the highest Frame Per Second (FPS) count.
        /** \return Highest number of frame per second over a second
        */
        const s_uint& GetBestFPS() const;

        /// Returns the lowest Frame Per Second (FPS) count.
        /** \return Lowest number of frame per second over a second
        */
        const s_uint& GetWorstFPS() const;

        /// Returns the current time in seconds.
        /** \return Current time (origin : game's start)
        */
        s_double      GetTime() const;

        /// Returns the current time (year).
        /** \return The current year
        */
        s_uint        GetYear() const;

        /// Returns the current time (month).
        /** \return The current month
        */
        s_uint        GetMonth() const;

        /// Returns the current time (day name code).
        /** \return The current day name code
        */
        s_uint        GetDayName() const;

        /// Returns the current time (day).
        /** \return The current day number
        */
        s_uint        GetDay() const;

        /// Returns the current time (hours).
        /** \return The current hour
        */
        s_uint        GetHour() const;

        /// Returns the current time (minutes).
        /** \return The current minutes
        */
        s_uint        GetMinutes() const;

        /// Returns the current time (microseconds).
        /** \return The current microseconds
        *   \note On windows, this function only has milisecond
        *         resolution.
        */
        s_uint        GetMicroseconds() const;

        /// Returns the time elapsed since game's start.
        /** \return The elapsed time since the game has started,
        *   formated : "[h.:m.:s.:ms.]"
        */
        s_str         GetPlayTime() const;

        /// Returns the current time (seconds).
        /** \return The current seconds
        */
        s_uint        GetSeconds() const;

        /// Starts or stops profiling.
        /** \param bProfiling Starts profiling
        */
        void          SetProfiling(const s_bool& bProfiling);

        /// Checks if the manager is currently profiling.
        /** \return 'true' if it is profiling
        */
        s_bool        IsProfiling() const;

        /// Creates a profiler with the given parameters.
        /** \param iGroup  The group to which it belongs (used to group prints later)
        *   \param sName   The name of this Profiler (usualy the name of the function
        *                  it profiles)
        *   \param bRecord Store all timings in a list to print them later
        *   \note If this profiler already exists, this function returns a pointer to it.
        */
        s_ptr<Profiler> GetProfiler(const s_uint& iGroup, const s_str& sName, const s_bool& bRecord);

        /// Prints in the log all timing infos.
        /** \param iGroup Filter output by group number
        */
        void          Print(const s_uint& iGroup = s_uint::INF);

        static const s_str CLASS_NAME;

    protected :

        /// Default constructor
        /** \note This is a singleton class. That means that there can
        *         only be a single instance at the same time. That's why
        *         the constructor is not in public scope.<br>
        *         If you need to get a pointer to that unique instance,
        *         call TimeManager::GetSingleton().<br>
        *         Note that, if GetSingleton() has been called at least once,
        *         you'll have to delete the returned pointer when you close
        *         your program by calling TimeManager::Delete() (this is
        *         automatically done by Engine).
        */
        TimeManager();

        /// Destructor
        /** \note If GetSingleton() has been called at least once,
        *         you'll have to delete the returned pointer when
        *         you close your program by calling Delete() (this is
        *         automatically done by Engine).
        */
        ~TimeManager();

        /// Copy constructor
        TimeManager(const TimeManager& mMgr);

        /// Assignment operator
        TimeManager& operator = (const TimeManager& mMgr);

    private :

        s_refptr<Ogre::Timer> pTimer_;
        s_refptr<Ogre::Timer> pFPSTimer_;

        s_bool bProfiled_;
        s_bool bProfiling_;

        s_uint   uiFPS_;
        s_double dDelta_;

        s_uint uiFrameNbr_;
        s_uint uiAvgFPS_;
        s_uint uiBestFPS_;
        s_uint uiWorstFPS_;

        s_double dStart_;

        s_double dLastTime_;
        s_double dCurTime_;

        s_double dLast_;
        s_double dProfileTime_;

        s_map<s_str, Profiler> lProfilerList_;
    };
}

#endif
