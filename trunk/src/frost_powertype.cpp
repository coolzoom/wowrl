/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            PowerType source            */
/*                                        */
/*                                        */

#include "frost_powertype.h"

#include "frost_unitmanager.h"

using namespace std;

namespace Frost
{
    PowerTypeInstance::PowerTypeInstance( s_ptr<Unit> pUnit, const s_str& sPowerName ) :
        pUnit_(pUnit)
    {
        pParent_ = UnitManager::GetSingleton()->GetPowerTypeByName(sPowerName);
        this->On("UnitSpawn");
    }

    const s_str& PowerTypeInstance::GetPowerTypeName() const
    {
        return pParent_->sName;
    }

    void PowerTypeInstance::On( const s_str& sScriptName, s_ptr<Event> pEvent )
    {
        if (VECTORFIND(sScriptName, pParent_->lDefinedScriptList))
        {
            s_ptr<Lua::State> pLua = UnitManager::GetSingleton()->GetLua();

            pLua->PushGlobal(pUnit_->GetLuaID());
            pLua->SetGlobal("unit");

            pLua->CallFunction(pParent_->sName+":On"+sScriptName);
        }
    }

    void PowerTypeInstance::Update( const s_float& fDelta )
    {
    }
}
