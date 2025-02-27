/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*             Timers source              */
/*                                        */
/*                                        */

#include "frost_utils_timers.h"
#include "frost_utils_timemanager.h"

using namespace std;

namespace Frost
{
    const s_str PeriodicTimer::CLASS_NAME = "PeriodicTimer";
    const s_str Timer::CLASS_NAME = "Timer";

    PeriodicTimer::PeriodicTimer( const s_double& dDuration, StartType mType, const s_bool& bTickFirst )
    {
        dDuration_ = dDuration;
        if (bTickFirst) dElapsed_ = dDuration;
        else dElapsed_ = 0.0;
        mType_ = mType;
        bPaused_ = true;
        bFirstTick_ = true;
        if (mType == START_NOW) Start();
    }

    s_double PeriodicTimer::GetElapsed()
    {
        if (!bPaused_)
            return dElapsed_ + TimeManager::GetSingleton()->GetTime() - dStart_;
        else
            return dElapsed_;
    }

    const s_double& PeriodicTimer::GetPeriod() const
    {
        return dDuration_;
    }

    const s_bool& PeriodicTimer::IsPaused() const
    {
        return bPaused_;
    }

    s_bool PeriodicTimer::Ticks()
    {
        if ( (mType_ == START_FIRST_TICK) && bFirstTick_ )
        {
            Start();
            bFirstTick_ = false;
        }

        if (!bPaused_)
        {
            if ((dElapsed_ + TimeManager::GetSingleton()->GetTime() - dStart_) >= dDuration_)
            {
                Zero();
                return true;
            }
            else
                return false;
        }
        else
        {
            if (dElapsed_ >= dDuration_)
                return true;
            else
                return false;
        }
    }

    void PeriodicTimer::Stop()
    {
        dElapsed_ = 0.0;
        bPaused_ = true;
    }

    void PeriodicTimer::Start()
    {
        if (bPaused_)
        {
            dStart_ = TimeManager::GetSingleton()->GetTime();
            bPaused_ = false;
        }
    }

    void PeriodicTimer::Pause()
    {
        if (!bPaused_)
        {
            dElapsed_ += TimeManager::GetSingleton()->GetTime() - dStart_;
            bPaused_ = true;
        }
    }

    void PeriodicTimer::Zero()
    {
        dElapsed_ = 0.0;
        dStart_ = TimeManager::GetSingleton()->GetTime();
    }

    Timer::Timer(const StartType& mType)
    {
        if (mType == START_PAUSED)
            bPaused_ = true;
        else
            dStart_ = TimeManager::GetSingleton()->GetTime();
    }

    void Timer::AddTime( const s_double& dAdditionnalTime )
    {
        dStart_ -= dAdditionnalTime;
    }

    void Timer::SetTime( const s_double& dTime )
    {
        dElapsed_ = dTime;
        dStart_ = TimeManager::GetSingleton()->GetTime();
    }

    s_double Timer::GetElapsed()
    {
        if (!bPaused_)
            return dElapsed_ + TimeManager::GetSingleton()->GetTime() - dStart_;
        else
            return dElapsed_;
    }

    const s_bool& Timer::IsPaused() const
    {
        return bPaused_;
    }

    void Timer::Stop()
    {
        dElapsed_ = 0.0;
        bPaused_ = true;
    }

    void Timer::Start( const s_bool& bReset )
    {
        dStart_ = TimeManager::GetSingleton()->GetTime();
        bPaused_ = false;
        if (bReset)
            dElapsed_ = 0.0;
    }

    void Timer::Pause()
    {
        if (!bPaused_)
        {
            dElapsed_ += TimeManager::GetSingleton()->GetTime() - dStart_;
            bPaused_ = true;
        }
    }

    void Timer::Zero()
    {
        dElapsed_ = 0.0;
        dStart_ = TimeManager::GetSingleton()->GetTime();
    }
}
