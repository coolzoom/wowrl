/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            Creature source             */
/*                                        */
/*                                        */

#include "unit/frost_creature.h"

#include "model/frost_modelmanager.h"
#include "model/frost_model.h"
#include "model/frost_animmanager.h"
#include "unit/frost_unitmanager.h"

using namespace std;

namespace Frost
{
    // TODO : Creature : Implémenter les drops
    // TODO : Creature : Implémenter l'aggro
    // TODO : Creature : Implémenter l'IA

    const s_str Creature::CLASS_NAME = "Creature";

    Creature::Creature( const s_uint& uiID, const s_str& sName ) :
        Unit(uiID, sName), mInterface_(this)
    {
    }

    Creature::~Creature()
    {
    }

    void Creature::CreateGlue( s_ptr<Lua::State> pLua )
    {
        pLua->PushNumber(GetID());
        lGlueList_.PushBack(
            pLua->Push<LuaCreature>(new LuaCreature(pLua->GetState()))
        );
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
        pBodyModel_->SetUserObject(&mInterface_);
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

    CreatureOgreInterface::CreatureOgreInterface( s_ptr<Creature> pCreature ) : pCreature_(pCreature)
    {
    }

    const Ogre::String& CreatureOgreInterface::getTypeName() const
    {
        static Ogre::String sName = "CREATURE";
        return sName;
    }

    s_ptr<Creature> CreatureOgreInterface::GetCreature() const
    {
        return pCreature_;
    }
}



