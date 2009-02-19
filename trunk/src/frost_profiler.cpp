/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*             Profiler source            */
/*                                        */
/*                                        */

#include "frost_profiler.h"
#include "frost_timemanager.h"

using namespace std;

namespace Frost
{
    const s_str Profiler::CLASS_NAME = "Profiler";
    const s_str Chrono::CLASS_NAME = "Chrono";

    Profiler::Profiler( s_uint uiGroup, s_str sName, s_bool bRecord )
    {
        sName_ = sName;
        uiGroup_ = uiGroup;
        bRecord_ = bRecord;
        uiCallNbr_ = 0;
        uiZeroTimings_ = 0;
        dTotalTime_ = 0.0f;
        dLowestTime_ = -1.0f;
        dHighestTime_ = -1.0f;
    }

    void Profiler::AddTiming( s_double dTime )
    {
        uiCallNbr_++;

        if (dTime < 0.000001)
            uiZeroTimings_++;
        else
        {
            if ( (dLowestTime_ == -1.0) || (dTime < dLowestTime_) )
                dLowestTime_ = dTime;
            if ( (dHighestTime_ == -1.0) || (dTime > dHighestTime_) )
                dHighestTime_ = dTime;

            dTotalTime_ += dTime;

            if (bRecord_)
                lRecordStack_.push_back(dTime);
        }
    }

    void Profiler::SetKeepRecords( s_bool bRecord )
    {
        bRecord_ = bRecord;
    }

    void Profiler::PrintProfile( s_double dProfileTime )
    {
        Log("  - "+ sName_ +" :");
        Log("     - call number : "+ uiCallNbr_ +" ("+ uiZeroTimings_ +" not timed)");

        if (uiCallNbr_ != 0)
        {
            Log("     - total time : "+ dTotalTime_ +" s ("+ 100.0*dTotalTime_/dProfileTime +"%% of total)");
            Log("     - average time : "+ 1000.0*dTotalTime_/s_double(uiCallNbr_-uiZeroTimings_) +" ms");
            Log("     - highest time : "+ 1000.0*dHighestTime_ +" ms");
            Log("     - lowest time : "+ 1000.0*dLowestTime_ +" ms");

            if (bRecord_)
            {
                Log("     - listing records :");
                s_uint i = 1;
                vector<s_double>::iterator iter;
                foreach (iter, lRecordStack_)
                {
                    Log("        ["+ i +"] : "+ 1000.0*(*iter) +" ms");
                    i++;
                }
            }
        }
    }

    s_uint Profiler::GetGroup()
    {
        return uiGroup_;
    }

    Chrono::Chrono(s_ptr<Profiler> pProfiler, s_bool bStart)
    {
        if (bStart)
        {
            if (pProfiler)
                dStart_ = TimeManager::GetSingleton()->GetTime();
            bStopped_ = false;
        }
        else
        {
            bStopped_ = true;
        }
        pParent_ = pProfiler;
        dTotal_ = 0.0f;
    }

    Chrono::~Chrono()
    {
        if ( (pParent_ != NULL) && (!bStopped_) )
        {
            s_double dTime = TimeManager::GetSingleton()->GetTime() - dStart_;
            dTotal_ += dTime;
            pParent_->AddTiming(dTotal_);
        }
    }

    void Chrono::Start()
    {
        if (bStopped_)
        {
            if (pParent_ != NULL)
            {
                dStart_ = TimeManager::GetSingleton()->GetTime();
            }
            bStopped_ = false;
        }
    }

    void Chrono::Stop( s_bool bSend )
    {
        if (!bStopped_)
        {
            if (pParent_ != NULL)
            {
                s_double dTime = TimeManager::GetSingleton()->GetTime() - dStart_;
                dTotal_ += dTime;
                if (!bSend)
                {
                    pParent_->AddTiming(dTotal_);
                    dTotal_ = 0.0;
                }
            }
            bStopped_ = true;
        }
    }

    s_double Chrono::GetTime()
    {
        return TimeManager::GetSingleton()->GetTime() - dStart_;
    }
}
