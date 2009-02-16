/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*           GUIManager source            */
/*                                        */
/*  ## : A class that manages the GUI,    */
/*  including parsing, rendering,         */
/*  interacting and destructing.          */
/*                                        */


#include "frost_guimanager.h"

#include "gui/frost_gui_uiobject.h"
#include "gui/frost_gui_anchor.h"

using namespace std;

namespace Frost
{
    const s_str GUIManager::CLASS_NAME = "GUIManager";
    const s_str AddOn::CLASS_NAME = "AddOn";

    GUIManager::GUIManager()
    {
        pLua_ = NULL;
    }

    GUIManager::~GUIManager()
    {
        if (pLua_)
            lua_close(pLua_);
    }

    void GUIManager::Initialize()
    {
        Lua::InitLua(&pLua_);
        Lua::RegisterGUIClasses(pLua_);
        Lua::RegisterGlobalFuncs(pLua_);
    }

    s_bool GUIManager::AddUIObject( s_ptr<GUI::UIObject> pObj )
    {
        map< s_str, s_ptr<GUI::UIObject> >* lNamedList;
        if (pObj->IsVirtual())
        {
            if (pObj->GetParent())
            {
                s_uint i;
                map< s_uint, s_ptr<GUI::UIObject> >::iterator iterObj = lObjectList_.find(i);
                while (iterObj != lObjectList_.end())
                {
                    i++;
                    iterObj = lObjectList_.find(i);
                }
                lObjectList_[i] = pObj;
                pObj->SetID(i);

                return true;
            }
            else
            {
                lNamedList = &lNamedVirtualObjectList_;
            }
        }
        else
        {
            lNamedList = &lNamedObjectList_;
        }

        if (pObj != NULL)
        {
            map< s_str, s_ptr<GUI::UIObject> >::iterator iterNamedObj = lNamedList->find(pObj->GetName());
            if (iterNamedObj == lNamedList->end())
            {
                s_uint i;
                map< s_uint, s_ptr<GUI::UIObject> >::iterator iterObj = lObjectList_.find(i);
                while (iterObj != lObjectList_.end())
                {
                    i++;
                    iterObj = lObjectList_.find(i);
                }
                lObjectList_[i] = pObj;
                (*lNamedList)[pObj->GetName()] = pObj;
                pObj->SetID(i);

                return true;
            }
            else
            {
                Error(CLASS_NAME,
                    "A "+pObj->IsVirtual().GetAsString("virtual", "")+" widget with the name \""
                    +pObj->GetName()+"\" already exists."
                );
                return false;
            }
        }
        else
        {
            Warning(CLASS_NAME, "Adding a NULL widget.");
            return false;
        }
    }

    void GUIManager::RemoveUIObject( s_ptr<GUI::UIObject> pObj )
    {
        map< s_uint, s_ptr<GUI::UIObject> >::iterator iterObj = lObjectList_.find(pObj->GetID());
        if (iterObj != lObjectList_.end())
        {
            lObjectList_.erase(iterObj);
            lNamedObjectList_.erase(lNamedObjectList_.find(pObj->GetName()));
        }
    }

    s_ptr<GUI::UIObject> GUIManager::GetUIObjectByName( const s_str& sName, const s_bool& bVirtual )
    {
        if (bVirtual)
        {
            if (MAPFIND(sName, lNamedVirtualObjectList_))
            {
                return lNamedVirtualObjectList_[sName];
            }
            else
            {
                Warning(CLASS_NAME, "No virtual UIObject with the name \""+sName+"\".");
                return NULL;
            }
        }
        else
        {
            if (MAPFIND(sName, lNamedObjectList_))
            {
                return lNamedObjectList_[sName];
            }
            else
            {
                Warning(CLASS_NAME, "No UIObject with the name \""+sName+"\".");
                return NULL;
            }
        }
    }

    lua_State* GUIManager::GetLua()
    {
        return pLua_;
    }

    void GUIManager::LoadAddOnTOC_( const s_str& sAddOnName, const s_str& sAddOnFolder )
    {
        if (!MAPFIND(sAddOnName, lAddOnList_))
        {
            AddOn mAddOn;
            mAddOn.bEnabled = true;
            mAddOn.sFolder = sAddOnFolder + "/" + sAddOnName;

            File mFile(mAddOn.sFolder + "/" + sAddOnName + ".toc", FILE_I);

            if (mFile.IsOpen())
            {
                while (mFile.IsValid())
                {
                    s_str sLine = mFile.GetLine();
                    if ( (sLine[0] == '#') && (sLine[1] == '#') )
                    {
                        sLine = sLine.Extract(2);
                        sLine.Trim(' ');
                        vector<s_str> lArgs = sLine.Cut(":", 1);
                        if (lArgs.size() == 2)
                        {
                            s_str sKey = lArgs[0];
                            sKey.Trim(' ');
                            s_str sValue = lArgs[1];
                            sValue.Trim(' ');
                            if (sKey == "Interface")
                            {
                                mAddOn.sUIVersion = sValue;

                                s_str sGameVersion = Engine::GetSingleton()->GetConstant("GameVersion").GetS();
                                sGameVersion.Replace(".", "");
                                if (mAddOn.sUIVersion == sGameVersion)
                                    mAddOn.bEnabled = true;
                                else
                                {
                                    Warning(CLASS_NAME,
                                        "Wrong game version for \""+sAddOnName+"\". AddOn disabled."
                                    );
                                    mAddOn.bEnabled = false;
                                }
                            }
                            else if (sKey == "Title")
                            {
                                mAddOn.sName = sValue;
                            }
                            else if (sKey == "Version")
                            {
                                mAddOn.sVersion = sValue;
                            }
                            else if (sKey == "Author")
                            {
                                mAddOn.sAuthor = sValue;
                            }
                            else if (sKey == "SavedVariables")
                            {
                                vector<s_str> lVariables = sValue.Cut(",");
                                vector<s_str>::iterator iterVar;
                                foreach (iterVar, lVariables)
                                {
                                    iterVar->Trim(' ');
                                    mAddOn.lSavedVariableList.push_back(*iterVar);
                                }
                            }
                        }
                    }
                    else
                    {
                        sLine.Trim(' ');
                        s_uint i = sLine.Find(".lua");
                        s_uint j = sLine.Find(".xml");
                        if (i || j)
                        {
                            mAddOn.lFileList.push_back(mAddOn.sFolder + "/" + sLine);
                        }
                    }
                }

                mFile.Close();

                if (mAddOn.sName == "")
                    Error(CLASS_NAME, "Missing AddOn name in "+mFile.GetName()+".");
                else
                    lAddOnList_[mAddOn.sName] = mAddOn;
            }
            else
            {
                Warning(CLASS_NAME,
                    "Missing TOC file for AddOn \""+sAddOnName+"\". Folder ignored."
                );
            }
        }
    }

    void GUIManager::LoadAddOnFiles_( s_ptr<AddOn> pAddOn )
    {
        vector<s_str>::iterator iterFile;
        foreach (iterFile, pAddOn->lFileList)
        {
            s_uint i = iterFile->Find(".lua");
            s_uint j = iterFile->Find(".xml");
            if (i.IsValid())
            {
                Lua::DoFile(pLua_, iterFile->Get());
            }
            else if (j.IsValid())
            {
                this->ParseXMLFile_(iterFile->Get(), pAddOn);
            }
        }
    }

    void GUIManager::LoadAddOnDirectory_( const s_str& sDirectory )
    {
        Directory mDir(sDirectory);
        s_ptr<Directory> pSubDir;

        foreach_subdir (pSubDir, mDir)
        {
            this->LoadAddOnTOC_(pSubDir->GetName(), sDirectory);
        }

        vector< s_ptr<AddOn> > lCoreAddOnStack;
        vector< s_ptr<AddOn> > lAddOnStack;
        s_bool bCore = false;

        File mFile(sDirectory + "/AddOns.txt", FILE_I);
        if (mFile.IsOpen())
        {
            while (mFile.IsValid())
            {
                s_str sLine = mFile.GetLine();
                if (sLine[0] == '#')
                {
                    sLine.EraseFromStart(1);
                    sLine.Trim(' ');
                    if (sLine == "Core")
                        bCore = true;
                    else
                        bCore = false;
                }
                else
                {
                    vector<s_str> lArgs = sLine.Cut(":", 1);
                    if (lArgs.size() == 2)
                    {
                        s_str sKey = lArgs[0];
                        sKey.Trim(' ');
                        s_str sValue = lArgs[1];
                        sValue.Trim(' ');
                        if (MAPFIND(sKey, lAddOnList_))
                        {
                            if (bCore)
                                lCoreAddOnStack.push_back(&lAddOnList_[sKey]);
                            else
                                lAddOnStack.push_back(&lAddOnList_[sKey]);

                            if (sValue != "1")
                            {
                                lAddOnList_[sKey].bEnabled = false;
                            }
                        }
                    }
                }
            }
            mFile.Close();
        }

        vector< s_ptr<AddOn> >::iterator iterAddOn;
        foreach (iterAddOn, lCoreAddOnStack)
        {
            this->LoadAddOnFiles_(*iterAddOn);
        }

        foreach (iterAddOn, lAddOnStack)
        {
            this->LoadAddOnFiles_(*iterAddOn);
        }
    }

    void GUIManager::LoadUI()
    {
        this->LoadAddOnDirectory_("Interface/BaseUI");
        this->LoadAddOnDirectory_("Interface/AddOns");
    }

    void GUIManager::CloseUI()
    {
    }

    void GUIManager::ReloadUI()
    {
    }

    void GUIManager::Update( const s_float& fDelta )
    {
        map< s_str, s_ptr<GUI::UIObject> >::iterator iterObj;
        foreach (iterObj, lNamedObjectList_)
        {
            if (!iterObj->second->GetParent())
                iterObj->second->Update();
        }
    }

    void GUIManager::PrintUI()
    {
        if (lObjectList_.size() >= 1)
        {
            Log("\n\n######################## UIObjects ########################\n\n########################\n");
            map< s_uint, s_ptr<GUI::UIObject> >::iterator iterObj;
            foreach (iterObj, lObjectList_)
            {
                if (!iterObj->second->IsVirtual() && !iterObj->second->GetParent())
                    Log(iterObj->second->Serialize("") + "\n########################\n");
            }

            Log("\n\n######################## Virtual UIObjects ########################\n\n########################\n");
            foreach (iterObj, lObjectList_)
            {
                if (iterObj->second->IsVirtual() && !iterObj->second->GetParent())
                    Log(iterObj->second->Serialize("") + "\n########################\n");
            }
        }
    }
}

