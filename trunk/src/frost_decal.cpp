#include "frost_decal.h"

#include <OgreFrustum.h>
#include <OgreSceneNode.h>
#include <OgreTechnique.h>
#include <OgrePass.h>

namespace Frost
{
    s_str Decal::CLASS_NAME = "Decal";

    Decal::Decal( const s_str& sTextureFile, const s_uint& uiID, s_ptr<Ogre::Material> pOgreMat )
    {
        uiID_ = uiID;
        pOgreMat_ = pOgreMat;
        if (pOgreMat_)
        {
            pOgrePass_ = pOgreMat_->getTechnique(0)->createPass();
            pOgrePass_->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
            pOgrePass_->setDepthBias(1, 0.1);
        }
        sTextureFile_ = sTextureFile;
        pOgreFrustum_ = s_refptr<Ogre::Frustum>(new Ogre::Frustum());
        pOgreFrustum_->setAspectRatio(1.0f);
        pNode_->attachObject(pOgreFrustum_.Get());
        mProjection_ = DECAL_PROJECTION_PERSP;
        fScale_ = 1.0f;
    }

    Decal::Decal( const Decal& mDecal, const s_uint& uiID, s_ptr<Ogre::Material> pOgreMat ) : MovableObject(mDecal)
    {
        uiID_ = uiID;
        pOgreMat_ = pOgreMat;
        pOgrePass_ = pOgreMat_->getTechnique(0)->createPass();
        pOgrePass_->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
        pOgrePass_->setDepthBias(1, 0.1);
        sTextureFile_ = mDecal.sTextureFile_;
        pOgreFrustum_ = s_refptr<Ogre::Frustum>(new Ogre::Frustum());
        pOgreFrustum_->setAspectRatio(1.0f);
        pNode_->attachObject(pOgreFrustum_.Get());
        mProjection_ = DECAL_PROJECTION_PERSP;
        fScale_ = 1.0f;
    }

    Decal::~Decal()
    {
        this->Hide();
        pOgreMat_->getTechnique(0)->removePass(pOgrePass_->getIndex());
    }


    void Decal::SetScale( const s_float& fScale )
    {
        fScale_ = fScale;
        switch (mProjection_)
        {
            case DECAL_PROJECTION_PERSP :
            {
                pOgreFrustum_->setFOVy(Ogre::Degree(45/fScale_.Get()));
                break;
            }
            case DECAL_PROJECTION_ORTHO :
            {
                pOgreFrustum_->setOrthoWindow(fScale_.Get(), fScale_.Get());
                break;
            }
        }
    }

    const s_float& Decal::GetScale() const
    {
        return fScale_;
    }

    void Decal::SetDiffuse( const Color& mColor )
    {
        pOgrePass_->setDiffuse(
            mColor.GetR().Get()/255.0f,
            mColor.GetG().Get()/255.0f,
            mColor.GetB().Get()/255.0f,
            mColor.GetA().Get()/255.0f
        );
    }

    Color Decal::GetDiffuse() const
    {
        return Color(pOgrePass_->getDiffuse().getAsARGB());
    }

    void Decal::SetSelfIllumination( const Color& mColor )
    {
        pOgrePass_->setSelfIllumination(
            mColor.GetR().Get()/255.0f,
            mColor.GetG().Get()/255.0f,
            mColor.GetB().Get()/255.0f
        );
    }

    void Decal::SetAmbient( const Color& mColor )
    {
        pOgrePass_->setAmbient(
            mColor.GetR().Get()/255.0f,
            mColor.GetG().Get()/255.0f,
            mColor.GetB().Get()/255.0f
        );
    }

    void Decal::SetProjection( const DecalProjection& mProjection )
    {
        mProjection_ = mProjection;
        switch (mProjection_)
        {
            case DECAL_PROJECTION_PERSP :
            {
                pOgreFrustum_->setProjectionType(Ogre::PT_PERSPECTIVE);
                pOgreFrustum_->setFOVy(Ogre::Degree(45/fScale_.Get()));
                break;
            }
            case DECAL_PROJECTION_ORTHO :
            {
                pOgreFrustum_->setProjectionType(Ogre::PT_ORTHOGRAPHIC);
                pOgreFrustum_->setOrthoWindow(fScale_.Get(), fScale_.Get());
                break;
            }
        }
    }

    const DecalProjection& Decal::GetProjection() const
    {
        return mProjection_;
    }

    void Decal::Show()
    {
        if (pOgrePass_ && !bShown_)
        {
            //pOgrePass_->setAlphaRejectFunction(Ogre::CMPF_ALWAYS_PASS);
            //pOgrePass_->setDepthCheckEnabled(false);
            pTUS_ = pOgrePass_->createTextureUnitState(sTextureFile_.Get());
            pTUS_->setProjectiveTexturing(true, pOgreFrustum_.Get());
            pTUS_->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);
            pTUS_->setTextureFiltering(Ogre::FO_POINT, Ogre::FO_LINEAR, Ogre::FO_NONE);

            bShown_ = true;
        }
    }

    void Decal::Hide()
    {
        if (pOgrePass_ && bShown_)
        {
            pOgrePass_->removeTextureUnitState(pOgrePass_->getTextureUnitStateIndex(pTUS_.Get()));
            /*if (pOgrePass_->getNumTextureUnitStates() == 0)
            {
                pOgrePass_->setAlphaRejectFunction(Ogre::CMPF_ALWAYS_FAIL);
            }*/

            bShown_ = false;
        }
    }

    const s_uint& Decal::GetID() const
    {
        return uiID_;
    }

    s_ptr<Ogre::Pass> Decal::GetOgrePass()
    {
        return pOgrePass_;
    }

    s_refptr<Ogre::Frustum> Decal::GetOgreFrustum()
    {
        return pOgreFrustum_;
    }

    const s_str& Decal::GetTextureFile() const
    {
        return sTextureFile_;
    }
}
