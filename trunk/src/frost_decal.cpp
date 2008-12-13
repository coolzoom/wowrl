#include "frost_decal.h"

#include <OgreFrustum.h>
#include <OgreSceneNode.h>
#include <OgrePass.h>

namespace Frost
{
    s_str Decal::CLASS_NAME = "Decal";

    Decal::Decal( const s_str& sTextureFile, const s_uint& uiID, s_ptr<Ogre::Pass> pPass )
    {
        uiID_ = uiID;
        pOgrePass_ = pPass;
        sTextureFile_ = sTextureFile;
        pOgreFrustum_ = s_refptr<Ogre::Frustum>(new Ogre::Frustum());
        pNode_->attachObject(pOgreFrustum_.Get());
    }

    Decal::Decal( const Decal& mDecal, const s_uint& uiID, s_ptr<Ogre::Pass> pPass ) : MovableObject(mDecal)
    {
        uiID_ = uiID;
        pOgrePass_ = pPass;
        sTextureFile_ = mDecal.sTextureFile_;
        pOgreFrustum_ = s_refptr<Ogre::Frustum>(new Ogre::Frustum());
        pNode_->attachObject(pOgreFrustum_.Get());
    }

    Decal::~Decal()
    {
        this->Hide();
    }

    void Decal::Show()
    {
        if (pOgrePass_ && !bShown_)
        {
            pOgrePass_->setAlphaRejectFunction(Ogre::CMPF_ALWAYS_PASS);
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
            if (pOgrePass_->getNumTextureUnitStates() == 0)
            {
                pOgrePass_->setAlphaRejectFunction(Ogre::CMPF_ALWAYS_FAIL);
            }
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
