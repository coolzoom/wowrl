/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*           UnitManager source           */
/*                                        */
/*                                        */


#include "material/frost_shadermanager.h"
#include "material/frost_shader.h"
#include "xml/frost_xml_document.h"

using namespace std;

namespace Frost
{
    s_bool ShaderManager::ParseXMLFile_( const s_str& sFile )
    {
        XML::Document mDoc(sFile, "Shaders/Shader.def");

        s_str sPreProcessorCommands;
        if (Engine::GetSingleton()->GetBoolConstant("EnableSpecular"))
            sPreProcessorCommands += "SPECULAR,";
        if (Engine::GetSingleton()->GetBoolConstant("EnableMotionBlur"))
            sPreProcessorCommands += "MOTION_BLUR,";

        if (mDoc.Check(sPreProcessorCommands))
        {
            s_ptr<XML::Block> pMainBlock = mDoc.GetMainBlock();
            s_ptr<XML::Block> pShaderBlock;
            foreach_block (pShaderBlock, pMainBlock)
            {
                if (pShaderBlock->GetName() == "VertexShader")
                {
                    if (!this->ParseVertexShader_(pShaderBlock))
                        return false;
                }
                else if (pShaderBlock->GetName() == "PixelShader")
                {
                    if (!this->ParsePixelShader_(pShaderBlock))
                        return false;
                }
            }

            return true;
        }
        else
            return false;
    }

    s_bool ShaderManager::ParseVertexShader_( s_ptr<XML::Block> pShaderBlock )
    {
        s_ptr<VertexShader> pVS = this->CreateVertexShader(
            pShaderBlock->GetAttribute("name"),
            pShaderBlock->GetAttribute("file")
        );
        if (pVS)
        {
            if (s_bool(pShaderBlock->GetAttribute("skeletalAnimation")))
                pVS->NotifyUsesSkeletalAnimation();

            this->ParsePreProcessor_(pShaderBlock, pVS);
            this->ParseParams_(pShaderBlock, pVS);

            pVS->Load();

            return true;
        }
        else
            return false;
    }

    s_bool ShaderManager::ParsePixelShader_( s_ptr<XML::Block> pShaderBlock )
    {
        s_ptr<PixelShader> pPS = this->CreatePixelShader(
            pShaderBlock->GetAttribute("name"),
            pShaderBlock->GetAttribute("file")
        );
        if (pPS)
        {
            this->ParsePreProcessor_(pShaderBlock, pPS);
            this->ParseParams_(pShaderBlock, pPS);

            s_ptr<XML::Block> pTextureBindsBlock = pShaderBlock->GetBlock("TextureBinds");
            if (pTextureBindsBlock)
            {
                s_ptr<XML::Block> pTextureBindBlock;
                foreach_block (pTextureBindBlock, pTextureBindsBlock)
                {
                    pPS->BindTextureSampler(
                        pTextureBindBlock->GetAttribute("name"),
                        s_uint(pTextureBindBlock->GetAttribute("id"))
                    );
                }
            }

            pPS->Load();

            return true;
        }
        else
            return false;
    }

    s_bool ShaderManager::ParsePreProcessor_( s_ptr<XML::Block> pShaderBlock, s_ptr<Shader> pShader )
    {
        s_ptr<XML::Block> pPreProcBlock = pShaderBlock->GetBlock("PreProcessor");
        if (pPreProcBlock)
        {
            s_ptr<XML::Block> pDirectiveBlock;
            foreach_block (pDirectiveBlock, pPreProcBlock)
            {
                s_str sDirective = pDirectiveBlock->GetAttribute("variable")+"="+pDirectiveBlock->GetAttribute("value");
                pShader->AddPreProcessor(sDirective);
            }
        }

        return true;
    }

    s_bool ShaderManager::ParseParams_( s_ptr<XML::Block> pShaderBlock, s_ptr<Shader> pShader )
    {
        s_ptr<XML::Block> pParamsBlock = pShaderBlock->GetBlock("Params");
        if (pParamsBlock)
        {
            s_ptr<XML::Block> pParamBlock;
            foreach_block (pParamBlock, pParamsBlock)
            {
                if (pParamBlock->GetName() == "LightParams")
                {
                    pShader->AddLightParams(s_uint(pParamBlock->GetAttribute("number")));
                }
                else if (pParamBlock->GetName() == "AutoParam")
                {
                    if (pParamBlock->IsProvided("param"))
                    {
                        pShader->AddAutoParam(
                            pParamBlock->GetAttribute("name"),
                            pParamBlock->GetAttribute("param"),
                            s_uint(pParamBlock->GetAttribute("data"))
                        );
                    }
                    else
                    {
                        pShader->AddAutoParam(pParamBlock->GetAttribute("name"));
                    }
                }
                else if (pParamBlock->GetName() == "Param")
                {
                    s_str sType = pParamBlock->GetAttribute("type");
                    if (sType == "float")
                    {
                        pShader->AddParam(
                            pParamBlock->GetAttribute("name"),
                            s_float(pParamBlock->GetAttribute("value"))
                        );
                    }
                    else if (sType == "int")
                    {
                        pShader->AddParam(
                            pParamBlock->GetAttribute("name"),
                            s_int(pParamBlock->GetAttribute("value"))
                        );
                    }
                    else
                    {
                        Warning(CLASS_NAME,
                            "Unsupported param type : \""+sType+"\"."
                        );
                    }
                }
            }
        }

        return true;
    }
}
