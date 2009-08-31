/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*          LightManager source           */
/*                                        */
/*                                        */

#include "scene/frost_lightmanager.h"

#include "scene/frost_light.h"

#include "OgreSceneManager.h"

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
        mSunDir_ = mSunDir;
        mSunDir_.Normalize();
    }

    const Vector& LightManager::GetSunDirection() const
    {
        return mSunDir_;
    }

    void LightManager::SetSunColor(const Color& mSunColor)
    {
        mSunColor_ = mSunColor;
    }

    const Color& LightManager::GetSunColor() const
    {
        return mSunColor_;
    }

    s_ptr<Light> LightManager::CreateLight( LightType mType )
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
        Engine::GetSingleton()->GetOgreSceneManager()->setAmbientLight(
            Color::FrostToOgre(mAmbient_)
        );
    }

    const Color& LightManager::GetAmbient() const
    {
        return mAmbient_;
    }
}
