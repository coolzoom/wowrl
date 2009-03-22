/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            Creature source             */
/*                                        */
/*                                        */

#include "frost_creature.h"

#include "frost_modelmanager.h"
#include "frost_model.h"
#include "frost_animmanager.h"
#include "frost_unitmanager.h"

using namespace std;

namespace Frost
{
    const s_str Creature::CLASS_NAME = "Creature";

    Creature::Creature( const s_uint& uiID, const s_str& sName ) :
        Unit(uiID, sName)
    {
    }

    Creature::~Creature()
    {
    }

    void Creature::CreateGlue()
    {
        s_ptr<Lua::State> pLua = UnitManager::GetSingleton()->GetLua();
        pLua->PushNumber(GetID());
        LuaCreature* pNewGlue;
        pGlue_ = pNewGlue = new LuaCreature(pLua->GetState());
        Lunar<LuaCreature>::push(pLua->GetState(), pNewGlue);
        pLua->SetGlobal(GetLuaID());
    }

    void Creature::SetBodyModel( const s_str& sModelName )
    {
        if (pBodyModel_)
        {
            if (sModelName == pBodyModel_->GetModelName())
                return;
        }

        pBodyModel_ = ModelManager::GetSingleton()->CreateModel(sModelName, sName_);
    }

    void Creature::PushOnLua( s_ptr<Lua::State> pLua ) const
    {
        pLua->PushGlobal(GetLuaID());
        pLua->SetGlobal("unit");
        pLua->PushNil();
        pLua->SetGlobal("character");
        pLua->PushGlobal(GetLuaID());
        pLua->SetGlobal("creature");
    }

    void Creature::Update( const s_float& fDelta )
    {
        Unit::Update(fDelta);
    }
}



