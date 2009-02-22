/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*             Timers header              */
/*                                        */
/*                                        */


#ifndef FROST_TIMERS_H
#define FROST_TIMERS_H

#include "frost_utils.h"
#include "frost_utils_double.h"
#include "frost_utils_bool.h"

namespace Frost
{
    enum TimerType
    {
        /// The timer will start if you call Start()
        TIMER_START_NO,
        /// The timer starts immediatly after it is created
        TIMER_START_NOW,
        /// The timer will start when you first call Ticks()
        TIMER_START_FIRST
    };

    /// A... periodic timer
    /** This timer is meant to tick periodicaly,
    *   so you can use it for any periodic event
    *   such as key repetition or a count down.
    */
    class PeriodicTimer
    {
    public :

        /// Default constructor
        /** \param dDuration The time interval between each tick
        *   \param mType     See TimerType
        *   \param bTicks    The timer ticks immediately
        */
        PeriodicTimer(const s_double& dDuration, TimerType mType, const s_bool& bTicks);

        /// Returns the time elapsed since the last tick.
        /** \return The time elapsed since last tick
        */
        s_double GetElapsed();

        /// Checks if the timer's period has been reached.
        /** \return 'true' if the period has been reached
        */
        s_bool   Ticks();

        /// Pauses the timer and resets it.
        void     Stop();

        /// Starts the timer but doesn't reset it.
        void     Start();

        /// Pauses the timer.
        void     Pause();

        /// Resets the timer but doesn't pause it.
        void     Zero();

        static const s_str CLASS_NAME;

    private :

        s_double dElapsed_;
        s_double dStart_;
        s_double dDuration_;
        s_bool   bPaused_;

        TimerType mType_;
    };


    /// A simple timer
    class Timer
    {
    public :

        /// Constructor.
        Timer();

        /// Returns the total time elapsed.
        /** \return The total time elapsed.
        */
        s_double GetElapsed();

        /// Starts this timer.
        void     Start(const s_bool& bReset = false);

        /// Pauses this timer.
        void     Pause();

        /// Stops this timer.
        void     Stop();

        /// Resets this timer (doesn't pause it).
        void     Zero();

        static const s_str CLASS_NAME;

    private :

        s_double dElapsed_;
        s_double dStart_;
        s_bool   bPaused_;

    };
}

#endif
