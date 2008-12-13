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

using namespace std;

#define JUMP_COEF_1   (-4.0*dJumpHeight_/(dJumpDuration_*dJumpDuration_))
#define JUMP_COEF_2   (4.0*dJumpHeight_/dJumpDuration_)

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

    void Creature::SetBodyModel( const s_str& sModelName )
    {
        if (pBodyModel_)
        {
            if (sModelName == pBodyModel_->GetModelName())
                return;
        }

        pBodyModel_ = ModelManager::GetSingleton()->CreateModel(sModelName, sName_);
    }

    void Creature::Update( const s_float& fDelta )
    {
        Unit::Update(fDelta);
    }
}



