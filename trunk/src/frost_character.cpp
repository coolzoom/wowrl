/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            Character source            */
/*                                        */
/*                                        */

#include "frost_character.h"

#include "frost_modelmanager.h"
#include "frost_model.h"
#include "frost_animmanager.h"
#include "frost_materialmanager.h"
#include "frost_material.h"
#include "frost_camera.h"

using namespace std;

namespace Frost
{
    const s_str Character::CLASS_NAME = "Character";
    const s_str Race::CLASS_NAME = "Race";
    const s_str CharacterModelInfo::CLASS_NAME = "CharacterModelInfo";

    Character::Character( const s_uint& uiID, const s_str& sName, const Race& mRace, CharGender mGender ) :
        Unit(uiID, sName), mGender_(mGender), mRace_(mRace)
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

        // Attach the unit's camera to the model
        pCamera_->Attach(s_ptr<Model>(pBodyModel_.Get()));
    }

    Character::~Character()
    {
    }

    s_ptr<ModelPart> Character::GetModelPart( const s_uint& uiCategory, const s_uint& uiVariation )
    {
        s_uint uiID = uiVariation;
        if (!uiCategory.IsNull())
        {
            if ( (uiVariation.IsNull()) || (uiVariation > 99) )
                uiID = 1;
            uiID = uiCategory*100u + uiID;
        }

        return pBodyModel_->GetModelPart(uiID);
    }

    void Character::Update( const s_float& fDelta )
    {
        Unit::Update(fDelta);
    }
}
