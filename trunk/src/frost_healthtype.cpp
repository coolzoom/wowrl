/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            HealthType source           */
/*                                        */
/*                                        */

#include "frost_healthtype.h"

#include "frost_unitmanager.h"

using namespace std;

namespace Frost
{
    HealthTypeInstance::HealthTypeInstance( s_ptr<Unit> pUnit, const s_str& sHealthName ) :
        pUnit_(pUnit)
    {
        pParent_ = UnitManager::GetSingleton()->GetHealthTypeByName(sHealthName);
        this->On("UnitSpawn");
    }

    const s_str& HealthTypeInstance::GetHealthTypeName() const
    {
        return pParent_->sName;
    }

    void HealthTypeInstance::On( const s_str& sScriptName, s_ptr<Event> pEvent )
    {
        if (VECTORFIND(sScriptName, pParent_->lDefinedScriptList))
        {
            s_ptr<Lua::State> pLua = UnitManager::GetSingleton()->GetLua();

            pLua->PushGlobal(pUnit_->GetLuaID());
            pLua->SetGlobal("unit");

            pLua->CallFunction(pParent_->sName+":On"+sScriptName);
        }
    }

    void HealthTypeInstance::Update( const s_float& fDelta )
    {
    }
}
