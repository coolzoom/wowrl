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

#include "impl/ogre/ogre_spritemanager.h"
#include "frost_sprite.h"
#include "impl/ogre/ogre_sprite.h"
#include "impl/ogre/ogre_material.h"
#include "frost_rendertarget.h"
#include "impl/ogre/ogre_rendertarget.h"

#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreRenderSystem.h>
#include <OgreSceneManager.h>
#include <OgreMatrix4.h>
#include <OgreHardwareBufferManager.h>
#include <OgreHardwareVertexBuffer.h>

#define OGRE2D_MINIMAL_HARDWARE_BUFFER_SIZE 120

namespace Frost
{
    void RenderCustom();
}

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str SpriteManager::CLASS_NAME = "SpriteManager";

/** \cond NOT_REMOVE_FROM_DOC
*/
struct VertexChunk
{
    Ogre::Material* pMat;
    uint            uiVertexCount;
};
/** \endcond
*/

SpriteManager::SpriteManager( const s_uint& uiScreenWidth, const s_uint& uiScreenHeight, const s_str& sSceneMgrName )
{
    mAxisType_ = AXIS_DOWN;

    pSceneMgr_ = Ogre::Root::getSingleton().getSceneManager(sSceneMgrName.Get());

    mHardwareBuffer_.setNull();

    pRS_ = Ogre::Root::getSingletonPtr()->getRenderSystem();

    s_float fWidth  = s_float(uiScreenWidth);
    s_float fHeight = s_float(uiScreenHeight);

    f2_ScreenWidth_ = 2.0f/fWidth;
    f2_ScreenHeight_ = 2.0f/fHeight;

    fXOffset_ = pRS_->getHorizontalTexelOffset();
    fYOffset_ = pRS_->getVerticalTexelOffset();

    pSceneMgr_->addRenderQueueListener(this);

    pMainTarget_ = s_refptr<GUI::RenderTarget>(new GUI::RenderTarget(uiScreenWidth, uiScreenHeight));
    pMainSprite_ = s_refptr<GUI::Sprite>(new GUI::Sprite(nullptr, pMainTarget_.Get()));

    bRenderTargets_ = true;
}

SpriteManager::~SpriteManager()
{
    if (!mHardwareBuffer_.isNull())
        DestroyHardwareBuffers_();
}

void SpriteManager::renderQueueStarted( Ogre::uint8 uiQueueGroupId, const Ogre::String& sInvocation, bool& bSkip )
{
}

void SpriteManager::renderQueueEnded( Ogre::uint8 uiQueueGroupId, const Ogre::String& sInvocation, bool& bRepeat )
{
    if (uiQueueGroupId == Ogre::RENDER_QUEUE_OVERLAY)
        RenderMainTarget_();
}

void SpriteManager::SetWindowSize( const s_uint& uiScreenWidth, const s_uint& uiScreenHeight )
{
    s_float fWidth  = s_float(uiScreenWidth);
    s_float fHeight = s_float(uiScreenHeight);

    f2_ScreenWidth_ = 2.0f/fWidth;
    f2_ScreenHeight_ = 2.0f/fHeight;

    fXOffset_ = pRS_->getHorizontalTexelOffset()/fWidth;
    fYOffset_ = pRS_->getVerticalTexelOffset()/fHeight;

    s_uint uiWidth = s_uint(fWidth);
    s_uint uiHeight = s_uint(fHeight);

    if (!pMainTarget_)
        pMainTarget_ = s_refptr<GUI::RenderTarget>(new GUI::RenderTarget(uiScreenWidth, uiScreenHeight));
    else
        pMainTarget_->SetDimensions(uiScreenWidth, uiScreenHeight);

    pMainSprite_ = s_refptr<GUI::Sprite>(new GUI::Sprite(nullptr,
        pMainSprite_->GetImpl()->GetMaterial(), fWidth, fHeight
    ));
}

/** \cond NOT_REMOVE_FROM_DOC
*/
inline void WriteVertex( float* &pBuffer, GUI::Vertex &mV )
{
    *pBuffer = mV.mPosition.X().Get();  // x
    ++pBuffer;
    *pBuffer = mV.mPosition.Y().Get();  // y
    ++pBuffer;
    *pBuffer = -1;                      // z
    ++pBuffer;

    *pBuffer = mV.mUVs.X().Get(); // u
    ++pBuffer;
    *pBuffer = mV.mUVs.Y().Get(); // v
    ++pBuffer;

    *pBuffer = mV.mColor.GetR().Get();  // r
    ++pBuffer;

    *pBuffer = mV.mColor.GetG().Get();  // g
    ++pBuffer;

    *pBuffer = mV.mColor.GetB().Get();  // b
    ++pBuffer;

    *pBuffer = mV.mColor.GetA().Get();  // a
    ++pBuffer;
}
/** \endcond
*/

void SpriteManager::RenderBuffers_( s_bool bCallBeginEnd )
{
    if (lQuadList_.IsEmpty())
        return;

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

    s_ctnr<GUI::Quad>::iterator itEndQuad = lQuadList_.End();
    s_ctnr<GUI::Quad>::iterator itCurrQuad = lQuadList_.Begin();
    mThisChunk.pMat = itCurrQuad->mMat.GetImpl()->GetOgreMaterial().Get();
    mThisChunk.uiVertexCount = 0u;
    while (itCurrQuad != itEndQuad)
    {
        WriteVertex(pBuffer, itCurrQuad->lVertexArray[0]);
        WriteVertex(pBuffer, itCurrQuad->lVertexArray[1]);
        WriteVertex(pBuffer, itCurrQuad->lVertexArray[2]);

        WriteVertex(pBuffer, itCurrQuad->lVertexArray[2]);
        WriteVertex(pBuffer, itCurrQuad->lVertexArray[3]);
        WriteVertex(pBuffer, itCurrQuad->lVertexArray[0]);

        // Remember this chunk
        mThisChunk.uiVertexCount += 6u;
        ++itCurrQuad;
        if ( (itCurrQuad == itEndQuad) || (mThisChunk.pMat != itCurrQuad->mMat.GetImpl()->GetOgreMaterial().Get()) )
        {
            lChunkList.PushBack(mThisChunk);
            if (itCurrQuad != itEndQuad)
            {
                mThisChunk.pMat = itCurrQuad->mMat.GetImpl()->GetOgreMaterial().Get();
                mThisChunk.uiVertexCount = 0u;
            }
        }
    }

    mHardwareBuffer_->unlock();

    // Do the real render!
    s_ctnr<VertexChunk>::iterator itCurrChunk;

    if (bCallBeginEnd)
        Ogre::Root::getSingleton().getRenderSystem()->_beginFrame();

    mRenderOp_.vertexData->vertexStart = 0;
    foreach (itCurrChunk, lChunkList)
    {
        mRenderOp_.vertexData->vertexCount = itCurrChunk->uiVertexCount;
        s_ptr<Ogre::Technique> pTech = itCurrChunk->pMat->getTechnique(0);

        for (uint i = 0; i < pTech->getNumPasses(); ++i)
        {
            Ogre::Pass* pPass = pTech->getPass(i);
            pSceneMgr_->_setPass(pPass);
            if (pPass->isProgrammable())
            {
                Ogre::AutoParamDataSource mDataSource;
                mDataSource.setCurrentViewport(pRenderTarget_->GetImpl()->GetOgreRenderTarget()->getViewport(0));
                mDataSource.setCurrentRenderTarget(pRenderTarget_->GetImpl()->GetOgreRenderTarget().Get());
                mDataSource.setCurrentSceneManager(pSceneMgr_.Get());
                mDataSource.setWorldMatrices(&Ogre::Matrix4::IDENTITY, 1);
                mDataSource.setCurrentPass(pPass);
                pPass->_updateAutoParamsNoLights(&mDataSource);

                if (pPass->hasVertexProgram())
                {
                    Ogre::GpuProgramParametersSharedPtr pParams = pPass->getVertexProgramParameters();
                    pParams->setNamedConstant("mProj", mProj_);
                    pRS_->bindGpuProgramParameters(Ogre::GPT_VERTEX_PROGRAM,
                        pPass->getVertexProgramParameters()
                    );
                }

                if (pPass->hasFragmentProgram())
                {
                    pRS_->bindGpuProgramParameters(Ogre::GPT_FRAGMENT_PROGRAM,
                        pPass->getFragmentProgramParameters()
                    );
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

void SpriteManager::PrepareForRender_()
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

    mProj_ = Ogre::Matrix4::IDENTITY;
    if (mAxisType_ == AXIS_DOWN)
    {
        if (bRenderTargets_)
        {
            mProj_.setScale(Ogre::Vector3(
                2.0f/pRenderTarget_->GetRealWidth().Get(),
                -2.0f/pRenderTarget_->GetRealHeight().Get(),
                1.0f
            ));
        }
        else
        {
            mProj_.setScale(Ogre::Vector3(
                f2_ScreenWidth_.Get(),
                -f2_ScreenHeight_.Get(),
                1.0f
            ));
        }
        mProj_ = Ogre::Matrix4::getTrans(-1.0f, 1.0f, 0.0f) * mProj_;
    }
    else
    {
        if (bRenderTargets_)
        {
            mProj_.setScale(Ogre::Vector3(
                2.0f/pRenderTarget_->GetRealWidth().Get(),
                2.0f/pRenderTarget_->GetRealHeight().Get(),
                1.0f
            ));
            mProj_ = Ogre::Matrix4::getTrans(
                -1.0f,
                (-2.0f*pRenderTarget_->GetHeight().Get())/pRenderTarget_->GetRealHeight().Get() + 1.0f,
                0.0f
            )*mProj_;
        }
        else
        {
            mProj_.setScale(Ogre::Vector3(
                f2_ScreenWidth_.Get(),
                f2_ScreenHeight_.Get(),
                1.0f
            ));
            mProj_ = Ogre::Matrix4::getTrans(-1.0f, -1.0f, 0.0f) * mProj_;
        }
    }

    pRS_->_setProjectionMatrix(mProj_);

    Ogre::Matrix4 mWorld_ = Ogre::Matrix4::getTrans(fXOffset_.Get(), fYOffset_.Get(), 0.0f);

    pRS_->_setWorldMatrix(mWorld_);
    pRS_->_setCullingMode(Ogre::CULL_NONE);
}

void SpriteManager::Begin( s_ptr<GUI::RenderTarget> pRenderTarget )
{
    if (!bFrameStarted_)
    {
        // Set a new render target
        if (!pRenderTarget)
        {
            // The main target...
            pRenderTarget_ = pMainTarget_.Get();
        }
        else
        {
            // ... or the provided one if any
            pRenderTarget_ = pRenderTarget;
        }

        pRS_->_setViewport(pRenderTarget_->GetImpl()->GetOgreRenderTarget()->getViewport(0));

        PrepareForRender_();

        bFrameStarted_ = true;
    }
    else
    {
        Warning(CLASS_NAME,
            "Begin() call ignored : waiting for End() to be called."
        );
    }
}

void SpriteManager::End()
{
    if (bFrameStarted_)
    {
        // Draw everything on the render target
        RenderBuffers_(true);

        pRenderTarget_ = nullptr;
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

    PrepareForRender_();

    pMainSprite_->Render(0, 0);

    RenderBuffers_(false);

    bRenderTargets_ = true;

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
    uiOffset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2);

    // Color
    pDecl->addElement(0, uiOffset, Ogre::VET_FLOAT4, Ogre::VES_DIFFUSE);

    mHardwareBuffer_ = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
        pDecl->getVertexSize(0),
        uiSize,
        Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE,
        false
    );

    mRenderOp_.vertexData->vertexBufferBinding->setBinding(0, mHardwareBuffer_);

    mRenderOp_.operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;
    mRenderOp_.useIndexes = false;
}

void SpriteManager::DestroyHardwareBuffers_()
{
    delete mRenderOp_.vertexData;
    mRenderOp_.vertexData = nullptr;
    mHardwareBuffer_.setNull();
}

void SpriteManager::RenderQuad( const GUI::Quad& mQuad )
{
    lQuadList_.PushBack(mQuad);
}

void SpriteManager::SetYAxisType( const AxisType& mAxis )
{
    mAxisType_ = mAxis;
}

const AxisType& SpriteManager::GetYAxisType() const
{
    return mAxisType_;
}
