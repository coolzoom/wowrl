/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*           ShaderManager source         */
/*                                        */
/*                                        */

#include "material/frost_shadermanager.h"
#include "material/frost_shader.h"
#include "material/frost_materialmanager.h"
#include "gui/frost_spritemanager.h"
#include "gui/frost_sprite.h"
#include "camera/frost_cameramanager.h"
#include "camera/frost_camera.h"

#include <OgreRenderTargetListener.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreRenderSystem.h>

#include <utils/frost_utils_directory.h>

using namespace std;

namespace Frost
{
    class SceneRenderTargetListener : public Ogre::RenderTargetListener
    {
    public :

        void preRenderTargetUpdate( const Ogre::RenderTargetEvent& mEvent )
        {
            // Clear the render target with the selected background color
            ShaderManager::GetSingleton()->GetSceneRenderTarget()->Clear(
                CameraManager::GetSingleton()->GetBackgroundColor()
            );

            if (ShaderManager::GetSingleton()->IsMotionBlurEnabled())
            {
                // Clear the render target with the initial color
                ShaderManager::GetSingleton()->GetMotionBlurMaskRenderTarget()->Clear(
                    Color(0, 255, 255, 255)
                );
            }
        }
    };

    class RenderWindowListener : public Ogre::RenderTargetListener
    {
    public :

        void preRenderTargetUpdate( const Ogre::RenderTargetEvent& mEvent )
        {
            // Only render the SpriteManager's render target on the screen
            Engine::GetSingleton()->GetOgreSceneManager()->setSpecialCaseRenderQueueMode(
                Ogre::SceneManager::SCRQM_INCLUDE
            );
        }

        void postRenderTargetUpdate( const Ogre::RenderTargetEvent& mEvent )
        {
            // ... and disable it for the scene render target
            Engine::GetSingleton()->GetOgreSceneManager()->setSpecialCaseRenderQueueMode(
                Ogre::SceneManager::SCRQM_EXCLUDE
            );
        }
    };

    const s_str ShaderManager::CLASS_NAME = "ShaderManager";

    ShaderManager::ShaderManager()
    {
        Log("Creating "+CLASS_NAME+".");
    }

    ShaderManager::~ShaderManager()
    {
        Log("Closing "+CLASS_NAME+"...");
        pSceneSprite_.Delete();
    }

    s_ptr<VertexShader> ShaderManager::CreateVertexShader( const s_str& sName, const s_str& sFile )
    {
        s_ptr<VertexShader> pVS = new VertexShader(sName+"_VS");
        pVS->SetFile(sFile);

        lShaderList_[sName+"_VS"] = pVS;

        return pVS;
    }

    s_ptr<PixelShader> ShaderManager::CreatePixelShader( const s_str& sName, const s_str& sFile )
    {
        s_ptr<PixelShader> pPS = new PixelShader(sName+"_PS");
        pPS->SetFile(sFile);

        lShaderList_[sName+"_PS"] = pPS;

        return pPS;
    }

    s_ptr<VertexShader> ShaderManager::GetVertexShader( const s_str& sName ) const
    {
        s_map< s_str, s_ptr<Shader> >::const_iterator iter = lShaderList_.Get(sName+"_VS");
        if (iter != lShaderList_.End())
        {
            s_ptr<VertexShader> pVS = s_ptr<VertexShader>::DynamicCast(iter->second);
            if (pVS)
            {
                return pVS;
            }
            else
            {
                throw Exception(CLASS_NAME, "GetVertexShader() : dynamic cast failed !");
            }
        }
        else
        {
            Warning(CLASS_NAME, "No shader found with the name \""+sName+"_VS\".");
            return nullptr;
        }
    }

    s_ptr<PixelShader> ShaderManager::GetPixelShader( const s_str& sName ) const
    {
        s_map< s_str, s_ptr<Shader> >::const_iterator iter = lShaderList_.Get(sName+"_PS");
        if (iter != lShaderList_.End())
        {
            s_ptr<PixelShader> pPS = s_ptr<PixelShader>::DynamicCast(iter->second);
            if (pPS)
            {
                return pPS;
            }
            else
            {
                throw Exception(CLASS_NAME, "GetPixelShader() : dynamic cast failed !");
            }
        }
        else
        {
            Warning(CLASS_NAME, "No shader found with the name \""+sName+"_PS\".");
            return nullptr;
        }
    }

    void ShaderManager::LoadShaders()
    {
        Directory mDir("Shaders");

        s_ctnr<s_str> lFileList = mDir.GetRecursiveFileList(true, "xml");
        s_ctnr<s_str>::iterator iterFile;
        foreach (iterFile, lFileList)
        {
            ParseXMLFile_(*iterFile);
        }

        if (IsPostProcessingEnabled())
        {
            if (IsMotionBlurEnabled())
            {
                if (Engine::GetSingleton()->GetRenderSystem()->getCapabilities()->getNumMultiRenderTargets() < 2)
                {
                    Engine::GetSingleton()->SetConstant("EnablePostProcessing", s_bool(false));
                    Engine::GetSingleton()->SetConstant("EnableMotionBlur",     s_bool(false));
                    Warning(CLASS_NAME, "Your graphics card doesn't support Multiple Render Targets (MRTs).\n"
                        "Frost needs at least 2 for motion blur to be enabled."
                    );
                    return;
                }
            }

            Engine::GetSingleton()->GetRenderWindow()->addListener(
                new RenderWindowListener()
            );

            pSceneRenderTarget_ = SpriteManager::GetSingleton()->CreateRenderTarget(
                "SceneTarget",
                Engine::GetSingleton()->GetScreenWidth(),
                Engine::GetSingleton()->GetScreenHeight(),
                RenderTarget::PIXEL_ARGB, RenderTarget::USAGE_3D
            );

            pSceneMRT_ = Engine::GetSingleton()->GetRenderSystem()->createMultiRenderTarget(
                "Scene_MRT"
            );
            pSceneMRT_->addListener(new SceneRenderTargetListener());
            pSceneMRT_->bindSurface(0,
                (Ogre::RenderTexture*)pSceneRenderTarget_->GetOgreRenderTarget().Get()
            );

            Engine::GetSingleton()->GetOgreSceneManager()->addSpecialCaseRenderQueue(Ogre::RENDER_QUEUE_OVERLAY);

            s_refptr<Material> pMat = MaterialManager::GetSingleton()->CreateMaterial2DFromRT(
                pSceneRenderTarget_
            );
            s_ptr<Ogre::Pass> pPass = pMat->GetDefaultPass();
            pPass->setSeparateSceneBlending(
                Ogre::SBF_ONE, Ogre::SBF_ZERO,
                Ogre::SBF_ZERO, Ogre::SBF_ZERO
            );
            pPass->getTextureUnitState(0)->setTextureAddressingMode(
                Ogre::TextureUnitState::TAM_CLAMP
            );

            if (IsMotionBlurEnabled())
            {
                pMotionBlurMask_ = SpriteManager::GetSingleton()->CreateRenderTarget(
                    "MotionBlurMask",
                    Engine::GetSingleton()->GetScreenWidth(),
                    Engine::GetSingleton()->GetScreenHeight(),
                    RenderTarget::PIXEL_ARGB, RenderTarget::USAGE_3D
                );

                pSceneMRT_->bindSurface(1,
                    (Ogre::RenderTexture*)pMotionBlurMask_->GetOgreRenderTarget().Get()
                );

                s_ptr<Ogre::TextureUnitState> pTUS = pPass->createTextureUnitState();
                pTUS->setTextureName(pMotionBlurMask_->GetName().Get());

                pMat->SetPixelShader("MotionBlur");
            }

            pSceneSprite_ = new Sprite(pMat);
        }
    }

    void ShaderManager::UpdateShaders( const s_float& fDelta )
    {
        s_map< s_str, s_ptr<Shader> >::iterator iterShader;
        foreach (iterShader, lShaderList_)
        {
            iterShader->second->Update();
        }

        if (IsMotionBlurEnabled())
        {
            Ogre::GpuProgramParametersSharedPtr pParam = pSceneSprite_->GetMaterial()->GetDefaultPass()->getFragmentProgramParameters();
            s_ptr<Ogre::Camera> pCam = CameraManager::GetSingleton()->GetMainCamera()->GetOgreCamera();
            Ogre::Matrix4 mViewProj = pCam->getProjectionMatrixWithRSDepth() * pCam->getViewMatrix(true);
            Ogre::Matrix4 mViewProjInverse = mViewProj.inverse();
            pParam->setNamedConstant("mViewProjInverse", mViewProjInverse);

            static Ogre::Matrix4 mPrevViewProj = Ogre::Matrix4::IDENTITY;
            pParam->setNamedConstant("mPrevViewProj", mPrevViewProj);
            mPrevViewProj = mViewProj;

            pParam->setNamedConstant("mFPS", 1.0f/fDelta.Get());

            static s_bool bFirstIteration = true;
            if (bFirstIteration)
            {
                pParam->setNamedConstant("mMaxUV", Ogre::Vector4(
                    (s_float(pSceneRenderTarget_->GetWidth()-1.0f)/s_float(pSceneRenderTarget_->GetRealWidth())).Get(),
                    (s_float(pSceneRenderTarget_->GetHeight()-1.0f)/s_float(pSceneRenderTarget_->GetRealHeight())).Get(),
                    0.0f, 0.0f
                ));
                bFirstIteration = false;
            }
        }
    }

    void ShaderManager::RenderPostProcessedScene()
    {
        if (pSceneSprite_)
            pSceneSprite_->Render(0, 0);
    }

    s_bool ShaderManager::IsPostProcessingEnabled()
    {
        return Engine::GetSingleton()->GetBoolConstant("EnablePostProcessing");
    }

    s_bool ShaderManager::IsMotionBlurEnabled()
    {
        return Engine::GetSingleton()->GetBoolConstant("EnablePostProcessing") &&
               Engine::GetSingleton()->GetBoolConstant("EnableMotionBlur");
    }

    void ShaderManager::EnablePostProcessing( const s_bool& bEnable )
    {
        Engine::GetSingleton()->SetConstant("EnablePostProcessing", bEnable);
    }

    void ShaderManager::EnableMotionBlur( const s_bool& bEnable )
    {
        Engine::GetSingleton()->SetConstant("EnableMotionBlur", bEnable);
    }

    s_ptr<Ogre::MultiRenderTarget> ShaderManager::GetSceneMultiRenderTarget()
    {
        return pSceneMRT_;
    }

    s_ptr<RenderTarget> ShaderManager::GetSceneRenderTarget()
    {
        return pSceneRenderTarget_;
    }

    s_ptr<RenderTarget> ShaderManager::GetMotionBlurMaskRenderTarget()
    {
        return pMotionBlurMask_;
    }
}
