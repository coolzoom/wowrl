/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            Profiler source             */
/*                                        */
/*  ## : A class that manages code        */
/*  profiling.                            */
/*                                        */

#include "frost_utils_timemanager.h"
#include "frost_utils_profiler.h"
#include "frost_utils_stdhelper.h"
#include "frost_utils_log.h"
#include "frost_utils_math.h"
#include "frost_utils_timers.h"

#ifdef WIN32
    #include <sys/time.h>
    #include <sys/timeb.h>
#else
    #include <sys/time.h>
#endif

using namespace std;

namespace Frost
{
    const s_str TimeManager::CLASS_NAME = "TimeManager";

    TimeManager::TimeManager()
    {
        dDelta_ = 0.017;
        uiWorstFPS_ = s_uint::INF;
        pGetTimeFunction_ = nullptr;
    }

    TimeManager::~TimeManager()
    {
    }

    void TimeManager::SetGetTimeFunction( GetTimeFunction pFunc )
    {
        pGetTimeFunction_ = pFunc;
    }

    void TimeManager::Initialize()
    {
        pFPSTimer_ = s_refptr<Timer>(new Timer(Timer::START_NOW));
        dLastTime_ = dCurTime_ = dStart_ = dLast_ = GetTime();

        dProfileTime_ = 0.0;
        uiFPS_ = 0;
        dDelta_ = 0.017;
        uiAvgFPS_ = 0;
        uiFrameNbr_ = 0;
        uiBestFPS_ = 0;
        uiWorstFPS_ = s_uint::INF;
    }

    void TimeManager::Update()
    {
        // Get delta time
        dLastTime_ = dCurTime_;
        dCurTime_ = GetTime();
        dDelta_ = dCurTime_ - dLastTime_;

        // Update FPS
        static s_uint uiFrameRateCount = 0;
        static s_uint uiFrameCount = 0;
        ++uiFrameCount;

        if (dDelta_ < 0.0001)
            uiFrameRateCount += 10000;
        else
            uiFrameRateCount += s_uint(1.0/dDelta_);

        if (pFPSTimer_ && pFPSTimer_->GetElapsed() > 1.0)
        {
            pFPSTimer_->Zero();

            uiFPS_ = uiFrameRateCount/uiFrameCount;
            uiFrameRateCount = 0;
            uiFrameCount = 0;

            uiAvgFPS_ += uiFPS_;
            ++uiFrameNbr_;

            uiBestFPS_ = s_uint::Max(uiBestFPS_, uiFPS_);
            uiWorstFPS_ = s_uint::Min(uiWorstFPS_, uiFPS_);
        }
    }

    s_double TimeManager::GetDelta() const
    {
        if (dDelta_ > 0.0001)
            return dDelta_;
        else
        {
            if (uiFPS_ != 0)
                return 1.0/uiFPS_.Get();
            else
                return 0.0;
        }
    }

    const s_uint& TimeManager::GetFPS() const
    {
        return uiFPS_;
    }

    s_uint TimeManager::GetAverageFPS() const
    {
        if (uiFrameNbr_ == 0)
            return s_uint(s_uint::INTEGER_NAN);
        else
            return uiAvgFPS_/uiFrameNbr_;
    }

    const s_uint& TimeManager::GetBestFPS() const
    {
        return uiBestFPS_;
    }

    const s_uint& TimeManager::GetWorstFPS() const
    {
        return uiWorstFPS_;
    }

    s_double TimeManager::GetTime() const
    {
        if (pGetTimeFunction_)
            return (*pGetTimeFunction_)();
        else
            return 0.0;
    }

    s_str TimeManager::GetPlayTime() const
    {
        s_double dTime = GetTime();
        s_uint uiHours(s_double::RoundDown(dTime/3600.0));
        dTime -= s_double(uiHours)*3600.0;
        s_uint uiMinutes(s_double::RoundDown(dTime/60.0));
        dTime -= s_double(uiMinutes)*60.0;
        s_uint uiSeconds(s_double::RoundDown(dTime));
        dTime -= s_double(uiSeconds);
        s_uint uiMilliseconds(s_double::RoundDown(dTime*1000));

        s_str sTime = "[";
        sTime += s_str::Convert(uiHours, 2) + ":";
        sTime += s_str::Convert(uiMinutes, 2) + ":";
        sTime += s_str::Convert(uiSeconds, 2) + ":";
        sTime += s_str::Convert(uiMilliseconds, 3) + "]";

        return sTime;
    }

    s_ptr<Profiler> TimeManager::GetProfiler( const s_uint& uiGroup, const s_str& sName, const s_bool& bRecord )
    {
        if (bProfiling_ == true)
        {
            bProfiled_ = true;

            if (!lProfilerList_.Find(sName))
            {
                lProfilerList_[sName] = Profiler(uiGroup, sName, bRecord);
            }

            return &lProfilerList_[sName];
        }
        else
            return nullptr;
    }

    void TimeManager::SetProfiling( const s_bool& bProfiling )
    {
        // Update total profiling time
        if (bProfiling && !bProfiling_)
            dLast_ = GetTime();
        else if (!bProfiling && bProfiling_)
            dProfileTime_ = GetTime() - dLast_;

        bProfiling_ = bProfiling;
    }

    void TimeManager::Print( const s_uint& uiGroup )
    {
        if (bProfiled_)
        {
            if (bProfiling_)
            {
                dProfileTime_ = GetTime() - dLast_;
                bProfiling_ = false;
            }

            Log("|t\n");
            if (!uiGroup.IsValid())
                Log("# Profiling info : "+ dProfileTime_ +" s of profiling");
            else
                Log("# Profiling info for group ["+ uiGroup +"] : "+ dProfileTime_ +" s of profiling");
            map<s_str, Profiler>::iterator iter;
            foreach (iter, lProfilerList_)
            {
                s_ptr<Profiler> pProfiler = &iter->second;
                if ( (pProfiler->GetGroup() == uiGroup) || (!uiGroup.IsValid()) )
                    pProfiler->PrintProfile(dProfileTime_);
            }
            Log("# end.");
        }
    }

    s_uint TimeManager::GetYear() const
    {
        time_t iTimestamp = time(nullptr);
        return localtime(&iTimestamp)->tm_year + 1900;
    }

    s_uint TimeManager::GetMonth() const
    {
        time_t iTimestamp = time(nullptr);
        return localtime(&iTimestamp)->tm_mon + 1;
    }

    s_uint TimeManager::GetDayName() const
    {
        time_t iTimestamp = time(nullptr);
        return localtime(&iTimestamp)->tm_wday + 1;
    }

    s_uint TimeManager::GetDay() const
    {
        time_t iTimestamp = time(nullptr);
        return localtime(&iTimestamp)->tm_mday;
    }

    s_uint TimeManager::GetHour() const
    {
        time_t iTimestamp = time(nullptr);
        return localtime(&iTimestamp)->tm_hour;
    }

    s_uint TimeManager::GetMinutes() const
    {
        time_t iTimestamp = time(nullptr);
        return localtime(&iTimestamp)->tm_min;
    }

    s_uint TimeManager::GetSeconds() const
    {
        time_t iTimestamp = time(nullptr);
        return localtime(&iTimestamp)->tm_sec;
    }

    s_uint TimeManager::GetMicroseconds() const
    {
    #ifdef WIN32
        timeb tb;
        ftime(&tb);
        return (uint)tb.millitm*1000;
    #else
        timeval tv;
        gettimeofday(&tv, nullptr);
        return tv.tv_usec;
    #endif
    }
}
