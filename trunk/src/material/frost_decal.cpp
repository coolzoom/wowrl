/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              Decal source              */
/*                                        */
/*                                        */

#include "material/frost_decal.h"
#include "material/frost_material.h"
#include "material/frost_shadermanager.h"
#include "material/frost_shader.h"

#include <OgreFrustum.h>
#include <OgreSceneNode.h>
#include <OgreTechnique.h>
#include <OgrePass.h>

namespace Frost
{
    s_str Decal::CLASS_NAME = "Decal";

    Decal::Decal( const s_str& sTextureFile )
    {
        sTextureFile_ = sTextureFile;
        pOgreFrustum_ = s_refptr<Ogre::Frustum>(new Ogre::Frustum());
        pOgreFrustum_->setAspectRatio(1.0f);
        pNode_->attachObject(pOgreFrustum_.Get());
        mProjection_ = PROJ_PERSPECTIVE;
        fScale_ = 1.0f;
        mDiffuse_ = Color::WHITE;
        mSelfIllum_ = Color::BLACK;
        mAmbient_ = Color::BLACK;
    }

    Decal::Decal( const Decal& mDecal ) : MovableObject(mDecal)
    {
        sTextureFile_ = mDecal.sTextureFile_;
        pOgreFrustum_ = s_refptr<Ogre::Frustum>(new Ogre::Frustum());
        pOgreFrustum_->setAspectRatio(1.0f);
        pNode_->attachObject(pOgreFrustum_.Get());
        SetProjection(mDecal.mProjection_);
        SetScale(mDecal.fScale_);
        SetDiffuse(mDecal.mDiffuse_);
        SetSelfIllumination(mDecal.mSelfIllum_);
        SetAmbient(mDecal.mAmbient_);
    }

    Decal::~Decal()
    {
        s_map<s_uint, MaterialInfo>::iterator iter;
        foreach (iter, lMaterialList_)
        {
            if (s_refptr<Material> pLocked = iter->second.pMat.Lock())
            {
                Hide(iter->second.pMat);
                iter->second.pOgreMat->getTechnique(0)->removePass(iter->second.pOgrePass->getIndex());
            }
        }

        if (pOgreFrustum_)
            pNode_->detachObject(pOgreFrustum_.Get());
    }

    void Decal::AddMaterial( s_wptr<Material> pMat )
    {
        if (s_refptr<Material> pLocked = pMat.Lock())
        {
            MaterialInfo mInfo;
            mInfo.pMat      = pMat;
            mInfo.pOgreMat  = pLocked->GetOgreMaterial();
            mInfo.pOgrePass = mInfo.pOgreMat->getTechnique(0)->createPass();

            mInfo.pOgrePass->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
            mInfo.pOgrePass->setDepthBias(1.0f, 0.1f);
            mInfo.pOgrePass->setDiffuse(
                mDiffuse_.GetR().Get()/255.0f,
                mDiffuse_.GetG().Get()/255.0f,
                mDiffuse_.GetB().Get()/255.0f,
                mDiffuse_.GetA().Get()/255.0f
            );
            mInfo.pOgrePass->setSelfIllumination(
                mSelfIllum_.GetR().Get()/255.0f,
                mSelfIllum_.GetG().Get()/255.0f,
                mSelfIllum_.GetB().Get()/255.0f
            );
            mInfo.pOgrePass->setAmbient(
                mAmbient_.GetR().Get()/255.0f,
                mAmbient_.GetG().Get()/255.0f,
                mAmbient_.GetB().Get()/255.0f
            );
            mInfo.pOgrePass->setAlphaRejectFunction(Ogre::CMPF_ALWAYS_FAIL);

            lMaterialList_[pLocked->GetID()] = mInfo;

            if (bShown_)
                Show(pMat);
        }
    }

    void Decal::RemoveMaterial( s_wptr<Material> pMat )
    {
        if (s_refptr<Material> pLocked = pMat.Lock())
        {
            s_map<s_uint, MaterialInfo>::iterator iter = lMaterialList_.Get(pLocked->GetID());
            if (iter != lMaterialList_.End())
            {
                Hide(pMat);
                iter->second.pOgreMat->getTechnique(0)->removePass(iter->second.pOgrePass->getIndex());
                lMaterialList_.Erase(iter);
            }
        }
    }

    void Decal::SetTextureFile( const s_str& sFile )
    {
        if (sFile != sTextureFile_)
        {
            sTextureFile_ = sFile;
            s_map<s_uint, MaterialInfo>::iterator iter;
            foreach (iter, lMaterialList_)
            {
                if (iter->second.bShown)
                {
                    iter->second.pTUS->setTextureName(sTextureFile_.Get());
                }
            }
        }
    }

    const s_str& Decal::GetTextureFile() const
    {
        return sTextureFile_;
    }

    void Decal::SetScale( const s_float& fScale )
    {
        fScale_ = fScale;
        switch (mProjection_)
        {
            case PROJ_PERSPECTIVE :
            {
                pOgreFrustum_->setFOVy(Ogre::Degree(45/fScale_.Get()));
                break;
            }
            case PROJ_ORTHOGRAPHIC :
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
        mDiffuse_ = mColor;
        s_map<s_uint, MaterialInfo>::iterator iter;
        foreach (iter, lMaterialList_)
        {
            iter->second.pOgrePass->setDiffuse(
                mDiffuse_.GetR().Get()/255.0f,
                mDiffuse_.GetG().Get()/255.0f,
                mDiffuse_.GetB().Get()/255.0f,
                mDiffuse_.GetA().Get()/255.0f
            );
        }
    }

    const Color& Decal::GetDiffuse() const
    {
        return mDiffuse_;
    }

    void Decal::SetSelfIllumination( const Color& mColor )
    {
        mSelfIllum_ = mColor;
        s_map<s_uint, MaterialInfo>::iterator iter;
        foreach (iter, lMaterialList_)
        {
            iter->second.pOgrePass->setSelfIllumination(
                mSelfIllum_.GetR().Get()/255.0f,
                mSelfIllum_.GetG().Get()/255.0f,
                mSelfIllum_.GetB().Get()/255.0f
            );
        }
    }

    const Color& Decal::GetSelfIllumination() const
    {
        return mSelfIllum_;
    }

    void Decal::SetAmbient( const Color& mColor )
    {
        mAmbient_ = mColor;
        s_map<s_uint, MaterialInfo>::iterator iter;
        foreach (iter, lMaterialList_)
        {
            iter->second.pOgrePass->setAmbient(
                mAmbient_.GetR().Get()/255.0f,
                mAmbient_.GetG().Get()/255.0f,
                mAmbient_.GetB().Get()/255.0f
            );
        }
    }

    const Color& Decal::GetAmbient() const
    {
        return mAmbient_;
    }

    void Decal::SetProjection( const Projection& mProjection )
    {
        mProjection_ = mProjection;
        switch (mProjection_)
        {
            case PROJ_PERSPECTIVE :
            {
                pOgreFrustum_->setProjectionType(Ogre::PT_PERSPECTIVE);
                pOgreFrustum_->setFOVy(Ogre::Degree(45.0f/fScale_.Get()));
                break;
            }
            case PROJ_ORTHOGRAPHIC :
            {
                pOgreFrustum_->setProjectionType(Ogre::PT_ORTHOGRAPHIC);
                pOgreFrustum_->setOrthoWindow(fScale_.Get(), fScale_.Get());
                break;
            }
        }
    }

    const Decal::Projection& Decal::GetProjection() const
    {
        return mProjection_;
    }

    void Decal::Show( s_wptr<Material> pMat )
    {
        if (s_refptr<Material> pLocked = pMat.Lock())
        {
            s_map<s_uint, MaterialInfo>::iterator iter = lMaterialList_.Get(pLocked->GetID());
            if (iter != lMaterialList_.End())
            {
                if (!iter->second.bShown)
                {
                    iter->second.pOgrePass->setAlphaRejectFunction(Ogre::CMPF_ALWAYS_PASS);
                    iter->second.pTUS = iter->second.pOgrePass->createTextureUnitState(sTextureFile_.Get());
                    iter->second.pTUS->setProjectiveTexturing(true, pOgreFrustum_.Get());
                    iter->second.pTUS->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);
                    iter->second.pTUS->setTextureFiltering(Ogre::FO_POINT, Ogre::FO_LINEAR, Ogre::FO_NONE);
                    iter->second.bShown = true;
                }
            }
        }
        else
        {
            s_map<s_uint, MaterialInfo>::iterator iter;
            foreach (iter, lMaterialList_)
            {
                Show(iter->second.pMat);
            }

            bShown_ = true;
        }
    }

    void Decal::Hide( s_wptr<Material> pMat )
    {
        if (s_refptr<Material> pLocked = pMat.Lock())
        {
            s_map<s_uint, MaterialInfo>::iterator iter = lMaterialList_.Get(pLocked->GetID());
            if (iter != lMaterialList_.End())
            {
                if (iter->second.bShown)
                {
                    iter->second.pOgrePass->removeTextureUnitState(
                        iter->second.pOgrePass->getTextureUnitStateIndex(iter->second.pTUS.Get())
                    );
                    iter->second.pOgrePass->setAlphaRejectFunction(Ogre::CMPF_ALWAYS_FAIL);
                    iter->second.bShown = false;
                }
            }
        }
        else
        {
            s_map<s_uint, MaterialInfo>::iterator iter;
            foreach (iter, lMaterialList_)
            {
                Hide(iter->second.pMat);
            }

            bShown_ = false;
        }
    }

    const s_bool& Decal::IsShown() const
    {
        return bShown_;
    }

    const s_uint& Decal::GetID() const
    {
        return uiID_;
    }

    s_refptr<Ogre::Frustum> Decal::GetOgreFrustum()
    {
        return pOgreFrustum_;
    }
}
