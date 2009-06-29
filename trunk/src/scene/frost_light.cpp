/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              Light source              */
/*                                        */
/*                                        */

#include "scene/frost_light.h"

#include <OgreSceneManager.h>
#include <OgreLight.h>

using namespace std;

namespace Frost
{
    const s_str Light::CLASS_NAME = "Light";

    Light::Light(LightType mType) : mType_(mType)
    {
        pOgreLight_ = Engine::GetSingleton()->GetOgreSceneManager()->createLight(
            ("Light_"+uiID_).Get()
        );

        pNode_->attachObject(pOgreLight_.Get());

        switch (mType_)
        {
            case LIGHT_POINT :
            {
                pOgreLight_->setType(Ogre::Light::LT_POINT);
                break;
            }
            case LIGHT_DIRECTIONAL :
            {
                pOgreLight_->setType(Ogre::Light::LT_DIRECTIONAL);
                break;
            }
            case LIGHT_SPOT :
            {
                pOgreLight_->setType(Ogre::Light::LT_SPOTLIGHT);
                break;
            }
        }

        SetAttenuation(0.0f, 1.0f, 0.0f);
        SetRange(30.0f);
    }

    Light::~Light()
    {
        Engine::GetSingleton()->GetOgreSceneManager()->destroyLight(
            pOgreLight_.Get()
        );
    }

    void Light::SetColor( const Color& mDiffuse )
    {
        mDiffuse_ = mDiffuse;
        pOgreLight_->setDiffuseColour(Color::FrostToOgre(mDiffuse_));
    }

    void Light::SetSpecularColor( const Color& mSpecular )
    {
        mSpecular_ = mSpecular;
        pOgreLight_->setSpecularColour(Color::FrostToOgre(mSpecular_));
    }

    const Color& Light::GetColor() const
    {
        return mDiffuse_;
    }

    const Color& Light::GetSpecularColor() const
    {
        return mSpecular_;
    }

    void Light::SetSpotDimensions( const s_float& fInAngle, const s_float& fOutAngle )
    {
        if (mType_ == LIGHT_SPOT)
        {
            fInAngle_ = fInAngle;
            fOutAngle_ = fOutAngle;
            pOgreLight_->setSpotlightInnerAngle(Ogre::Radian(fInAngle_.GetRad().Get()));
            pOgreLight_->setSpotlightOuterAngle(Ogre::Radian(fOutAngle_.GetRad().Get()));
        }
    }

    void Light::SetSpotFalloff( const s_float& fFalloff )
    {
        if (mType_ == LIGHT_SPOT)
        {
            fFalloff_ = fFalloff;
            pOgreLight_->setSpotlightFalloff(fFalloff.Get());
        }
    }

    void Light::SetAttenuation( const s_float& fConstant, const s_float& fLinear, const s_float& fQuadratic )
    {
        lAttenFactors_[1] = fConstant;
        lAttenFactors_[2] = fLinear;
        lAttenFactors_[3] = fQuadratic;
        bChangeFactors_ = true;
    }

    void Light::SetRange( const s_float& fRange )
    {
        lAttenFactors_[0] = fRange;
        bChangeFactors_ = true;
    }

    s_ptr<Ogre::Light> Light::GetOgreLight()
    {
        return pOgreLight_;
    }

    const s_uint& Light::GetID() const
    {
        return uiID_;
    }

    void Light::Update( const s_float& fDelta )
    {
        MovableObject::Update(fDelta);

        if (bChangeFactors_)
        {
            pOgreLight_->setAttenuation(
                lAttenFactors_[0].Get(),
                lAttenFactors_[1].Get(),
                lAttenFactors_[2].Get(),
                lAttenFactors_[3].Get()
            );
            bChangeFactors_ = false;
        }
    }
}
