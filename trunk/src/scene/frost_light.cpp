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

    Light::Light( Type mType, s_ptr<Ogre::SceneManager> pSceneManager ) : MovableObject(s_uint::NaN, pSceneManager), mType_(mType)
    {
        pOgreLight_ = pSceneManager_->createLight(("Light_"+uiID_).Get());

        pNode_->attachObject(pOgreLight_.Get());

        switch (mType_)
        {
            case POINT :
            {
                pOgreLight_->setType(Ogre::Light::LT_POINT);
                SetAttenuation(0.0f, 1.0f, 0.0f);
                break;
            }
            case DIRECTIONAL :
            {
                pOgreLight_->setType(Ogre::Light::LT_DIRECTIONAL);
                SetAttenuation(1.0f, 0.0f, 0.0f);
                break;
            }
            case SPOT :
            {
                pOgreLight_->setType(Ogre::Light::LT_SPOTLIGHT);
                SetAttenuation(0.0f, 1.0f, 0.0f);
                break;
            }
        }

        SetRange(30.0f);
    }

    Light::~Light()
    {
        pSceneManager_->destroyLight(pOgreLight_.Get());
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

    void Light::SetDirection( const Vector& mDir )
    {
        if (mType_ == SPOT || mType_ == DIRECTIONAL)
        {
            pOgreLight_->setDirection(Vector::FrostToOgre(mDir));
        }
    }

    void Light::SetSpotDimensions( const s_float& fInAngle, const s_float& fOutAngle )
    {
        if (mType_ == SPOT)
        {
            fInAngle_ = fInAngle;
            fOutAngle_ = fOutAngle;
            pOgreLight_->setSpotlightInnerAngle(Ogre::Radian(fInAngle_.GetRad().Get()));
            pOgreLight_->setSpotlightOuterAngle(Ogre::Radian(fOutAngle_.GetRad().Get()));
        }
    }

    void Light::SetSpotFalloff( const s_float& fFalloff )
    {
        if (mType_ == SPOT)
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
