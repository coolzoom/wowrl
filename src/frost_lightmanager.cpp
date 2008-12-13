/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*          LightManager source           */
/*                                        */
/*                                        */

#include "frost_lightmanager.h"

#include "frost_light.h"
#include "frost_gfx.h"

#include "OgreSceneManager.h"

using namespace std;

namespace Frost
{
    const s_str LightManager::CLASS_NAME = "LightManager";

    LightManager::LightManager()
    {
        uiCounter_ = 0u;
        mAmbient_ = Color(20, 20, 20);
    }

    LightManager::~LightManager()
    {
        map< s_uint, s_ptr<Light> >::iterator iterLight;
        foreach (iterLight, lLightList_)
        {
            delete iterLight->second.Get();
        }
    }

    void LightManager::Initialize()
    {
        Engine::GetSingleton()->GetOgreSceneManager()->setAmbientLight(
            GFX::FrostToOgreColor(mAmbient_)
        );
    }

    s_ptr<Light> LightManager::CreateLight( LightType mType )
    {
        s_ptr<Light> pLight(new Light(uiCounter_, mType));
        lLightList_[uiCounter_] = pLight;
        uiCounter_++;

        return pLight;
    }

    void LightManager::DeleteLight( s_ptr<Light> pLight )
    {
        if (pLight)
        {
            map< s_uint, s_ptr<Light> >::iterator iterLight;
            iterLight = lLightList_.find(pLight->GetID());

            if (iterLight != lLightList_.end())
            {
                if (iterLight->second->GetID() == pLight->GetID())
                {
                    // Everything went fine, delete and erase from map
                    delete iterLight->second.Get();
                    lLightList_.erase(iterLight);
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
        map< s_uint, s_ptr<Light> >::iterator iterLight;
        foreach (iterLight, lLightList_)
        {
            iterLight->second->Update(fDelta);
        }
    }

    void LightManager::SetAmbient( const Color& mAmbient )
    {
        mAmbient_ = mAmbient;
        Engine::GetSingleton()->GetOgreSceneManager()->setAmbientLight(
            GFX::FrostToOgreColor(mAmbient_)
        );
    }

    const Color& LightManager::GetAmbient() const
    {
        return mAmbient_;
    }
}
