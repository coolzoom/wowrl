/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*          LightManager source           */
/*                                        */
/*                                        */

#include "scene/frost_lightmanager.h"

#include "scene/frost_light.h"
#include "frost_engine.h"

#include <OgreSceneManager.h>

using namespace std;

namespace Frost
{
    const s_str LightManager::CLASS_NAME = "LightManager";

    LightManager::LightManager()
    {
        mAmbient_ = Color(20, 20, 20);
    }

    LightManager::~LightManager()
    {
        Log("Closing "+CLASS_NAME+"...");
        s_map< s_uint, s_ptr<Light> >::iterator iterLight;
        foreach (iterLight, lLightList_)
        {
            iterLight->second.Delete();
        }
    }

    void LightManager::Initialize()
    {
        Engine::GetSingleton()->GetOgreSceneManager()->setAmbientLight(
            Color::FrostToOgre(mAmbient_)
        );
    }

    void LightManager::SetSunDirection(const Vector& mSunDir)
    {
        if (!pSun_)
            pSun_ = CreateLight(Light::DIRECTIONAL);

        mSunDir_ = mSunDir;
        mSunDir_.Normalize();

        pSun_->SetDirection(mSunDir_);
    }

    const Vector& LightManager::GetSunDirection() const
    {
        return mSunDir_;
    }

    void LightManager::SetSunColor(const Color& mSunColor)
    {
        if (!pSun_)
            pSun_ = CreateLight(Light::DIRECTIONAL);

        mSunColor_ = mSunColor;
        pSun_->SetColor(mSunColor_);
    }

    const Color& LightManager::GetSunColor() const
    {
        return mSunColor_;
    }

    s_ptr<Light> LightManager::CreateLight( Light::Type mType )
    {
        s_ptr<Light> pLight = new Light(mType);
        lLightList_[pLight->GetID()] = pLight;

        return pLight;
    }

    void LightManager::DeleteLight( s_ptr<Light> pLight )
    {
        if (pLight)
        {
            s_map< s_uint, s_ptr<Light> >::iterator iterLight;
            iterLight = lLightList_.Get(pLight->GetID());

            if (iterLight != lLightList_.End())
            {
                if (iterLight->second->GetID() == pLight->GetID())
                {
                    // Everything went fine, delete and erase from map
                    iterLight->second.Delete();
                    lLightList_.Erase(iterLight);
                }
            }
            else
            {
                Warning(CLASS_NAME,
                    "Trying to call DeleteLight on a Light that has not been created by LightManager (ID:"+
                    pLight->GetID()+")."
                );
            }
        }
    }

    void LightManager::UpdateLights( const s_float& fDelta )
    {
        s_map< s_uint, s_ptr<Light> >::iterator iterLight;
        foreach (iterLight, lLightList_)
        {
            iterLight->second->Update(fDelta);
        }
    }

    void LightManager::SetAmbient( const Color& mAmbient )
    {
        mAmbient_ = mAmbient;
        if (!bAmbientLocked_)
        {
            Engine::GetSingleton()->GetOgreSceneManager()->setAmbientLight(
                Color::FrostToOgre(mAmbient_)
            );
        }
    }

    const Color& LightManager::GetAmbient() const
    {
        if (bAmbientLocked_)
            return mLockedAmbient_;
        else
            return mAmbient_;
    }

    const s_bool& LightManager::IsAmbientLocked() const
    {
        return bAmbientLocked_;
    }

    void LightManager::LockAmbient(const Color& mLockedAmbient)
    {
        bAmbientLocked_ = true;
        mLockedAmbient_ = mLockedAmbient;
        Engine::GetSingleton()->GetOgreSceneManager()->setAmbientLight(
            Color::FrostToOgre(mLockedAmbient_)
        );
    }

    void LightManager::UnlockAmbient()
    {
        bAmbientLocked_ = false;
        mLockedAmbient_ = Color::VOID;
        Engine::GetSingleton()->GetOgreSceneManager()->setAmbientLight(
            Color::FrostToOgre(mAmbient_)
        );
    }
}
