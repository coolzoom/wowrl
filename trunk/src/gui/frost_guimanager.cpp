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
#include "frost_gui.h"

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

    s_uint GUIManager::AddUIObject( s_ptr<GUI::UIObject> pObj )
    {
        if (pObj != NULL)
        {
            map< s_str, s_ptr<GUI::UIObject> >::iterator iterNamedObj = lNamedObjectList_.find(pObj->GetName());
            if (iterNamedObj == lNamedObjectList_.end())
            {
                s_uint i;
                map< s_uint, s_ptr<GUI::UIObject> >::iterator iterObj = lObjectList_.find(i);
                while (iterObj != lObjectList_.end())
                {
                    i++;
                    iterObj = lObjectList_.find(i);
                }
                lObjectList_[i] = pObj;
                lNamedObjectList_[pObj->GetName()] = pObj;

                return i;
            }
            else
            {
                Error(CLASS_NAME, "A widget with the name \""+pObj->GetName()+"\" already exists.");
                return s_uint(s_uint::INTEGER_NAN);
            }
        }
        else
        {
            Warning(CLASS_NAME, "Adding a NULL widget.");
            return s_uint(s_uint::INTEGER_NAN);
        }
    }

    void GUIManager::RemoveUIObject( s_ptr<GUI::UIObject> pObj )
    {
        map< s_uint, s_ptr<GUI::UIObject> >::iterator iterObj = lObjectList_.find(pObj->GetID());
        if (iterObj != lObjectList_.end())
        {
            lObjectList_.erase(iterObj);
        }
    }

    s_ptr<GUI::UIObject> GUIManager::GetUIObjectByName( const s_str& sName )
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

    lua_State* GUIManager::GetLua()
    {
        return pLua_;
    }

    void GUIManager::LoadAddOn( s_str sName, s_str sFolder )
    {
        if (!MAPFIND(sName, lAddOnList_))
        {
            AddOn mAddOn;
            mAddOn.bEnabled = true;
            mAddOn.sFolder = sFolder + sName;
            string sTocFile = (mAddOn.sFolder + "/" + sName + ".toc").Get();
            fstream mFile(sTocFile.c_str(), ios::in);

            if (mFile.is_open())
            {
                s_str sLine;
                while (!mFile.eof())
                {
                    getline(mFile, sLine.GetR());
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

                                s_var mGameVersion = Engine::GetSingleton()->GetConstant("GameVersion");
                                if (mAddOn.sUIVersion == mGameVersion.GetS())
                                    mAddOn.bEnabled = true;
                                else
                                {
                                    Warning(CLASS_NAME,
                                        "Wrong game version for \""+sName+"\". AddOn disabled."
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

                mFile.close();

                if (mAddOn.sName == "")
                    Error(CLASS_NAME, "Missing AddOn name in "+sTocFile+".");
                else
                    lAddOnList_[mAddOn.sName] = mAddOn;
            }
        }
    }

    void GUIManager::LoadUI()
    {
        Directory mDir("Interface/BaseUI");
        s_ptr<Directory> pSubDir = mDir.GetNextSubDirectory();
        while (pSubDir)
        {
            LoadAddOn(pSubDir->GetName(), "Interface/BaseUI");
            pSubDir = mDir.GetNextSubDirectory();
        }

        fstream mFile("Interface/AddOns.txt", ios::in);
        if (mFile.is_open())
        {
            while (!mFile.eof())
            {
                s_str sLine;
                getline(mFile, sLine.GetR());
                vector<s_str> lArgs = sLine.Cut(":", 1);
                if (lArgs.size() == 2)
                {
                    s_str sKey = lArgs[0];
                    sKey.Trim(' ');
                    s_str sValue = lArgs[1];
                    sValue.Trim(' ');
                    if ( MAPFIND(sKey, lAddOnList_) && (sValue != "1") )
                    {
                        lAddOnList_[sKey].bEnabled = false;
                    }
                }
            }
            mFile.close();
        }

        map<s_str, AddOn>::iterator iterAddOn;
        foreach (iterAddOn, lAddOnList_)
        {
            s_ptr<AddOn> pAddOn = &iterAddOn->second;
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
                    /*s_ptr<TiXmlDocument> pDoc = XMLManager::GetSingleton()->OpenFile(*iterFile);
                    if (pDoc)
                    {
                        //XML::ParseUIFile(pAddOn, mDoc);
                    }*/
                }
            }
        }
    }

    void GUIManager::CloseUI()
    {
    }

    void GUIManager::ReloadUI()
    {
    }
}

