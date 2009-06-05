/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            PowerType source            */
/*                                        */
/*                                        */

#include "unit/frost_powertype.h"

#include "unit/frost_unitmanager.h"

using namespace std;

namespace Frost
{
    PowerTypeInstance::PowerTypeInstance( s_ptr<Unit> pUnit, const s_str& sPowerName ) :
        pUnit_(pUnit)
    {
        pParent_ = UnitManager::GetSingleton()->GetPowerTypeByName(sPowerName);
        if (pParent_->mRegenType == POWER_REGEN_TICK)
            pTimer_ = s_refptr<PeriodicTimer>(new PeriodicTimer(s_double(pParent_->fRegenTickDuration), TIMER_START_FIRST, false));
    }

    const s_str& PowerTypeInstance::GetPowerTypeName() const
    {
        return pParent_->sName;
    }

    void PowerTypeInstance::On( const s_str& sScriptName, s_ptr<Event> pEvent )
    {
        if (pParent_->lDefinedScriptList.Find(sScriptName))
        {
            s_ptr<Lua::State> pLua = UnitManager::GetSingleton()->GetLua();

            pUnit_->PushOnLua();
            pLua->CallFunction(pParent_->sName+":On"+sScriptName);
        }
    }

    void PowerTypeInstance::Update( const s_float& fDelta )
    {
        switch (pParent_->mRegenType)
        {
            case POWER_REGEN_TICK :
            {
                if (pTimer_->Ticks())
                {
                    this->On("Tick");
                }
                break;
            }
            case POWER_REGEN_CONTINUOUS :
            {
                pUnit_->AddPower(fDelta*pParent_->fRegenRate);
                break;
            }
            default : break;
        }
    }
}
