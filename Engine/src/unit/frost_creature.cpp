/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            Creature source             */
/*                                        */
/*                                        */

#include "unit/frost_creature.h"

#include "unit/frost_unitmanager.h"
#include <model/frost_modelmanager.h>
#include <model/frost_model.h>
#include <model/frost_animmanager.h>


using namespace std;

namespace Frost
{
    // TODO : Creature : Implement drops
    // TODO : Creature : Implement aggro (AI)

    const s_str Creature::CLASS_NAME = "Creature";

    Creature::Creature( const s_uint& uiID, const s_str& sName ) :
        MovableUnit(uiID, sName)
    {
        mOgreInterface_.SetCreature(this);
        mOgreInterface_.EnableMouse(true);
    }

    Creature::~Creature()
    {
    }

    void Creature::CreateGlue( s_ptr<Lua::State> pLua )
    {
        pLua->PushNumber(GetID());
        lGlueList_.PushBack(pLua->PushNew<LuaCreature>());
        pLua->SetGlobal(GetLuaID());
        pLua->Pop();
    }

    void Creature::SetBodyModel( const s_str& sModelName )
    {
        if (pBodyModel_ && sModelName == pBodyModel_->GetOgreModelName())
            return;

        pBodyModel_ = ModelManager::GetSingleton()->CreateModel(sModelName, sName_);
        mOgreInterface_.BindEntity(pBodyModel_->GetEntity());
        mOgreInterface_.SetSceneObject(this);
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
        MovableUnit::Update(fDelta);
    }

    const s_str& Creature::GetType() const
    {
        return CLASS_NAME;
    }

    void CreatureOgreInterface::SetCreature( s_ptr<Creature> pCreature )
    {
        pCreature_ = pCreature;
    }

    void CreatureOgreInterface::On( const s_str& sEvent )
    {
        if (sEvent == "Enter")
        {
            pCreature_->Highlight(true);
            UnitManager::GetSingleton()->NotifyMouseOveredUnit(pCreature_);
        }
        else if (sEvent == "Leave")
        {
            pCreature_->Highlight(false);
            UnitManager::GetSingleton()->NotifyMouseOveredUnit(nullptr);
        }
    }
}
