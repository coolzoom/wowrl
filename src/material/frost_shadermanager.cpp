/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*           ShaderManager source         */
/*                                        */
/*                                        */

#include "material/frost_shadermanager.h"
#include "material/frost_shader.h"

using namespace std;

namespace Frost
{
    const s_str ShaderManager::CLASS_NAME = "ShaderManager";

    ShaderManager::ShaderManager()
    {
    }

    ShaderManager::~ShaderManager()
    {
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
            return NULL;
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
            return NULL;
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
    }

    void ShaderManager::UpdateShaders()
    {
        s_map< s_str, s_ptr<Shader> >::iterator iterShader;
        foreach (iterShader, lShaderList_)
        {
            iterShader->second->Update();
        }
    }
}
