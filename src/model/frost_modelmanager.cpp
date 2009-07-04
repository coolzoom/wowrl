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
        s_map< s_str, s_ptr<ModelData> >::iterator iterModelData;
        foreach (iterModelData, lLoadedModelList_)
        {
            iterModelData->second.Delete();
        }
    }

    s_refptr<Model> ModelManager::CreateModel( const s_str& sModelName, const s_str& sEntityName )
    {
        s_refptr<Model> pModel;
        s_map<s_str, s_str>& mMap = lModelNameToFileMap_[""];

        if (mMap.Find(sModelName))
        {
            s_str sFile = mMap[sModelName];
            if (lLoadedModelList_.Find(sModelName))
            {
                // If this model has already been loaded, just copy it
                pModel = s_refptr<Model>(new Model(*lLoadedModelList_[sFile], sEntityName));
            }
            else
            {
                // Else, load it
                s_ptr<ModelData> pData = lLoadedModelList_[sFile] = s_ptr<ModelData>(new ModelData(sFile, sModelName));

                pModel = s_refptr<Model>(new Model(*pData, sEntityName));
            }
        }
        else
        {
            Warning(CLASS_NAME,
                "No model with the name \""+sModelName+"\". Have you linked it with a file ?"
            );
        }

        return pModel;
    }

    s_refptr<Model> ModelManager::CreateModel( const s_str& sCategory, const s_str& sModelName, const s_str& sEntityName )
    {
        s_refptr<Model> pModel;
        s_map<s_str, s_str>& mMap = lModelNameToFileMap_[sCategory];

        if (mMap.Find(sModelName))
        {
            s_str sFile = mMap[sModelName];
            if (lLoadedModelList_.Find(sModelName))
            {
                // If this model has already been loaded, just copy it
                pModel = s_refptr<Model>(new Model(*lLoadedModelList_[sFile], sEntityName));
            }
            else
            {
                // Else, load it
                s_ptr<ModelData> pData = lLoadedModelList_[sFile] = s_ptr<ModelData>(new ModelData(sFile, sModelName));

                pModel = s_refptr<Model>(new Model(*pData, sEntityName));
            }
        }
        else
        {
            Warning(CLASS_NAME,
                "No model with the name \""+sModelName+"\" in category \""+sCategory+"\". "
                "Have you linked it with a file ?"
            );
        }

        return pModel;
    }

    void ModelManager::LinkModelNameToFile( const s_str& sModelName, const s_str& sFile )
    {
        s_map<s_str, s_str>& mMap = lModelNameToFileMap_[""];
        if (mMap.Find(sModelName))
        {
            Warning(CLASS_NAME,
                "\""+sModelName+"\" is already linked to a file. Previous value erased."
            );
        }

        mMap[sModelName] = sFile;
    }

    void ModelManager::LinkModelNameToFile( const s_str& sCategory, const s_str& sModelName, const s_str& sFile )
    {
        s_map<s_str, s_str>& mMap = lModelNameToFileMap_[sCategory];
        if (mMap.Find(sModelName))
        {
            Warning(CLASS_NAME,
                "\""+sModelName+"\" is already linked to a file. Previous value erased."
            );
        }

        mMap[sModelName] = sFile;
    }

    void ModelManager::ClearLink( const s_str& sModelName )
    {
        s_map<s_str, s_str>& mMap = lModelNameToFileMap_[""];
        s_map<s_str, s_str>::iterator iter = mMap.Get(sModelName);
        if (iter != mMap.End())
        {
            mMap.Erase(iter);
        }
        else
        {
            Warning(CLASS_NAME,
                "Trying to remove link for \""+sModelName+"\", which it is not linked to any file."
            );
        }
    }

    void ModelManager::ClearLink( const s_str& sCategory, const s_str& sModelName )
    {
        s_map<s_str, s_str>& mMap = lModelNameToFileMap_[sCategory];
        s_map<s_str, s_str>::iterator iter = mMap.Get(sModelName);
        if (iter != mMap.End())
        {
            mMap.Erase(iter);
        }
        else
        {
            Warning(CLASS_NAME,
                "Trying to remove link for \""+sModelName+"\" in category \""+sCategory+"\", "
                "which it is not linked to any file."
            );
        }
    }
}
