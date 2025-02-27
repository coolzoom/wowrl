/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            Character source            */
/*                                        */
/*                                        */

#include "unit/frost_character.h"

#include <model/frost_modelmanager.h>
#include <model/frost_model.h>
#include <scene/frost_node.h>
#include <model/frost_animmanager.h>
#include <material/frost_materialmanager.h>
#include <material/frost_material.h>
#include <camera/frost_camera.h>
#include "unit/frost_unitmanager.h"
#include "unit/frost_movableunithandler.h"
#include <frost_engine.h>

using namespace std;

namespace Frost
{
    // TODO : Character : Implement equipment
    // TODO : Character : Implement inventory

    const s_str Character::CLASS_NAME = "Character";
    const s_str Race::CLASS_NAME = "Race";
    const s_str CharacterModelInfo::CLASS_NAME = "CharacterModelInfo";

    Character::Character( const s_uint& uiID, const s_str& sName, const Race& mRace, Gender mGender ) :
       MovableUnit(uiID, sName), mGender_(mGender), mRace_(mRace)
    {
        mOgreInterface_.SetCharacter(this);
        mOgreInterface_.EnableMouse(true);

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

        if (pBodyModel_->HasAnimation())
        {
            pBodyModel_->GetAnimMgr()->SetAnim(ANIM_JUMP, ANIM_PRIORITY_BACKGROUND);
            pBodyModel_->GetAnimMgr()->Play();
        }

        // Apply a texture to the body
        if (!pCharModelInfo->lBodyTextureList.IsEmpty())
        {
            s_refptr<Material> pBodyMat = MaterialManager::GetSingleton()->CreateMaterial3D(
                pCharModelInfo->lBodyTextureList.Back()
            );

            if (pBodyModel_->HasAnimation())
                pBodyMat->SetShaders("Character_Skinning");
            else
                pBodyMat->SetShaders("SimpleTexture");

            if (Engine::GetSingleton()->GetBoolConstant("EnableMotionBlur"))
            {
                bMotionBlurEnabled_ = true;
                pBodyModel_->SetCustomShaderParameter(0, Ogre::Vector4(1,1,1,1));
            }

            pBodyModel_->SetMaterial(pBodyMat);
        }

        // Apply a texture to the hairs
        /*if (!pCharModelInfo->lHairTextureList.IsEmpty())
        {
            s_refptr<Material> pHairMat = MaterialManager::GetSingleton()->CreateMaterial3D(
                pCharModelInfo->lHairTextureList.back()
            );
            pHairMat->SetAlphaReject(true);
            pHairMat->SetHardwareSkinning(true);

            pBodyModel_->SetMaterial(pHairMat);
        }*/

        // Show default model parts
        if (!pCharModelInfo->lDefaultPartList.IsEmpty())
        {
            pBodyModel_->Hide(true);
            pBodyModel_->Show();

            s_ctnr<s_uint>::iterator iterPart;
            foreach(iterPart, pCharModelInfo->lDefaultPartList)
            {
                s_ptr<ModelPart> pPart = pBodyModel_->GetModelPart(*iterPart);
                if (pPart)
                    pPart->Show();
            }
        }

        pBodyModel_->AttachTo(pNode_);
        mOgreInterface_.BindEntity(pBodyModel_->GetEntity());
        mOgreInterface_.SetSceneObject(this);

        const AxisAlignedBox& mBox = pBodyModel_->GetBoundingBox();
        const Vector& mMin = mBox.GetMin();
        const Vector& mMax = mBox.GetMax();

        // Supposing :
        //  - the model is centered for its X and Z axis
        //  - the model's feet are on (0, 0, 0)
        //  - the model is approximately as large in X and Z.
        pHandler_->SetEllipsoidRadius(Vector(
            (mMax.Z() - mMin.Z())/2.0f,
            mMax.Y()/2.0f,
            (mMax.Z() - mMin.Z())/2.0f
        ));
        pHandler_->SetHotSpot(Vector(0.0f, -mMax.Y()/2.0f, 0.0f));
    }

    Character::~Character()
    {
        if (pBodyModel_)
            pBodyModel_->Detach();
    }

    void Character::CreateGlue( s_ptr<Lua::State> pLua )
    {
        pLua->PushNumber(GetID());
        lGlueList_.PushBack(pLua->PushNew<LuaCharacter>());
        pLua->SetGlobal(GetLuaID());
        pLua->Pop();
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

    void Character::SetPlayerControlled( const s_bool& bPlayerControlled )
    {
        Unit::SetPlayerControlled(bPlayerControlled);

        if (bPlayerControlled_)
            Highlight(false);
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
        MovableUnit::Update(fDelta);
    }

    const s_str& Character::GetType() const
    {
        return CLASS_NAME;
    }

    void CharacterOgreInterface::SetCharacter( s_ptr<Character> pCharacter )
    {
        pCharacter_ = pCharacter;
    }

    void CharacterOgreInterface::On( const s_str& sEvent )
    {
        if (sEvent == "Enter")
        {
            if (!pCharacter_->IsPlayerControlled())
                pCharacter_->Highlight(true);
            UnitManager::GetSingleton()->NotifyMouseOveredUnit(pCharacter_);
        }
        else if (sEvent == "Leave")
        {
            if (!pCharacter_->IsPlayerControlled())
                pCharacter_->Highlight(false);
            UnitManager::GetSingleton()->NotifyMouseOveredUnit(nullptr);
        }
    }
}
