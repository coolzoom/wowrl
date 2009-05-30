/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            Character source            */
/*                                        */
/*                                        */

#include "unit/frost_character.h"

#include "model/frost_modelmanager.h"
#include "model/frost_model.h"
#include "model/frost_animmanager.h"
#include "material/frost_materialmanager.h"
#include "material/frost_material.h"
#include "camera/frost_camera.h"
#include "unit/frost_unitmanager.h"

using namespace std;

namespace Frost
{
    // TODO : Character : Implémenter l'équipement
    // TODO : Character : Implémenter l'inventaire

    const s_str Character::CLASS_NAME = "Character";
    const s_str Race::CLASS_NAME = "Race";
    const s_str CharacterModelInfo::CLASS_NAME = "CharacterModelInfo";

    Character::Character( const s_uint& uiID, const s_str& sName, const Race& mRace, CharGender mGender ) :
       Unit(uiID, sName), mInterface_(this), mGender_(mGender), mRace_(mRace)
    {
        // Get the gender
        s_ptr<CharacterModelInfo> pCharModelInfo;
        if (mGender == GENDER_MALE)
        {
            pCharModelInfo = &mRace_.mMaleModelInfo;
        }
        else if (mGender == GENDER_FEMALE)
        {
            pCharModelInfo = &mRace_.mFemaleModelInfo;
        }

        // Create the body model
        pBodyModel_ = ModelManager::GetSingleton()->CreateModel(
            pCharModelInfo->sModel,
            sName
        );
        pBodyModel_->GetAnimMgr()->SetAnim(ANIM_STAND, ANIM_PRIORITY_BACKGROUND);
        pBodyModel_->GetAnimMgr()->Play();

        // Apply a texture to the body
        if (!pCharModelInfo->lBodyTextureList.empty())
        {
            s_refptr<Material> pBodyMat = MaterialManager::GetSingleton()->CreateMaterial3D(
                pCharModelInfo->lBodyTextureList.back()
            );
            pBodyMat->SetHardwareSkinning(true);

            pBodyModel_->SetMaterial(pBodyMat);
        }

        // Apply a texture to the hairs
        /*if (!pCharModelInfo->lHairTextureList.empty())
        {
            s_refptr<Material> pHairMat = MaterialManager::GetSingleton()->CreateMaterial3D(
                pCharModelInfo->lHairTextureList.back()
            );
            pHairMat->SetAlphaReject(true);
            pHairMat->SetHardwareSkinning(true);

            pBodyModel_->SetMaterial(pHairMat);
        }*/

        // Show default model parts
        if (!pCharModelInfo->lDefaultPartList.empty())
        {
            pBodyModel_->Hide(true);
            pBodyModel_->Show();

            vector<s_uint>::iterator iterPart;
            foreach(iterPart, pCharModelInfo->lDefaultPartList)
            {
                s_ptr<ModelPart> pPart = pBodyModel_->GetModelPart(*iterPart);
                if (pPart)
                    pPart->Show();
            }
        }

        pBodyModel_->Attach(s_ptr<MovableObject>(pNode_));
        pBodyModel_->SetUserObject(&mInterface_);
    }

    Character::~Character()
    {
        pBodyModel_->Detach();
    }

    void Character::CreateGlue()
    {
        s_ptr<Lua::State> pLua = UnitManager::GetSingleton()->GetLua();
        pLua->PushNumber(GetID());
        LuaCharacter* pNewGlue;
        pGlue_ = pNewGlue = new LuaCharacter(pLua->GetState());
        Lunar<LuaCharacter>::push(pLua->GetState(), pNewGlue);
        pLua->SetGlobal(GetLuaID());
    }

    const Race& Character::GetRace()
    {
        return mRace_;
    }

    s_ptr<ModelPart> Character::GetModelPart( const s_uint& uiCategory, const s_uint& uiVariation )
    {
        s_uint uiID = uiVariation;
        if (uiCategory != 0u)
        {
            if ( (uiVariation == 0u) || (uiVariation > 99) )
                uiID = 1;
            uiID = uiCategory*100u + uiID;
        }

        return pBodyModel_->GetModelPart(uiID);
    }

    void Character::PushOnLua( s_ptr<Lua::State> pLua ) const
    {
        pLua->PushGlobal(GetLuaID());
        pLua->SetGlobal("unit");
        pLua->PushGlobal(GetLuaID());
        pLua->SetGlobal("character");
        pLua->PushNil();
        pLua->SetGlobal("creature");
    }

    void Character::Update( const s_float& fDelta )
    {
        Unit::Update(fDelta);
    }

    CharacterOgreInterface::CharacterOgreInterface( s_ptr<Character> pCharacter ) : pCharacter_(pCharacter)
    {
    }

    const Ogre::String& CharacterOgreInterface::getTypeName() const
    {
        static Ogre::String sName = "CHARACTER";
        return sName;
    }

    s_ptr<Character> CharacterOgreInterface::GetCharacter() const
    {
        return pCharacter_;
    }
}
