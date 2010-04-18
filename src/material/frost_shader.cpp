/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*             Shader source              */
/*                                        */
/*                                        */

#include "material/frost_shader.h"
#include "scene/frost_lightmanager.h"

#include <OgrePass.h>

using namespace std;

namespace Frost
{
    const s_str Shader::CLASS_NAME       = "Shader";
    const s_str VertexShader::CLASS_NAME = "VertexShader";
    const s_str PixelShader::CLASS_NAME  = "PixelShader";

    Shader::Shader( const s_str& sName ) :
        sName_(sName)
    {
    }

    Shader::~Shader()
    {
    }

    void Shader::SetFile( const s_str& sFile )
    {
        sFile_ = sFile;
    }

    void Shader::AddPreProcessor( const s_str& sPPCommand )
    {
        if (sPPCommands_.IsEmpty())
        {
            sPPCommands_ = sPPCommand;
        }
        else
        {
            sPPCommands_ += ","+sPPCommand;
        }
    }

    void Shader::AddParam( const s_str& sName, const s_var& vValue )
    {
        lParamList_.PushBack(Param(sName, vValue));
    }

    void Shader::AddAutoParam( const AutoParam& mAuto )
    {
        lAutoParamList_.PushBack(mAuto);
    }

    void Shader::AddAutoParam( const s_str& sName, Ogre::GpuProgramParameters::AutoConstantType mType, const s_uint& uiInfo )
    {
        lAutoParamList_.PushBack(AutoParam(sName, mType, uiInfo));
    }

    void Shader::AddAutoParam( const s_str& sName, const s_str& sType, const s_uint& uiInfo )
    {
        Ogre::GpuProgramParameters::AutoConstantType mType;
        if (sType == "texture_matrix")
            mType = Ogre::GpuProgramParameters::ACT_TEXTURE_MATRIX;
        else if (sType == "world_matrix_array_3x4")
            mType = Ogre::GpuProgramParameters::ACT_WORLD_MATRIX_ARRAY_3x4;
        else if (sType == "far_clip_distance")
            mType = Ogre::GpuProgramParameters::ACT_FAR_CLIP_DISTANCE;
        else if (sType == "inverse_viewproj_matrix")
            mType = Ogre::GpuProgramParameters::ACT_INVERSE_VIEWPROJ_MATRIX;
        else if (sType == "custom")
            mType = Ogre::GpuProgramParameters::ACT_CUSTOM;
        else
        {
            Error(CLASS_NAME,
                "Unsupported auto param : \""+sType+"\"."
            );
            return;
        }

        lAutoParamList_.PushBack(AutoParam(sName, mType, uiInfo));
    }

    void Shader::AddAutoParam( const s_str& sName )
    {
        Ogre::GpuProgramParameters::AutoConstantType mType;
        if (sName == "mAmbient")
            mType = Ogre::GpuProgramParameters::ACT_DERIVED_SCENE_COLOUR;
        else if (sName == "mDiffuse")
            mType = Ogre::GpuProgramParameters::ACT_SURFACE_DIFFUSE_COLOUR;
        else if (sName == "mWorld")
            mType = Ogre::GpuProgramParameters::ACT_WORLD_MATRIX;
        else if (sName == "mWorldViewProj")
            mType = Ogre::GpuProgramParameters::ACT_WORLDVIEWPROJ_MATRIX;
        else if (sName == "mViewProj")
            mType = Ogre::GpuProgramParameters::ACT_VIEWPROJ_MATRIX;
        else if (sName == "mCamPos")
            mType = Ogre::GpuProgramParameters::ACT_CAMERA_POSITION;
        else if (sName == "mDelta")
            mType = Ogre::GpuProgramParameters::ACT_FRAME_TIME;
        else if (sName == "mFPS")
            mType = Ogre::GpuProgramParameters::ACT_FPS;
        else
            return;

        AddAutoParam(sName, mType);
    }

    void Shader::AddLightParams( const s_uint& uiLightNbr, const s_bool& bDirectional )
    {
        AddAutoParam(
            "mLightPos",
            Ogre::GpuProgramParameters::ACT_LIGHT_POSITION_ARRAY,
            uiLightNbr.Get()
        );
        AddAutoParam(
            "mLightDiffuseColor",
            Ogre::GpuProgramParameters::ACT_DERIVED_LIGHT_DIFFUSE_COLOUR_ARRAY,
            uiLightNbr.Get()
        );
        AddAutoParam(
            "mLightAtten",
            Ogre::GpuProgramParameters::ACT_LIGHT_ATTENUATION_ARRAY,
            uiLightNbr.Get()
        );

        if (bDirectional)
        {
            bSendSunParameters_ = true;
        }
    }

    const s_str& Shader::GetOgreName()
    {
        return sName_;
    }

    const s_bool& Shader::IsValid() const
    {
        return bIsValid_;
    }

    void Shader::WriteParams_( s_ptr<Ogre::Pass> pPass )
    {
        if (!lAutoParamList_.IsEmpty())
        {
            Ogre::GpuProgramParametersSharedPtr pParam = GetOgreParamList_(pPass);
            s_ctnr<AutoParam>::iterator iterAutoParam;
            foreach (iterAutoParam, lAutoParamList_)
            {
                pParam->setNamedAutoConstant(
                    iterAutoParam->sName.Get(), iterAutoParam->mType, iterAutoParam->uiInfo.Get()
                );
            }
        }

        s_ctnr<Param>::iterator iterParam;
        foreach (iterParam, lParamList_)
        {
            SetParameter(iterParam->sName, iterParam->vValue, pPass);
        }
    }

    void Shader::UnBind( s_ptr<Ogre::Pass> pPass )
    {
        s_ctnr< s_ptr<Ogre::Pass> >::iterator iter = lBindedPassList_.Get(pPass);
        if (iter != lBindedPassList_.End())
        {
            lBindedPassList_.Erase(iter);
        }
        else
        {
            Warning(CLASS_NAME,
                "Trying to unbind \""+sName_+"\" from a pass that it has not been binded to."
            );
        }
    }

    void Shader::SetParameter( const s_str& sName, const s_var& vValue, s_ptr<Ogre::Pass> pPass )
    {
        if (pPass)
        {
            Ogre::GpuProgramParametersSharedPtr pParam = GetOgreParamList_(pPass);
            try
            {
                if (vValue.IsOfType<Vector>())
                    pParam->setNamedConstant(
                        sName.Get(),
                        Ogre::Vector4(Vector::FrostToOgre(vValue.Get<Vector>()))
                    );
                else if (vValue.IsOfType<Ogre::Vector3>())
                    pParam->setNamedConstant(
                        sName.Get(),
                        Ogre::Vector4(vValue.Get<Ogre::Vector3>())
                    );
                else if (vValue.IsOfType<Ogre::Vector4>())
                    pParam->setNamedConstant(
                        sName.Get(),
                        vValue.Get<Ogre::Vector4>()
                    );
                else if (vValue.IsOfType<Color>())
                {
                    Color mColor = vValue.Get<Color>();
                    pParam->setNamedConstant(
                        sName.Get(),
                        Ogre::Vector4(
                            mColor.GetR().Get()/255.0f,
                            mColor.GetG().Get()/255.0f,
                            mColor.GetB().Get()/255.0f,
                            mColor.GetA().Get()/255.0f
                        )
                    );
                }
                else if (vValue.IsOfType<Ogre::ColourValue>())
                    pParam->setNamedConstant(
                        sName.Get(),
                        vValue.Get<Ogre::ColourValue>()
                    );
                else
                {
                    Error(CLASS_NAME,
                        "Unsupported param type : \""+s_str(vValue.GetType().name())+"\"."
                    );
                }
            }
            catch (Ogre::Exception e)
            {
                Error(CLASS_NAME,
                    "Parameter \""+sName+"\" doesn't exist in \""+sName_+"\"."
                );
            }
        }
        else
        {
            s_ctnr< s_ptr<Ogre::Pass> >::iterator iterPass;
            foreach (iterPass, lBindedPassList_)
            {
                SetParameter(sName, vValue, *iterPass);
            }
        }
    }

    void Shader::Update()
    {
        if (bIsValid_)
        {
            if (bSendSunParameters_)
            {
                Ogre::GpuProgramParametersSharedPtr pParam;
                s_ctnr< s_ptr<Ogre::Pass> >::iterator iterPass;
                foreach (iterPass, lBindedPassList_)
                {
                    SetParameter("mSunDir",
                        -LightManager::GetSingleton()->GetSunDirection(),
                        *iterPass
                    );

                    SetParameter("mSunColor",
                        LightManager::GetSingleton()->GetSunColor(),
                        *iterPass
                    );
                }
            }
        }
    }

    VertexShader::VertexShader( const s_str& sName ) :
        Shader(sName)
    {
    }

    VertexShader::~VertexShader()
    {
    }

    void VertexShader::NotifyUsesSkeletalAnimation()
    {
        bSkeletalAnim_ = true;
    }

    void VertexShader::Load()
    {
        if (!bIsLoaded_)
        {
            try
            {
                Ogre::HighLevelGpuProgram* pProgram;

                // HLSL
                pProgram = Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
                    (sName_+"_HLSL").Get(), "Frost", "hlsl", Ogre::GPT_VERTEX_PROGRAM
                ).get();
                pProgram->setSourceFile((sFile_+".hlsl").Get());
                pProgram->setParameter("entry_point", "main_vs");
                pProgram->setParameter("target", "vs_2_0");
                if (bSkeletalAnim_)
                {
                    pProgram->setSkeletalAnimationIncluded(true);
                    pProgram->setParameter("column_major_matrices", "false");
                }
                if (!sPPCommands_.IsEmpty())
                {
                    pProgram->setParameter("preprocessor_defines", sPPCommands_.Get());
                }

                // GLSL
                pProgram = Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
                    (sName_+"_GLSL").Get(), "Frost", "glsl", Ogre::GPT_VERTEX_PROGRAM
                ).get();
                pProgram->setSourceFile((sFile_+".glsl").Get());
                if (bSkeletalAnim_)
                {
                    pProgram->setSkeletalAnimationIncluded(true);
                }
                if (!sPPCommands_.IsEmpty())
                {
                    pProgram->setParameter("preprocessor_defines", sPPCommands_.Get());
                }

                // Unified vertex shader
                pOgreShader_ = static_cast<Ogre::UnifiedHighLevelGpuProgram*>(
                    Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
                    sName_.Get(), "Frost", "unified", Ogre::GPT_VERTEX_PROGRAM
                ).get());
                pOgreShader_->addDelegateProgram((sName_+"_HLSL").Get());
                pOgreShader_->addDelegateProgram((sName_+"_GLSL").Get());
                pOgreShader_->load();

                if (!pOgreShader_->hasCompileError())
                    bIsValid_ = true;
                else
                {
                    Warning(CLASS_NAME,
                        "\""+sName_+"\" is not supported by your hardware."
                    );
                }
            }
            catch (Ogre::Exception e)
            {
                Error(CLASS_NAME, e.getDescription());
            }
            catch (...)
            {
                Error(CLASS_NAME, "Unhandled exception.");
            }

            bIsLoaded_ = true;
        }
    }

    void VertexShader::BindTo( s_ptr<Ogre::Pass> pPass )
    {
        if (bIsValid_)
        {
            lBindedPassList_.PushBack(pPass);
            pPass->setVertexProgram(sName_.Get());
            WriteParams_(pPass);
        }
    }

    Ogre::GpuProgramParametersSharedPtr VertexShader::GetOgreParamList_( s_ptr<Ogre::Pass> pPass )
    {
        return pPass->getVertexProgramParameters();
    }

    PixelShader::PixelShader( const s_str& sName ) :
        Shader(sName)
    {
    }

    PixelShader::~PixelShader()
    {
    }

    void PixelShader::BindTextureSampler( const s_str& sName, const s_uint& uiID )
    {
        lSamplerParamList_.PushBack(SamplerParam(sName, uiID));
    }

    void PixelShader::Load()
    {
        if (!bIsLoaded_)
        {
            try
            {
                Ogre::HighLevelGpuProgram* pProgram;

                // HLSL
                pProgram = Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
                    (sName_+"_HLSL").Get(), "Frost", "hlsl", Ogre::GPT_FRAGMENT_PROGRAM
                ).get();
                pProgram->setSourceFile((sFile_+".hlsl").Get());
                pProgram->setParameter("entry_point", "main_ps");
                pProgram->setParameter("target", "ps_2_0");
                if (!sPPCommands_.IsEmpty())
                {
                    pProgram->setParameter("preprocessor_defines", sPPCommands_.Get());
                }

                // GLSL
                pProgram = Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
                    (sName_+"_GLSL").Get(), "Frost", "glsl", Ogre::GPT_FRAGMENT_PROGRAM
                ).get();
                pProgram->setSourceFile((sFile_+".glsl").Get());
                if (!sPPCommands_.IsEmpty())
                {
                    pProgram->setParameter("preprocessor_defines", sPPCommands_.Get());
                }

                // Unified pixel shader
                pOgreShader_ = static_cast<Ogre::UnifiedHighLevelGpuProgram*>(
                    Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
                    sName_.Get(), "Frost", "unified", Ogre::GPT_FRAGMENT_PROGRAM
                ).get());
                pOgreShader_->addDelegateProgram((sName_+"_HLSL").Get());
                pOgreShader_->addDelegateProgram((sName_+"_GLSL").Get());
                pOgreShader_->load();

                if (!pOgreShader_->hasCompileError())
                    bIsValid_ = true;
                else
                {
                    Warning(CLASS_NAME,
                        "\""+sName_+"\" is not supported by your hardware."
                    );
                }
            }
            catch (Ogre::Exception e)
            {
                Error(CLASS_NAME, e.getDescription());
            }
            bIsLoaded_ = true;
        }
    }

    void PixelShader::BindTo( s_ptr<Ogre::Pass> pPass )
    {
        if (bIsValid_)
        {
            lBindedPassList_.PushBack(pPass);
            pPass->setFragmentProgram(sName_.Get());
            WriteParams_(pPass);

            if (Engine::GetSingleton()->GetRenderer() == "OpenGL")
            {
                s_ctnr<SamplerParam>::iterator iterParam;
                foreach (iterParam, lSamplerParamList_)
                {
                    pPass->getFragmentProgramParameters()->setNamedConstant(
                        iterParam->sName.Get(), (int)iterParam->uiID.Get()
                    );
                }
            }
        }
    }

    Ogre::GpuProgramParametersSharedPtr PixelShader::GetOgreParamList_( s_ptr<Ogre::Pass> pPass )
    {
        return pPass->getFragmentProgramParameters();
    }
}
