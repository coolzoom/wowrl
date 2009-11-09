/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*             Timers header              */
/*                                        */
/*                                        */


#ifndef FROST_TIMERS_H
#define FROST_TIMERS_H

#include "frost_utils_config.h"
#include "frost_utils_types.h"

namespace Frost
{
    /// A... periodic timer
    /** This timer is meant to tick periodicaly,
    *   so you can use it for any periodic event
    *   such as key repetition or a count down.
    */
    class PeriodicTimer
    {
    public :

        enum StartType
        {
            /// The timer will start if you call Start()
            START_PAUSED,
            /// The timer starts immediatly after it is created
            START_NOW,
            /// The timer will start when you first call Ticks()
            START_FIRST_TICK
        };

        /// Default constructor
        /** \param dDuration The time interval between each tick
        *   \param mType     See TimerType
        *   \param bTicks    The timer ticks immediately
        */
        PeriodicTimer(const s_double& dDuration, StartType mType, const s_bool& bTicks);

        /// Returns the time elapsed since the last tick.
        /** \return The time elapsed since last tick
        */
        s_double        GetElapsed();

        /// Returns the period of the PeriodicTimer.
        /** \return The period of the PeriodicTimer
        */
        const s_double& GetPeriod() const;

        /// Cheks if this PeriodicTimer is paused.
        /** \return 'true' if this PeriodicTimer is paused
        */
        const s_bool&   IsPaused() const;

        /// Checks if the timer's period has been reached.
        /** \return 'true' if the period has been reached
        */
        s_bool          Ticks();

        /// Pauses the timer and resets it.
        void            Stop();

        /// Starts the timer but doesn't reset it.
        void            Start();

        /// Pauses the timer.
        void            Pause();

        /// Resets the timer but doesn't pause it.
        void            Zero();

        static const s_str CLASS_NAME;

    private :

        s_double dElapsed_;
        s_double dStart_;
        s_double dDuration_;
        s_bool   bPaused_;
        s_bool   bFirstTick_;

        StartType mType_;
    };


    /// A simple timer
    class Timer
    {
    public :

        enum StartType
        {
            /// The timer will start if you call Start()
            START_PAUSED,
            /// The timer starts immediatly after it is created
            START_NOW,
        };

        /// Constructor.
        Timer(const StartType& mType = START_PAUSED);

        /// Adds time to this Timer.
        /** \param dAdditionnalTime The time to add (can be negative)
        */
        void          AddTime(const s_double& dAdditionnalTime);

        /// Returns the total time elapsed.
        /** \return The total time elapsed.
        */
        s_double      GetElapsed();

        /// Cheks if this Timer is paused.
        /** \return 'true' if this Timer is paused
        */
        const s_bool& IsPaused() const;

        /// Starts this timer.
        void          Start(const s_bool& bReset = false);

        /// Pauses this timer.
        void          Pause();

        /// Stops this timer.
        void          Stop();

        /// Resets this timer (doesn't pause it).
        void          Zero();

        static const s_str CLASS_NAME;

    private :

        s_double dElapsed_;
        s_double dStart_;
        s_bool   bPaused_;

    };
}

#endif
