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

#include "gui/frost_uiobject.h"
#include "gui/frost_anchor.h"
#include "gui/frost_frame.h"
#include "lua/frost_lua.h"

#include "frost_inputmanager.h"

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
        Log("Closing "+CLASS_NAME+"...");
        CloseUI();
    }

    void GUIManager::Initialize()
    {
        bClosed_ = true;
    }

    s_bool GUIManager::AddUIObject( s_ptr<GUI::UIObject> pObj )
    {
        s_map< s_str, s_ptr<GUI::UIObject> >* lNamedList;
        if (pObj->IsVirtual())
        {
            if (pObj->GetParent())
            {
                s_uint i = 0;
                s_map< s_uint, s_ptr<GUI::UIObject> >::iterator iterObj = lObjectList_.Get(i);
                while (iterObj != lObjectList_.end())
                {
                    i++;
                    iterObj = lObjectList_.Get(i);
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
            s_map< s_str, s_ptr<GUI::UIObject> >::iterator iterNamedObj = lNamedList->Get(pObj->GetName());
            if (iterNamedObj == lNamedList->end())
            {
                s_uint i = 0;
                s_map< s_uint, s_ptr<GUI::UIObject> >::iterator iterObj = lObjectList_.Get(i);
                while (iterObj != lObjectList_.End())
                {
                    i++;
                    iterObj = lObjectList_.Get(i);
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
                    "A "+s_str(pObj->IsVirtual() ? "virtual" : "")+" widget with the name \""
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

    s_ptr<GUI::UIObject> GUIManager::GetUIObjectByName( const s_str& sName, const s_bool& bVirtual )
    {
        if (bVirtual)
        {
            if (lNamedVirtualObjectList_.Find(sName))
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
            if (lNamedObjectList_.Find(sName))
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
        if (!lAddOnList_[sAddOnFolder].Find(sAddOnName))
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
                        s_ctnr<s_str> lArgs = sLine.Cut(":", 1);
                        if (lArgs.GetSize() == 2)
                        {
                            s_str sKey = lArgs[0];
                            sKey.Trim(' ');
                            s_str sValue = lArgs[1];
                            sValue.Trim(' ');
                            if (sKey == "Interface")
                            {
                                mAddOn.sUIVersion = sValue;

                                s_str sGameVersion = Engine::GetSingleton()->GetConstant("GameVersion").Get<s_str>();
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
                                s_ctnr<s_str> lVariables = sValue.Cut(",");
                                s_ctnr<s_str>::iterator iterVar;
                                foreach (iterVar, lVariables)
                                {
                                    iterVar->Trim(' ');
                                    mAddOn.lSavedVariableList.PushBack(*iterVar);
                                }
                            }
                        }
                    }
                    else
                    {
                        sLine.Trim(' ');
                        if (sLine.Find(".lua") || sLine.Find(".xml"))
                        {
                            mAddOn.lFileList.PushBack(mAddOn.sFolder + "/" + sLine);
                        }
                    }
                }

                mFile.Close();

                if (mAddOn.sName == "")
                    Error(CLASS_NAME, "Missing AddOn name in "+mFile.GetName()+".");
                else
                    lAddOnList_[sAddOnFolder][sAddOnName] = mAddOn;
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
        s_ctnr<s_str>::iterator iterFile;
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

        s_ctnr< s_ptr<AddOn> > lCoreAddOnStack;
        s_ctnr< s_ptr<AddOn> > lAddOnStack;
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
                    s_ctnr<s_str> lArgs = sLine.Cut(":", 1);
                    if (lArgs.GetSize() == 2)
                    {
                        s_str sKey = lArgs[0];
                        sKey.Trim(' ');
                        s_str sValue = lArgs[1];
                        sValue.Trim(' ');
                        if (lAddOnList_[sDirectory].Find(sKey))
                        {
                            if (bCore)
                                lCoreAddOnStack.PushBack(&lAddOnList_[sDirectory][sKey]);
                            else
                                lAddOnStack.PushBack(&lAddOnList_[sDirectory][sKey]);

                            if (sValue != "1")
                            {
                                lAddOnList_[sDirectory][sKey].bEnabled = false;
                            }
                        }
                    }
                }
            }
            mFile.Close();
        }

        s_ctnr< s_ptr<AddOn> >::iterator iterAddOn;
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

            s_map< s_uint, s_ptr<GUI::UIObject> >::iterator iterUIObject;
            foreach (iterUIObject, lMainObjectList_)
            {
                s_ptr<GUI::Frame>::DynamicCast(iterUIObject->second)->On("Load");
            }

            bClosed_ = false;
        }
    }

    void GUIManager::CloseUI()
    {
        if (!bClosed_)
        {
            s_map< s_uint, s_ptr<GUI::UIObject> >::iterator iterObj;
            foreach (iterObj, lMainObjectList_)
            {
                iterObj->second.Delete();
            }
            lMainObjectList_.Clear();
            lObjectList_.Clear();
            lNamedObjectList_.Clear();

            s_map< s_str, s_ptr<GUI::UIObject> >::iterator iterVirtual;
            foreach (iterVirtual, lNamedVirtualObjectList_)
            {
                iterVirtual->second.Delete();
            }
            lNamedVirtualObjectList_.Clear();

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
        s_map< s_uint, s_ptr<GUI::UIObject> >::iterator iterObj;
        foreach (iterObj, lMainObjectList_)
        {
            iterObj->second->Render();
        }
    }

    void GUIManager::Update( const s_float& fDelta )
    {
        if (pMovedObject_ || pSizedObject_)
        {
            fMouseMovementX_ += InputManager::GetSingleton()->GetMouseDX();
            fMouseMovementY_ += InputManager::GetSingleton()->GetMouseDY();
        }

        s_map< s_uint, s_ptr<GUI::UIObject> >::iterator iterObj;
        foreach (iterObj, lMainObjectList_)
        {
            iterObj->second->Update();
        }
    }

    void GUIManager::StartMoving( s_ptr<GUI::UIObject> pObj )
    {
        pSizedObject_ = NULL;
        pMovedObject_ = pObj;
        fMouseMovementX_ = 0.0f;
        fMouseMovementY_ = 0.0f;
    }

    void GUIManager::StopMoving( s_ptr<GUI::UIObject> pObj )
    {
        if (pMovedObject_ == pObj)
            pMovedObject_ = NULL;
    }

    s_bool GUIManager::IsMoving( s_ptr<GUI::UIObject> pObj ) const
    {
        return (pMovedObject_ == pObj);
    }

    void GUIManager::StartSizing( s_ptr<GUI::UIObject> pObj )
    {
        pMovedObject_  = NULL;
        pSizedObject_ = pObj;
        fMouseMovementX_ = 0.0f;
        fMouseMovementY_ = 0.0f;
    }

    void GUIManager::StopSizing( s_ptr<GUI::UIObject> pObj )
    {
        if (pSizedObject_ == pObj)
            pSizedObject_ = NULL;
    }

    s_bool GUIManager::IsSizing( s_ptr<GUI::UIObject> pObj ) const
    {
        return (pSizedObject_ == pObj);
    }

    s_int GUIManager::GetMovementX() const
    {
        return s_int(fMouseMovementX_);
    }

    s_int GUIManager::GetMovementY() const
    {
        return s_int(fMouseMovementY_);
    }

    void GUIManager::PrintUI()
    {
        if (lAddOnList_.GetSize() >= 1)
        {
            Log("\n\n######################## Loaded AddOns ########################\n");
            s_map< s_str, s_map<s_str, AddOn> >::iterator iterFolder;
            foreach (iterFolder, lAddOnList_)
            {
                Log("# Folder : "+iterFolder->first+"\n|-###");
                s_map<s_str, AddOn>::iterator iterAdd;
                foreach (iterAdd, iterFolder->second)
                {
                    if (iterAdd->second.bEnabled)
                        Log("|   # "+iterAdd->first);
                }
                Log("|-###\n#");
            }
        }
        if (lObjectList_.GetSize() >= 1)
        {
            Log("\n\n######################## UIObjects ########################\n\n########################\n");
            s_map< s_uint, s_ptr<GUI::UIObject> >::iterator iterObj;
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

