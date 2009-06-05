/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            HealthType source           */
/*                                        */
/*                                        */

#include "unit/frost_healthtype.h"

#include "unit/frost_unitmanager.h"

using namespace std;

namespace Frost
{
    HealthTypeInstance::HealthTypeInstance( s_ptr<Unit> pUnit, const s_str& sHealthName ) :
        pUnit_(pUnit)
    {
        pParent_ = UnitManager::GetSingleton()->GetHealthTypeByName(sHealthName);
        if (pParent_->mRegenType == HEALTH_REGEN_TICK)
            pTimer_ = s_refptr<PeriodicTimer>(new PeriodicTimer(s_double(pParent_->fRegenTickDuration), TIMER_START_FIRST, false));
    }

    const s_str& HealthTypeInstance::GetHealthTypeName() const
    {
        return pParent_->sName;
    }

    void HealthTypeInstance::On( const s_str& sScriptName, s_ptr<Event> pEvent )
    {
        if (pParent_->lDefinedScriptList.Find(sScriptName))
        {
            s_ptr<Lua::State> pLua = UnitManager::GetSingleton()->GetLua();

            pUnit_->PushOnLua();
            pLua->CallFunction(pParent_->sName+":On"+sScriptName);
        }
    }

    void HealthTypeInstance::Update( const s_float& fDelta )
    {
        switch (pParent_->mRegenType)
        {
            case HEALTH_REGEN_TICK :
            {
                if (pTimer_->Ticks())
                {
                    this->On("Tick");
                }
                break;
            }
            case HEALTH_REGEN_CONTINUOUS :
            {
                pUnit_->AddHealth(fDelta*pParent_->fRegenRate);
                break;
            }
            default : break;
        }
    }
}
