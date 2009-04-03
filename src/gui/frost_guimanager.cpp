/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*           GUIManager source            */
/*                                        */
/*  ## : A class that manages the GUI,    */
/*  including parsing, rendering,         */
/*  interacting and destructing.          */
/*                                        */


#include "gui/frost_guimanager.h"

#include "gui/frost_gui_uiobject.h"
#include "gui/frost_gui_anchor.h"
#include "frost_lua.h"

using namespace std;

namespace Frost
{
    const s_str GUIManager::CLASS_NAME = "GUIManager";
    const s_str AddOn::CLASS_NAME = "AddOn";

    GUIManager::GUIManager()
    {
    }

    GUIManager::~GUIManager()
    {
        CloseUI();
    }

    void GUIManager::Initialize()
    {
        bClosed_ = true;
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

                if (!pObj->IsVirtual() && !pObj->GetParent())
                    lMainObjectList_[i] = pObj;

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
            if (pObj->IsVirtual())
            {
                map< s_str, s_ptr<GUI::UIObject> >::iterator iterNamed = lNamedVirtualObjectList_.find(pObj->GetName());
                if (iterNamed != lNamedVirtualObjectList_.end())
                {
                    lNamedVirtualObjectList_.erase(lNamedVirtualObjectList_.find(pObj->GetName()));
                }
            }
            else
            {
                map< s_str, s_ptr<GUI::UIObject> >::iterator iterNamed = lNamedObjectList_.find(pObj->GetName());
                if (iterNamed != lNamedObjectList_.end())
                {
                    lNamedObjectList_.erase(lNamedObjectList_.find(pObj->GetName()));
                }
                iterObj = lMainObjectList_.find(pObj->GetID());
                if (iterObj != lMainObjectList_.end())
                {
                    lMainObjectList_.erase(iterObj);
                }
            }
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

    s_ptr<Lua::State> GUIManager::GetLua()
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
                        if (sLine.Find(".lua") || sLine.Find(".xml"))
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
            if (iterFile->Find(".lua"))
            {
                pLua_->DoFile(*iterFile);
            }
            else if (iterFile->Find(".xml"))
            {
                this->ParseXMLFile_(*iterFile, pAddOn);
            }
        }
    }

    void GUIManager::LoadAddOnDirectory_( const s_str& sDirectory )
    {
        Directory mDir(sDirectory);
        s_ptr<Directory> pSubDir;

        foreach_dir (pSubDir, mDir)
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
            if ((*iterAddOn)->bEnabled)
                this->LoadAddOnFiles_(*iterAddOn);
        }

        foreach (iterAddOn, lAddOnStack)
        {
            if ((*iterAddOn)->bEnabled)
                this->LoadAddOnFiles_(*iterAddOn);
        }
    }

    void GUIManager::LoadUI()
    {
        if (bClosed_)
        {
            pLua_ = LuaManager::GetSingleton()->CreateLua();
            Lua::RegisterGUIClasses(pLua_);
            Lua::RegisterGlobalFuncs(pLua_);

            this->LoadAddOnDirectory_("Interface/BaseUI");
            this->LoadAddOnDirectory_("Interface/AddOns");

            bClosed_ = false;
        }
    }

    void GUIManager::CloseUI()
    {
        if (!bClosed_)
        {
            map< s_uint, s_ptr<GUI::UIObject> >::iterator iterObj = lMainObjectList_.begin();
            while (iterObj != lMainObjectList_.end())
            {
                iterObj->second.Delete();
                iterObj = lMainObjectList_.begin();
            }

            map< s_str, s_ptr<GUI::UIObject> >::iterator iterVirtual = lNamedVirtualObjectList_.begin();
            while (iterVirtual != lNamedVirtualObjectList_.end())
            {
                iterVirtual->second.Delete();
                iterVirtual = lNamedVirtualObjectList_.begin();
            }

            LuaManager::GetSingleton()->CloseLua(pLua_);

            bClosed_ = true;
        }
    }

    void GUIManager::ReloadUI()
    {
        CloseUI();
        LoadUI();
    }

    void GUIManager::RenderUI()
    {
        map< s_uint, s_ptr<GUI::UIObject> >::iterator iterObj;
        foreach (iterObj, lMainObjectList_)
        {
            iterObj->second->Render();
        }
    }

    void GUIManager::Update( const s_float& fDelta )
    {
        map< s_uint, s_ptr<GUI::UIObject> >::iterator iterObj;
        foreach (iterObj, lMainObjectList_)
        {
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

