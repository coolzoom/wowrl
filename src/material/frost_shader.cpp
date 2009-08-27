/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*             Shader source              */
/*                                        */
/*                                        */

#include "material/frost_shader.h"

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
        else
        {
            Error(CLASS_NAME,
                "Unsupported auto param : \""+sType+"\"."
            );
            mType = Ogre::GpuProgramParameters::ACT_TIME;
        }

        lAutoParamList_.PushBack(AutoParam(sName, mType, uiInfo));
    }

    void Shader::AddAutoParam( const s_str& sName )
    {
        Ogre::GpuProgramParameters::AutoConstantType mType;
        if (sName == "mAmbient")
            mType = Ogre::GpuProgramParameters::ACT_DERIVED_SCENE_COLOUR;
        else if (sName == "mWorld")
            mType = Ogre::GpuProgramParameters::ACT_WORLD_MATRIX;
        else if (sName == "mWorldViewProj")
            mType = Ogre::GpuProgramParameters::ACT_WORLDVIEWPROJ_MATRIX;
        else if (sName == "mViewProj")
            mType = Ogre::GpuProgramParameters::ACT_VIEWPROJ_MATRIX;
        else if (sName == "mCamPos")
            mType = Ogre::GpuProgramParameters::ACT_CAMERA_POSITION;
        else
            return;

        AddAutoParam(sName, mType);
    }

    void Shader::AddLightParams(const s_uint& uiLightNbr)
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
    }

    const s_str& Shader::GetOgreName()
    {
        return sName_;
    }

    void Shader::WriteParams_(s_ptr<Ogre::GpuProgramParameters> pOgreShaderParams)
    {
        s_ctnr<AutoParam>::iterator iterAutoParam;
        foreach (iterAutoParam, lAutoParamList_)
        {
            pOgreShaderParams->setNamedAutoConstant(
                iterAutoParam->sName.Get(), iterAutoParam->mType, iterAutoParam->uiInfo.Get()
            );
        }

        s_ctnr<Param>::iterator iterParam;
        foreach (iterParam, lParamList_)
        {
            if (iterParam->vValue.IsOfType<s_float>())
            {
                pOgreShaderParams->setNamedConstant(
                    iterParam->sName.Get(),
                    iterParam->vValue.Get<s_float>().Get()
                );
            }
            if (iterParam->vValue.IsOfType<s_int>())
            {
                pOgreShaderParams->setNamedConstant(
                    iterParam->sName.Get(),
                    iterParam->vValue.Get<s_int>().Get()
                );
            }
            if (iterParam->vValue.IsOfType<Vector>())
            {
                pOgreShaderParams->setNamedConstant(
                    iterParam->sName.Get(),
                    Vector::FrostToOgre(iterParam->vValue.Get<Vector>())
                );
            }
            if (iterParam->vValue.IsOfType< s_array<float> >())
            {
                s_array<float> lArray = iterParam->vValue.Get< s_array<float> >();
                pOgreShaderParams->setNamedConstant(
                    iterParam->sName.Get(),
                    lArray.GetClassicArray(),
                    lArray.GetSize().Get()
                );
            }
            if (iterParam->vValue.IsOfType< s_array<s_int> >())
            {
                s_array<int> lArray = iterParam->vValue.Get< s_array<int> >();
                pOgreShaderParams->setNamedConstant(
                    iterParam->sName.Get(),
                    lArray.GetClassicArray(),
                    lArray.GetSize().Get()
                );
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
    }

    void VertexShader::BindTo( s_ptr<Ogre::Pass> pPass )
    {
        pPass->setVertexProgram(sName_.Get());
        WriteParams_(pPass->getVertexProgramParameters().get());
    }

    PixelShader::PixelShader( const s_str& sName ) :
        Shader(sName)
    {
    }

    PixelShader::~PixelShader()
    {
    }

    void PixelShader::BindTextureSampler(const s_str& sName, const s_uint& uiID)
    {
        lSamplerParamList_.PushBack(SamplerParam(sName, uiID));
    }

    void PixelShader::Load()
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
    }

    void PixelShader::BindTo( s_ptr<Ogre::Pass> pPass )
    {
        pPass->setFragmentProgram(sName_.Get());
        WriteParams_(pPass->getFragmentProgramParameters().get());

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
