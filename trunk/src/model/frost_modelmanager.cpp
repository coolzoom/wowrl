/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*           ModelManager source          */
/*                                        */
/*  ## : This class is used to manager    */
/*  model creation and loading.           */
/*                                        */

#include "model/frost_modelmanager.h"
#include "model/frost_animmanager.h"
#include "model/frost_model.h"
#include "model/frost_modeldata.h"

#include "model/frost_fmmodeldata.h"
#include "model/frost_m2modeldata.h"
#include "model/frost_ogremeshmodeldata.h"

using namespace std;

namespace Frost
{
    const s_str ModelManager::CLASS_NAME = "ModelManager";

    ModelManager::ModelManager()
    {
    }

    ModelManager::~ModelManager()
    {
        Log("Closing "+CLASS_NAME+"...");
        s_map<s_str, ModelInfo>::iterator iterModel;
        foreach (iterModel, lLoadedModelList_)
        {
            iterModel->second.pData.Delete();
        }
    }

    s_refptr<Model> ModelManager::CreateModel( const s_str& sModelName, const s_str& sEntityName )
    {
        return CreateModel("", sModelName, sEntityName);
    }

    s_refptr<Model> ModelManager::CreateModel( const s_str& sCategory, const s_str& sModelName, const s_str& sEntityName )
    {
        s_refptr<Model> pModel;
        s_map<s_str, s_str>& lCategory = lModelNameToFileMap_[sCategory];

        if (lCategory.Find(sModelName))
        {
            const s_str& sFile = lCategory[sModelName];
            ModelInfo& mInfo = lLoadedModelList_[sFile];
            if (!mInfo.pData)
            {
                // This model has not been loaded yet, let's fix it
                mInfo.pData = LoadModelData_(sFile);
            }

            if (mInfo.pData)
                pModel = s_refptr<Model>(new Model(*mInfo.pData, sEntityName));
        }
        else
        {
            Warning(CLASS_NAME,
                "No model with the name \""+sModelName+"\" in category \""+sCategory+"\".\n"
                "Have you linked it with a file ?"
            );
        }

        return pModel;
    }

    s_bool ModelManager::LinkModelNameToFile( const s_str& sModelName, const s_str& sFile )
    {
        return LinkModelNameToFile("", sModelName, sFile);
    }

    s_bool ModelManager::LinkModelNameToFile( const s_str& sCategory, const s_str& sModelName, const s_str& sFile )
    {
        s_map<s_str, s_str>& lCategory = lModelNameToFileMap_[sCategory];
        if (lCategory.Find(sModelName))
        {
            Warning(CLASS_NAME,
                "\""+sModelName+"\" in category \""+sCategory+"\" is already linked to a file. Ignored.\n"
                "(file : \""+sFile+"\")"
            );
            return false;
        }

        lCategory[sModelName] = sFile;

        if (!lLoadedModelList_.Find(sFile))
        {
            ModelInfo& mInfo = lLoadedModelList_[sFile] = ModelInfo();
            mInfo.sFile = sFile;
            mInfo.uiRefCount = 1;
        }
        else
        {
            ++lLoadedModelList_[sFile].uiRefCount;
        }

        return true;
    }

    void ModelManager::ClearLink( const s_str& sModelName )
    {
        ClearLink("", sModelName);
    }

    void ModelManager::ClearLink( const s_str& sCategory, const s_str& sModelName )
    {
        s_map<s_str, s_str>& lCategory = lModelNameToFileMap_[sCategory];
        s_map<s_str, s_str>::iterator iter = lCategory.Get(sModelName);
        if (iter != lCategory.End())
        {
            ModelInfo& mInfo = lLoadedModelList_[iter->second];
            --mInfo.uiRefCount;
            if (mInfo.uiRefCount == 0)
            {
                mInfo.pData.Delete();
                lLoadedModelList_.Erase(iter->second);
            }

            lCategory.Erase(iter);
        }
        else
        {
            Warning(CLASS_NAME,
                "Trying to remove link for \""+sModelName+"\" in category \""+sCategory+"\",\n"
                "which it is not linked to any file."
            );
        }
    }

    s_bool ModelManager::PreloadModel( const s_str& sModelName )
    {
        return PreloadModel("", sModelName);
    }

    s_bool ModelManager::PreloadModel( const s_str& sCategory, const s_str& sModelName )
    {
        s_map<s_str, s_str>& lCategory = lModelNameToFileMap_[sCategory];
        s_map<s_str, s_str>::iterator iter = lCategory.Get(sModelName);
        if (iter != lCategory.End())
        {
            ModelInfo& mInfo = lLoadedModelList_[iter->second];
            if (!mInfo.pData)
            {
                mInfo.pData = LoadModelData_(iter->second);
                if (!mInfo.pData)
                    return false;
            }

            return true;
        }
        else
        {
            Warning(CLASS_NAME,
                "Trying to preload \""+sModelName+"\" in category \""+sCategory+"\",\n"
                "but this model doesn't exist."
            );
            return false;
        }
    }

    s_bool ModelManager::PreloadCategory( const s_str& sCategory, const s_bool& bStopOnError )
    {
        s_map<s_str, s_str>& lCategory = lModelNameToFileMap_[sCategory];
        s_bool bResult = true;

        s_map<s_str, s_str>::iterator iter;
        foreach (iter, lCategory)
        {
            s_bool bLoadingResult = PreloadModel(sCategory, iter->first);
            if (!bLoadingResult)
            {
                if (bStopOnError)
                    return false;
                else
                    bResult = false;
            }
        }

        return bResult;
    }

    void ModelManager::RemoveCategory( const s_str& sCategory )
    {
        s_map<s_str, s_str>& lCategory = lModelNameToFileMap_[sCategory];
        s_map<s_str, s_str>::iterator iter;
        foreach (iter, lCategory)
        {
            ModelInfo& mInfo = lLoadedModelList_[iter->second];
            --mInfo.uiRefCount;
            if (mInfo.uiRefCount == 0)
            {
                mInfo.pData.Delete();
                lLoadedModelList_.Erase(iter->second);
            }
        }

        lModelNameToFileMap_.Erase(sCategory);
    }

    s_bool ModelManager::IsModelAvailable( const s_str& sCategory, const s_str& sModelName )
    {
        s_map<s_str, s_str>& lCategory = lModelNameToFileMap_[sCategory];
        s_map<s_str, s_str>::iterator iter = lCategory.Get(sModelName);

        return (iter != lCategory.End());
    }

    s_bool ModelManager::IsModelAvailable( const s_str& sModelName )
    {
        return IsModelAvailable("", sModelName);
    }

    s_ptr<ModelData> ModelManager::LoadModelData_( const s_str& sFile )
    {
        try
        {
            if (sFile.EndsWith(".m2"))
            {
                return new M2ModelData(sFile);
            }
            else if (sFile.EndsWith(".fm"))
            {
                return new FMModelData(sFile);
            }
            else if (sFile.EndsWith(".mesh"))
            {
                return new OgreMeshModelData(sFile);
            }
            else
            {
                s_str sExtension = sFile.Cut(".").Back();
                throw ModelLoadingException(CLASS_NAME, "Model extension \""+sExtension+"\" is not supported.");
            }
        }
        catch (const ModelLoadingException& e)
        {
            Error(CLASS_NAME, "Model \""+sFile+"\" couldn't be loaded : \n"+e.GetDescription());
            return nullptr;
        }
    }
}
