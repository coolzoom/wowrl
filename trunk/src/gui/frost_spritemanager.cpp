// Ogre 2d: a small wrapper for 2d Graphics Programming in Ogre.
/*
   Wrapper for 2d Graphics in the Ogre 3d engine.

   Coded by H. Hernán Moraldo from Moraldo Games
   www.hernan.moraldo.com.ar/pmenglish/field.php

   Thanks for the Cegui team as their rendering code in Ogre gave me
   fundamental insight on the management of hardware pBuffers in Ogre.

   --------------------

   Copyright (c) 2006 Horacio Hernan Moraldo

   This software is provided 'as-is', without any express or
   implied warranty. In no event will the authors be held liable
   for any damages arising from the use of this software.

   Permission is granted to anyone to use this software for any
   purpose, including commercial applications, and to alter it and
   redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you
   must not claim that you wrote the original software. If you use
   this software in a product, an acknowledgment in the product
   documentation would be appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and
   must not be misrepresented as being the original software.

   3. This notice may not be removed or altered from any source
   distribution.

   NOTE : This code has been modified to follow Frost Engine's
   naming rules, and to allow Materials instead of plain textures.
   It has also been edited to simplify sprite rendering.

*/

#include "gui/frost_spritemanager.h"
#include "gui/frost_sprite.h"
#include "material/frost_material.h"
#include "material/frost_materialmanager.h"

#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreRenderSystem.h>
#include <OgreSceneManager.h>
#include <OgreMatrix4.h>
#include <OgreHardwareBufferManager.h>
#include <OgreHardwareVertexBuffer.h>
#include <OgreHardwarePixelBuffer.h>

#define OGRE2D_MINIMAL_HARDWARE_BUFFER_SIZE 120

using namespace std;

namespace Frost
{
    const s_str RenderTarget::CLASS_NAME = "RenderTarget";
    const s_str SpriteManager::CLASS_NAME = "SpriteManager";

    /** \cond NOT_REMOVE_FROM_DOC
    */
    struct VertexChunk
    {
        Material* pMat;
        uint      uiVertexCount;
    };
    /** \endcond
    */

    Quad::Quad()
    {
        pMat = MaterialManager::GetSingleton()->GetDefault2D();
    }

    RenderTarget::RenderTarget( const s_uint& uiID, const s_uint& uiWidth, const s_uint& uiHeight )
    {
        uiID_ = uiID;
        sName_ = "_AutoNamedTarget:"+uiID_;
        uiWidth_ = uiWidth;
        uiHeight_ = uiHeight;
        uiRealWidth_ = uiWidth.GetNearestPowerOfTwo();
        uiRealHeight_ = uiHeight.GetNearestPowerOfTwo();

        Ogre::TexturePtr pTexture = Ogre::TextureManager::getSingleton().createManual(
            sName_.Get(), "Frost", Ogre::TEX_TYPE_2D,
            uiRealWidth_.Get(), uiRealHeight_.Get(),
            0, Ogre::PF_A8B8G8R8, Ogre::TU_RENDERTARGET
        );
        hResourceHandle_ = pTexture->getHandle();

        pOgreRenderTarget_ = pTexture->getBuffer()->getRenderTarget();
        pOgreRenderTarget_->addViewport(0);
    }

    RenderTarget::RenderTarget( const s_uint& uiID, const s_str& sName, const s_uint& uiWidth, const s_uint& uiHeight )
    {
        uiID_ = uiID;
        sName_ = sName;
        uiWidth_ = uiWidth;
        uiHeight_ = uiHeight;
        uiRealWidth_ = uiWidth.GetNearestPowerOfTwo();
        uiRealHeight_ = uiHeight.GetNearestPowerOfTwo();

        Ogre::TexturePtr pTexture = Ogre::TextureManager::getSingleton().createManual(
            sName_.Get(), "Frost", Ogre::TEX_TYPE_2D,
            uiRealWidth_.Get(), uiRealHeight_.Get(),
            0, Ogre::PF_A8B8G8R8, Ogre::TU_RENDERTARGET
        );
        hResourceHandle_ = pTexture->getHandle();

        pOgreRenderTarget_ = pTexture->getBuffer()->getRenderTarget();
        pOgreRenderTarget_->addViewport(0);
    }

    RenderTarget::~RenderTarget()
    {
        Ogre::TextureManager::getSingleton().remove(hResourceHandle_);
    }

    const s_uint& RenderTarget::GetWidth() const
    {
        return uiWidth_;
    }

    const s_uint& RenderTarget::GetHeight() const
    {
        return uiHeight_;
    }

    s_bool RenderTarget::SetDimensions( const s_uint& uiWidth, const s_uint& uiHeight )
    {
        s_uint uiNewWidth = uiWidth.GetNearestPowerOfTwo();
        s_uint uiNewHeight = uiHeight.GetNearestPowerOfTwo();
        if ((uiRealWidth_ < uiNewWidth) ||
            (uiRealHeight_ < uiNewHeight))
        {
            // The current render target is too small...
            // Let's delete it and create a bigger one
            Ogre::TextureManager::getSingleton().remove(
                hResourceHandle_
            );
            uiRealWidth_ = uiNewWidth;
            uiRealHeight_ = uiNewHeight;
            Ogre::TexturePtr pTexture = Ogre::TextureManager::getSingleton().createManual(
                ("_AutoNamedTarget:"+uiID_).Get(), "Frost", Ogre::TEX_TYPE_2D,
                uiRealWidth_.Get(), uiRealHeight_.Get(),
                0, Ogre::PF_A8B8G8R8, Ogre::TU_RENDERTARGET
            );
            hResourceHandle_ = pTexture->getHandle();

            pOgreRenderTarget_ = pTexture->getBuffer()->getRenderTarget();
            pOgreRenderTarget_->addViewport(0);
            return true;
        }
        else
        {
            // There was already one, with a correct size
            // Just change its virtual size
            uiWidth_ = uiWidth;
            uiHeight_ = uiHeight;
            return false;
        }
    }

    const s_uint& RenderTarget::GetRealWidth() const
    {
        return uiRealWidth_;
    }

    const s_uint& RenderTarget::GetRealHeight() const
    {
        return uiRealHeight_;
    }

    s_ptr<Ogre::RenderTarget> RenderTarget::GetOgreRenderTarget()
    {
        return pOgreRenderTarget_;
    }

    const s_uint& RenderTarget::GetID()
    {
        return uiID_;
    }

    const s_str& RenderTarget::GetName()
    {
        return sName_;
    }

    SpriteManager::SpriteManager()
    {
        pRenderFunc_ = NULL;
        mAxisType_ = AXIS_DOWN;
    }

    SpriteManager::~SpriteManager()
    {
        Log("Closing "+CLASS_NAME+"...");
        s_map< s_uint, s_ptr<RenderTarget> >::iterator iterTarget;
        foreach (iterTarget, lRenderTargetList_)
        {
            iterTarget->second.Delete();
        }

        if (!mHardwareBuffer_.isNull())
            DestroyHardwareBuffers_();
    }

    void SpriteManager::renderQueueStarted( Ogre::uint8 uiQueueGroupId, const Ogre::String &sInvocation, bool &bSkip )
    {
    }

    void SpriteManager::renderQueueEnded( Ogre::uint8 uiQueueGroupId, const Ogre::String &sInvocation, bool &bRepeat )
    {
        if (uiQueueGroupId == Ogre::RENDER_QUEUE_OVERLAY)
        {
            RenderMainTarget_();
        }
    }

    void SpriteManager::Initialize( s_ptr<Ogre::SceneManager> pSceneMgr, const s_float& fWidth, const s_float& fHeight )
    {
        pSceneMgr_ = pSceneMgr;

        mHardwareBuffer_.setNull();
        mColorBuffer_.setNull();

        pRS_ = Ogre::Root::getSingletonPtr()->getRenderSystem();

        f2_ScreenWidth_ = 2.0f/fWidth;
        f2_ScreenHeight_ = 2.0f/fHeight;

        fXOffset_ = pRS_->getHorizontalTexelOffset()/fWidth;
        fYOffset_ = pRS_->getVerticalTexelOffset()/fHeight;

        pSceneMgr_->addRenderQueueListener(this);

        pMainTarget_ = CreateRenderTarget(
            "_TargetMain", s_uint(fWidth), s_uint(fHeight)
        );
        s_refptr<Material> pMat = MaterialManager::GetSingleton()->CreateMaterial2DFromRT("_TargetMain");

        pMainSprite_ = s_refptr<Sprite>(new Sprite(
            pMat, fWidth, fHeight
        ));

        bRenderTargets_ = true;
    }

    void SpriteManager::SetWindowSize( const s_float& fWidth, const s_float& fHeight )
    {
        f2_ScreenWidth_ = 2.0f/fWidth;
        f2_ScreenHeight_ = 2.0f/fHeight;

        fXOffset_ = pRS_->getHorizontalTexelOffset()/fWidth;
        fYOffset_ = pRS_->getVerticalTexelOffset()/fHeight;

        s_uint uiWidth = s_uint(fWidth);
        s_uint uiHeight = s_uint(fHeight);
        if (!pMainTarget_)
        {
            pMainTarget_ = CreateRenderTarget(
                "_TargetMain", uiWidth, uiHeight
            );
        }
        else
        {
            pMainTarget_->SetDimensions(uiWidth, uiHeight);
        }

        pMainSprite_ = s_refptr<Sprite>(new Sprite(
            pMainSprite_->GetMaterial(), fWidth, fHeight
        ));
    }

    /** \cond NOT_REMOVE_FROM_DOC
    */
    inline void WriteVertex( float* &pBuffer, uint* &pColorBuffer, Frost::Vertex &mV )
    {
        *pBuffer = s_float::Round(mV.fX).Get();  // x
        pBuffer++;
        *pBuffer = s_float::Round(mV.fY).Get();  // y
        pBuffer++;
        *pBuffer = -1;           // z
        pBuffer++;

        *pBuffer = mV.fTX.Get(); // u
        pBuffer++;
        *pBuffer = mV.fTY.Get(); // v
        pBuffer++;

        *pColorBuffer = mV.mColor.GetPacked().Get();  // argb
        pColorBuffer++;
    }
    /** \endcond
    */

    void SpriteManager::RenderBuffers_( s_bool bCallBeginEnd )
    {
        if (lQuadList_.IsEmpty())
            return;

        s_ctnr<Quad>::iterator itCurrQuad, itEndQuad;

        VertexChunk mThisChunk;
        s_ctnr<VertexChunk> lChunkList;

        uint uiNewSize = lQuadList_.GetSize().Get()*6;

        if (uiNewSize < OGRE2D_MINIMAL_HARDWARE_BUFFER_SIZE)
            uiNewSize = OGRE2D_MINIMAL_HARDWARE_BUFFER_SIZE;

        // Grow hardware buffer if needed
        if ( mHardwareBuffer_.isNull() || (mHardwareBuffer_->getNumVertices() < uiNewSize) )
        {
            if (!mHardwareBuffer_.isNull())
                DestroyHardwareBuffers_();

            CreateHardwareBuffers_(uiNewSize);
        }

        // Write quads to the hardware buffer, and remember chunks
        float* pBuffer = (float*)mHardwareBuffer_->lock(Ogre::HardwareBuffer::HBL_DISCARD);
        uint*  pColorBuffer = (uint*)mColorBuffer_->lock(Ogre::HardwareBuffer::HBL_DISCARD);

        itEndQuad = lQuadList_.End();
        itCurrQuad = lQuadList_.Begin();
        mThisChunk.pMat = itCurrQuad->pMat.Get();
        mThisChunk.uiVertexCount = 0u;
        while (itCurrQuad != itEndQuad)
        {
            WriteVertex(pBuffer, pColorBuffer, itCurrQuad->lVertexArray[0]);
            WriteVertex(pBuffer, pColorBuffer, itCurrQuad->lVertexArray[1]);
            WriteVertex(pBuffer, pColorBuffer, itCurrQuad->lVertexArray[2]);

            WriteVertex(pBuffer, pColorBuffer, itCurrQuad->lVertexArray[2]);
            WriteVertex(pBuffer, pColorBuffer, itCurrQuad->lVertexArray[3]);
            WriteVertex(pBuffer, pColorBuffer, itCurrQuad->lVertexArray[0]);

            // Remember this chunk
            mThisChunk.uiVertexCount += 6u;
            itCurrQuad++;
            if ( (itCurrQuad == itEndQuad) || (mThisChunk.pMat->GetOgreMaterial() != itCurrQuad->pMat->GetOgreMaterial()) )
            {
                lChunkList.PushBack(mThisChunk);
                if (itCurrQuad != itEndQuad)
                {
                    mThisChunk.pMat = itCurrQuad->pMat.Get();
                    mThisChunk.uiVertexCount = 0u;
                }
            }
        }

        mHardwareBuffer_->unlock();
        mColorBuffer_->unlock();

        // Do the real render!
        s_ctnr<VertexChunk>::iterator itCurrChunk;

        if (bCallBeginEnd)
            Ogre::Root::getSingleton().getRenderSystem()->_beginFrame();

        mRenderOp_.vertexData->vertexStart = 0;
        foreach (itCurrChunk, lChunkList)
        {
            mRenderOp_.vertexData->vertexCount = itCurrChunk->uiVertexCount;
            s_ptr<Ogre::Technique> pTech = itCurrChunk->pMat->GetOgreMaterial()->getTechnique(0);

            for (uint i = 0; i < pTech->getNumPasses(); i++)
            {
                Ogre::Pass* pPass = pTech->getPass(i);
                pSceneMgr_->_setPass(pPass);
                if (pPass->isProgrammable())
                {
                    Ogre::AutoParamDataSource mDataSource;
                    mDataSource.setCurrentViewport(pRenderTarget_->GetOgreRenderTarget()->getViewport(0));
                    mDataSource.setCurrentRenderTarget(pRenderTarget_->GetOgreRenderTarget().Get());
                    mDataSource.setCurrentSceneManager(pSceneMgr_.Get());
                    mDataSource.setWorldMatrices(&Ogre::Matrix4::IDENTITY, 1);
                    mDataSource.setCurrentPass(pPass);
                    pPass->_updateAutoParamsNoLights(&mDataSource);
                    if (pPass->hasFragmentProgram())
                    {
                        pRS_->bindGpuProgramParameters(Ogre::GPT_FRAGMENT_PROGRAM,
                            pPass->getFragmentProgramParameters());
                    }
                }
                pRS_->_render(mRenderOp_);
            }

            mRenderOp_.vertexData->vertexStart += itCurrChunk->uiVertexCount;
        }

        lQuadList_.Clear();

        if (bCallBeginEnd)
            Ogre::Root::getSingleton().getRenderSystem()->_endFrame();
    }

    void SpriteManager::PrepareForRender_( s_bool bGeneral )
    {
        if (bGeneral)
        {
            Ogre::LayerBlendModeEx mColorBlendMode;
            Ogre::LayerBlendModeEx mAlphaBlendMode;
            Ogre::TextureUnitState::UVWAddressingMode mUVWAddressMode;

            mColorBlendMode.blendType = Ogre::LBT_COLOUR;
            mColorBlendMode.source1   = Ogre::LBS_TEXTURE;
            mColorBlendMode.operation = Ogre::LBX_SOURCE1;

            mAlphaBlendMode.blendType = Ogre::LBT_ALPHA;
            mAlphaBlendMode.source1   = Ogre::LBS_TEXTURE;
            mAlphaBlendMode.operation = Ogre::LBX_SOURCE1;

            mUVWAddressMode.u = Ogre::TextureUnitState::TAM_CLAMP;
            mUVWAddressMode.v = Ogre::TextureUnitState::TAM_CLAMP;
            mUVWAddressMode.w = Ogre::TextureUnitState::TAM_CLAMP;

            pRS_->_setWorldMatrix(Ogre::Matrix4::IDENTITY);
            pRS_->_setViewMatrix(Ogre::Matrix4::IDENTITY);
            pRS_->_setTextureMatrix(0, Ogre::Matrix4::IDENTITY);
            pRS_->_setTextureCoordSet(0, 0);
            pRS_->_setTextureCoordCalculation(0, Ogre::TEXCALC_NONE);
            pRS_->_setTextureBlendMode(0, mColorBlendMode);
            pRS_->_setTextureBlendMode(0, mAlphaBlendMode);
            pRS_->_setTextureAddressingMode(0, mUVWAddressMode);
            pRS_->_disableTextureUnitsFrom(1);
            pRS_->setLightingEnabled(false);
            pRS_->_setFog(Ogre::FOG_NONE);
            pRS_->_setDepthBufferParams(false, false);
            pRS_->_setColourBufferWriteEnabled(true, true, true, false);
            pRS_->setShadingType(Ogre::SO_GOURAUD);
            pRS_->_setPolygonMode(Ogre::PM_SOLID);
            pRS_->unbindGpuProgram(Ogre::GPT_FRAGMENT_PROGRAM);
            pRS_->unbindGpuProgram(Ogre::GPT_VERTEX_PROGRAM);
            pRS_->_setAlphaRejectSettings(Ogre::CMPF_ALWAYS_PASS, 0, false);
        }
        else
        {
            Ogre::Matrix4 mProj = Ogre::Matrix4::IDENTITY;
            if (mAxisType_ == AXIS_DOWN)
            {
                if (bRenderTargets_)
                {
                    mProj.setScale(Ogre::Vector3(
                        2.0f/pRenderTarget_->GetRealWidth().Get(),
                        -2.0f/pRenderTarget_->GetRealHeight().Get(),
                        1.0f
                    ));
                }
                else
                {
                    mProj.setScale(Ogre::Vector3(
                        f2_ScreenWidth_.Get(),
                        -f2_ScreenHeight_.Get(),
                        1.0f
                    ));
                }
                mProj = Ogre::Matrix4::getTrans(
                    fXOffset_.Get() - 1.0f,
                    fYOffset_.Get() + 1.0f,
                    0.0f
                )*mProj;
            }
            else
            {
                if (bRenderTargets_)
                {
                    mProj.setScale(Ogre::Vector3(
                        2.0f/pRenderTarget_->GetRealWidth().Get(),
                        2.0f/pRenderTarget_->GetRealHeight().Get(),
                        1.0f
                    ));
                    mProj = Ogre::Matrix4::getTrans(
                        fXOffset_.Get() - 1.0f,
                        (-2.0f*pRenderTarget_->GetHeight().Get())/pRenderTarget_->GetRealHeight().Get() + fYOffset_.Get() + 1.0f,
                        0.0f
                    )*mProj;
                }
                else
                {
                    mProj.setScale(Ogre::Vector3(
                        f2_ScreenWidth_.Get(),
                        f2_ScreenHeight_.Get(),
                        1.0f
                    ));
                    mProj = Ogre::Matrix4::getTrans(
                        fXOffset_.Get() - 1.0f,
                        fYOffset_.Get() - 1.0f,
                        0.0f
                    )*mProj;
                }
            }

            pRS_->_setProjectionMatrix(mProj);
            pRS_->_setCullingMode(Ogre::CULL_NONE);
        }
    }

    void SpriteManager::Begin( s_ptr<RenderTarget> pRenderTarget )
    {
        if (!bFrameStarted_)
        {
            // Set a new render target
            if (!pRenderTarget)
            {
                // The main target...
                pRenderTarget_ = pMainTarget_;
            }
            else
            {
                // ... or the provided one if any
                pRenderTarget_ = pRenderTarget;
            }

            pRS_->_setViewport(pRenderTarget_->GetOgreRenderTarget()->getViewport(0));

            PrepareForRender_(false);

            bFrameStarted_ = true;
        }
        else
        {
            Warning(CLASS_NAME,
                "Calling Begin(), but there is already something beeing rendered... ("+
                s_str(pRenderTarget.IsValid() ? pRenderTarget->GetName() : "main target")+")."
            );
        }
    }

    void SpriteManager::End()
    {
        if (bFrameStarted_)
        {
            // Draw everything on the render target
            RenderBuffers_(true);

            pRenderTarget_ = NULL;
            bFrameStarted_ = false;
        }
        else
        {
            Warning(CLASS_NAME,
                "Calling End(), but there was no corresponding Begin() call..."
            );
        }
    }

    void SpriteManager::Clear( const Color& mColor )
    {
        Ogre::Root::getSingleton().getRenderSystem()->clearFrameBuffer(
            Ogre::FBT_COLOUR, Color::FrostToOgre(mColor)
        );
    }

    s_bool SpriteManager::RenderMainTarget_()
    {
        bRenderTargets_ = false;

        // Set up...
        PrepareForRender_(true);
        PrepareForRender_(false);

        pMainSprite_->Render(0, 0);

        RenderBuffers_(false);

        bRenderTargets_ = true;

        return true;
    }

    s_bool SpriteManager::RenderTargets()
    {
        if (pRenderFunc_)
        {
            // Set up...
            PrepareForRender_(true);

            return (*pRenderFunc_)();
        }

        return true;
    }

    void SpriteManager::CreateHardwareBuffers_( uint uiSize )
    {
        Ogre::VertexDeclaration* pDecl;

        mRenderOp_.vertexData = new Ogre::VertexData;
        mRenderOp_.vertexData->vertexStart = 0;

        size_t uiOffset = 0;
        pDecl = mRenderOp_.vertexData->vertexDeclaration;
        // Position
        pDecl->addElement(0, 0, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
        uiOffset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);

        // Texture coordinates
        pDecl->addElement(0, uiOffset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES);

        // Color
        pDecl->addElement(1, 0, Ogre::VET_COLOUR_ARGB, Ogre::VES_DIFFUSE);

        mHardwareBuffer_ = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
            pDecl->getVertexSize(0),
            uiSize,
            Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE,
            false
        );

        mColorBuffer_ = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
            pDecl->getVertexSize(1),
            uiSize,
            Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE,
            false
        );

        mRenderOp_.vertexData->vertexBufferBinding->setBinding(0, mHardwareBuffer_);
        mRenderOp_.vertexData->vertexBufferBinding->setBinding(1, mColorBuffer_);

        mRenderOp_.operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;
        mRenderOp_.useIndexes = false;
    }

    void SpriteManager::DestroyHardwareBuffers_()
    {
        delete mRenderOp_.vertexData;
        mRenderOp_.vertexData = 0;
        mHardwareBuffer_.setNull();
        mColorBuffer_.setNull();
    }

    void SpriteManager::RenderQuad( const Quad &mQuad )
    {
        lQuadList_.PushBack(mQuad);
    }

    void SpriteManager::SetRenderFunction( Function pRenderFunc )
    {
        pRenderFunc_ = pRenderFunc;
    }

    void SpriteManager::SetYAxisType( const AxisType& mAxis )
    {
        mAxisType_ = mAxis;
    }

    const AxisType& SpriteManager::GetYAxisType() const
    {
        return mAxisType_;
    }

    s_ptr<RenderTarget> SpriteManager::CreateRenderTarget( const s_str& sTargetName, const s_uint& uiWidth, const s_uint& uiHeight )
    {
        s_ptr<RenderTarget> pRTarget = lRenderTargetList_[uiTargetCounter_] = new RenderTarget(
            uiTargetCounter_, sTargetName, uiWidth, uiHeight
        );
        uiTargetCounter_++;

        return pRTarget;
    }

    s_ptr<RenderTarget> SpriteManager::CreateRenderTarget( const s_uint& uiWidth, const s_uint& uiHeight )
    {
        s_ptr<RenderTarget> pRTarget = lRenderTargetList_[uiTargetCounter_] = new RenderTarget(
            uiTargetCounter_, uiWidth, uiHeight
        );
        uiTargetCounter_++;

        return pRTarget;
    }

    void SpriteManager::DeleteRenderTarget( s_ptr<RenderTarget> pTarget )
    {
        if (pTarget)
        {
            s_map< s_uint, s_ptr<RenderTarget> >::iterator iterRenderTarget;
            iterRenderTarget = lRenderTargetList_.Get(pTarget->GetID());

            if (iterRenderTarget != lRenderTargetList_.End())
            {
                if (iterRenderTarget->second->GetID() == pTarget->GetID())
                {
                    // Everything went fine, delete, erase from map and return
                    iterRenderTarget->second.Delete();
                    lRenderTargetList_.Erase(iterRenderTarget);
                    return;
                }
            }

            Warning(CLASS_NAME,
                "Trying to call DeleteRenderTarget on a RenderTarget that has not been created by SpriteManager (ID:"+pTarget->GetID()+")."
            );
        }
    }
}
