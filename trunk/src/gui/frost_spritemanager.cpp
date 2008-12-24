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

#include "frost_spritemanager.h"
#include "frost_sprite.h"
#include "frost_material.h"
#include "frost_materialmanager.h"

#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreRenderSystem.h>
#include <OgreSceneManager.h>
#include <OgreMatrix4.h>
#include <OgreHardwareBufferManager.h>
#include <OgreHardwareVertexBuffer.h>

#define OGRE2D_MINIMAL_HARDWARE_BUFFER_SIZE 120

using namespace std;

namespace Frost
{
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

    SpriteManager::SpriteManager()
    {
        pRenderFunc_ = NULL;
        mAxisType_ = AXIS_UP;
    }

    SpriteManager::~SpriteManager()
    {
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

        mMainTarget_ = GFX::CreateRenderTarget(
            "_TargetMain", s_uint(fWidth), s_uint(fHeight)
        );
        s_refptr<Material> pMat = MaterialManager::GetSingleton()->CreateMaterial2DFromRT("_TargetMain");

        pMainSprite_ = s_refptr<Sprite>(new Sprite(
            pMat, fWidth, fHeight
        ));
    }

    void SpriteManager::SetWindowSize( const s_float& fWidth, const s_float& fHeight )
    {
        f2_ScreenWidth_ = 2.0f/fWidth;
        f2_ScreenHeight_ = 2.0f/fHeight;

        fXOffset_ = pRS_->getHorizontalTexelOffset()/fWidth;
        fYOffset_ = pRS_->getVerticalTexelOffset()/fHeight;

        s_uint uiWidth = s_uint(fWidth);
        s_uint uiHeight = s_uint(fHeight);
        if (!mMainTarget_.pOgreRenderTarget)
        {
            // There was no main render target
            // Let's create one
            mMainTarget_ = GFX::CreateRenderTarget(
                "_TargetMain", uiWidth, uiHeight
            );
        }
        else if ((s_uint((uint)mMainTarget_.pOgreRenderTarget->getWidth()) < uiWidth.GetNearestPowerOfTwo()) ||
            (s_uint((uint)mMainTarget_.pOgreRenderTarget->getHeight()) < uiHeight.GetNearestPowerOfTwo()))
        {
            // There was already one, but it was too small
            // Let's delete the old one and create another bigger one
            GFX::DeleteRenderTarget("_TargetMain");
            mMainTarget_ = GFX::CreateRenderTarget(
                "_TargetMain", uiWidth, uiHeight
            );
        }
        else
        {
            // There was already one, with a correct size
            // Just change its virtual size
            mMainTarget_.uiWidth = uiWidth;
            mMainTarget_.uiHeight = uiHeight;
        }

        pMainSprite_ = s_refptr<Sprite>(new Sprite(
            pMainSprite_->GetMaterial(), fWidth, fHeight
        ));
    }

    /** \cond NOT_REMOVE_FROM_DOC
    */
    void WriteVertex( float* &pBuffer, uint* &pColorBuffer, Frost::Vertex &mV )
    {
        *pBuffer = mV.fX.Get();  // x
        pBuffer++;
        *pBuffer = mV.fY.Get();  // y
        pBuffer++;
        *pBuffer = -1;     // z
        pBuffer++;

        *pBuffer = mV.fTX.Get(); // u
        pBuffer++;
        *pBuffer = mV.fTY.Get(); // v
        pBuffer++;

        *pColorBuffer = mV.mColor.Get().Get();  // argb
        pColorBuffer++;
    }
    /** \endcond
    */

    void SpriteManager::RenderBuffers_( s_bool bCallBeginEnd )
    {
        if (lQuadList_.empty())
            return;

        vector<Quad>::iterator itCurrQuad, itEndQuad;

        VertexChunk mThisChunk;
        list<VertexChunk> lChunkList;

        uint uiNewSize = lQuadList_.size()*6;

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

        itEndQuad = lQuadList_.end();
        itCurrQuad = lQuadList_.begin();
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
            if ((itCurrQuad == itEndQuad) ||
                (mThisChunk.pMat->GetOgreMaterial() != itCurrQuad->pMat->GetOgreMaterial()))
            {
                lChunkList.push_back(mThisChunk);
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
        list<VertexChunk>::iterator itCurrChunk;

        mRenderOp_.vertexData->vertexStart = 0;
        foreach (itCurrChunk, lChunkList)
        {
            mRenderOp_.vertexData->vertexCount = itCurrChunk->uiVertexCount;

            for (uint i = 0; i < itCurrChunk->pMat->GetOgreMaterial()->getTechnique(0)->getNumPasses(); i++)
            {
                pSceneMgr_->manualRender(
                    &mRenderOp_,
                    itCurrChunk->pMat->GetOgreMaterial()->getTechnique(0)->getPass(i),
                    bCallBeginEnd
                );
            }

            mRenderOp_.vertexData->vertexStart += itCurrChunk->uiVertexCount;
        }

        // Sprites go home!
        lQuadList_.clear();
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
            pRS_->_setProjectionMatrix(Ogre::Matrix4::IDENTITY);
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
                mRenderTarget_ = mMainTarget_;
            }
            else
            {
                // ... or the provided one if any
                mRenderTarget_ = *pRenderTarget;
            }

            pRS_->_setViewport(mRenderTarget_.pOgreRenderTarget->getViewport(0));

            PrepareForRender_(false);

            bFrameStarted_ = true;
        }
    }

    void SpriteManager::End()
    {
        if (bFrameStarted_)
        {
            // Draw everything on the render target
            RenderBuffers_(true);

            mRenderTarget_.pOgreRenderTarget = NULL;
            bFrameStarted_ = false;
        }
    }

    void SpriteManager::Clear( const Color& mColor )
    {
        Ogre::Root::getSingleton().getRenderSystem()->clearFrameBuffer(
            Ogre::FBT_COLOUR, GFX::FrostToOgreColor(mColor)
        );
    }

    s_bool SpriteManager::RenderMainTarget_()
    {
        bRenderTargets_ = false;

        // Set up...
        PrepareForRender_(true);
        PrepareForRender_(false);

        pMainSprite_->Render(0, 0);

        RenderBuffers_();
        return true;
    }

    s_bool SpriteManager::RenderTargets()
    {
        bRenderTargets_ = true;
        if (pRenderFunc_)
        {
            // Set up...
            PrepareForRender_();

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

    void SpriteManager::Convert_( float &x, float &y )
    {
        if (mAxisType_ == AXIS_DOWN)
        {
            if (!bRenderTargets_)
            {
                x = x*f2_ScreenWidth_.Get(); x += fXOffset_.Get() - 1.0f;
                y = -y*f2_ScreenHeight_.Get(); y += 1.0f + fYOffset_.Get();
            }
            else
            {
                x = 2.0f*x/mRenderTarget_.pOgreRenderTarget->getWidth() - 1.0f + fXOffset_.Get();
                y = -2.0f*y/mRenderTarget_.pOgreRenderTarget->getHeight() + 1.0f + fYOffset_.Get();
            }
        }
        else
        {
            if (!bRenderTargets_)
            {
                x = x*f2_ScreenWidth_.Get(); x += fXOffset_.Get() - 1.0f;
                y = y*f2_ScreenHeight_.Get(); y += fYOffset_.Get() - 1.0f;
            }
            else
            {
                x = 2.0f*x/mRenderTarget_.pOgreRenderTarget->getWidth() - 1.0f + fXOffset_.Get();
                y = 2.0f*(y - mRenderTarget_.uiHeight.Get())/mRenderTarget_.pOgreRenderTarget->getHeight() + 1.0f + fYOffset_.Get();
            }
        }
    }

    void SpriteManager::RenderQuad( const Quad &mQuad )
    {
        lQuadList_.push_back(mQuad);
        Quad& pQuad = lQuadList_.back();

        // Convert coordinates
        Convert_(pQuad.lVertexArray[0].fX.GetR(), pQuad.lVertexArray[0].fY.GetR());
        Convert_(pQuad.lVertexArray[1].fX.GetR(), pQuad.lVertexArray[1].fY.GetR());
        Convert_(pQuad.lVertexArray[2].fX.GetR(), pQuad.lVertexArray[2].fY.GetR());
        Convert_(pQuad.lVertexArray[3].fX.GetR(), pQuad.lVertexArray[3].fY.GetR());
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
}
