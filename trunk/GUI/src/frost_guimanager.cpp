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

#include "frost_anchor.h"
#include "frost_uiobject.h"
#include "frost_button.h"
#include "frost_checkbutton.h"
#include "frost_editbox.h"
#include "frost_messageframe.h"
#include "frost_scrollframe.h"
#include "frost_frame.h"
#include "frost_scrollingmessageframe.h"
#include "frost_slider.h"
#include "frost_statusbar.h"
#include "frost_texture.h"
#include "frost_fontstring.h"
#include "frost_fontmanager.h"
#include "frost_rendertarget.h"

#include "frost_inputmanager.h"
#include "frost_localemanager.h"

#include <frost_xml_document.h>
#include <frost_utils_file.h>
#include <frost_utils_directory.h>
#include <frost_utils_event.h>
#include <frost_utils_eventmanager.h>

#if FRONT_END_OGRE
    #include "impl/ogre/ogre_guimanager.h"
#elif FRONT_END_SFML
    #include "impl/sfml/sfml_guimanager.h"
#else
    #error "No implementation defined for GUIManager !"
#endif

using namespace std;

namespace Frost
{
    const s_str GUIManager::CLASS_NAME = "GUIManager";
    const s_str AddOn::CLASS_NAME = "AddOn";
    const s_str Level::CLASS_NAME  = "GUI::Level";
    const s_str Strata::CLASS_NAME = "GUI::Strata";

    int l_SetKeyBinding(lua_State* pLua);
    int l_CreateFrame(lua_State* pLua);
    int l_DeleteFrame(lua_State* pLua);
    int l_GetLocale(lua_State* pLua);
    int l_Log(lua_State* pLua);
    int l_Exit(lua_State* pLua);

    void Shutdown();

    template<class T>
    T GetValue( const s_map<s_str, s_var>& lParams, const s_str& sName, const s_str& sType )
    {
        s_map<s_str, s_var>::const_iterator iter = lParams.Get(sName);

        if (iter == lParams.End())
            throw Exception("GUIManager", "Missing \""+sName+"\" parameter in Initialize().");
        if (!iter->second.IsOfType<T>())
            throw Exception("GUIManager", "\""+sName+"\" must be a "+sType+" (be sure to explicitely set the type).");

        return iter->second.Get<T>();
    }

    GUIManager::GUIManager( const s_map<s_str, s_var>& lParams )
    {
        sGameVersion_   = GetValue<s_str>(lParams, "game_version", "s_str");
        uiScreenWidth_  = GetValue<s_uint>(lParams, "width", "s_uint");
        uiScreenHeight_ = GetValue<s_uint>(lParams, "height", "s_uint");

        pImpl_ = s_refptr<GUIManagerImpl>(new GUIManagerImpl(lParams));

        RegisterEvent("KEY_PRESSED");

        bClosed_ = true;
        bEnableCaching_ = true;
        bEnabled_ = true;
        bClearFontsOnClose_ = true;
    }

    GUIManager::~GUIManager()
    {
        CloseUI();

        s_map<FrameStrata, Strata>::iterator iterStrata;
        foreach (iterStrata, lStrataList_)
        {
            iterStrata->second.pRenderTarget.Delete();
        }
    }

    s_wptr<GUIManagerImpl> GUIManager::GetImpl()
    {
        return pImpl_;
    }

    void GUIManager::Enable()
    {
        bEnabled_ = true;
    }

    void GUIManager::Disable()
    {
        bEnabled_ = false;
    }

    const s_bool& GUIManager::IsEnabled() const
    {
        return bEnabled_;
    }

    const s_uint& GUIManager::GetScreenWidth() const
    {
        return uiScreenWidth_;
    }

    const s_uint& GUIManager::GetScreenHeight() const
    {
        return uiScreenHeight_;
    }

    void GUIManager::AddAddOnDirectory( const s_str& sDirectory )
    {
        if (!lGUIDirectoryList_.Find(sDirectory))
            lGUIDirectoryList_.PushBack(sDirectory);
    }

    void GUIManager::ClearAddOnDirectoryList()
    {
        lGUIDirectoryList_.Clear();
    }

    s_bool GUIManager::CheckUIObjectName( const s_str& sName ) const
    {
        if (s_str::IsNumber(sName[0]))
        {
            Error(CLASS_NAME,
                "A widget's name can't start by a number : \""+sName+"\" is forbidden."
            );
            return false;
        }

        s_uint iPos = sName.FindPos("$");
        if (iPos.IsValid() && iPos != 0)
        {
            Error(CLASS_NAME,
                "A widget's name can't contain the character '$' except at the begining : \""+sName+"\" is forbidden."
            );
            return false;
        }

        s_str::const_iterator iterName; s_uint i;
        foreach (iterName, sName)
        {
            s_char c = *iterName;
            if (!s_str::IsAlphaNumeric(c) && c != '_' && c != '$')
            {
                Error(CLASS_NAME,
                    "A widget's name can only contain alpha numeric symbols, or underscores : \""+sName+"\" is forbidden."
                );
                return false;
            }
            ++i;
        }

        return true;
    }

    s_ptr<GUI::UIObject> GUIManager::CreateUIObject( const s_str& sClassName )
    {
        if (sClassName == "Frame")
            return new GUI::Frame(this);
        else if (sClassName == "Button")
            return new GUI::Button(this);
        else if (sClassName == "CheckButton")
            return new GUI::CheckButton(this);
        else if (sClassName == "EditBox")
            return new GUI::EditBox(this);
        else if (sClassName == "MessageFrame")
            return new GUI::MessageFrame(this);
        else if (sClassName == "ScrollFrame")
            return new GUI::ScrollFrame(this);
        else if (sClassName == "ScrollingMessageFrame")
            return new GUI::ScrollingMessageFrame(this);
        else if (sClassName == "Slider")
            return new GUI::Slider(this);
        else if (sClassName == "StatusBar")
            return new GUI::StatusBar(this);
        else if (sClassName == "Texture")
            return new GUI::Texture(this);
        else if (sClassName == "FontString")
            return new GUI::FontString(this);
        else
        {
            if (lCustomFrameList_.Find(sClassName))
                return (*lCustomFrameList_[sClassName])(this);
            else if (lCustomRegionList_.Find(sClassName))
                return (*lCustomRegionList_[sClassName])(this);
            else
            {
                Warning(CLASS_NAME, "Unkown UIObject class : \""+sClassName+"\".");
                return nullptr;
            }
        }
    }

    s_ptr<GUI::Frame> GUIManager::CreateFrame( const s_str& sClassName )
    {
        if (sClassName == "Frame")
            return new GUI::Frame(this);
        else if (sClassName == "Button")
            return new GUI::Button(this);
        else if (sClassName == "CheckButton")
            return new GUI::CheckButton(this);
        else if (sClassName == "EditBox")
            return new GUI::EditBox(this);
        else if (sClassName == "MessageFrame")
            return new GUI::MessageFrame(this);
        else if (sClassName == "ScrollFrame")
            return new GUI::ScrollFrame(this);
        else if (sClassName == "ScrollingMessageFrame")
            return new GUI::ScrollingMessageFrame(this);
        else if (sClassName == "Slider")
            return new GUI::Slider(this);
        else if (sClassName == "StatusBar")
            return new GUI::StatusBar(this);
        else
        {
            if (lCustomFrameList_.Find(sClassName))
                return (*lCustomFrameList_[sClassName])(this);
            else
            {
                Warning(CLASS_NAME, "Unkown Frame class : \""+sClassName+"\".");
                return nullptr;
            }
        }
    }

    s_ptr<GUI::Frame> GUIManager::CreateFrame( const s_str& sClassName, const s_str& sName, s_ptr<GUI::Frame> pParent, const s_str& sInheritance )
    {
        if (!CheckUIObjectName(sName))
            return nullptr;

        s_ptr<GUI::Frame> pNewFrame = CreateFrame(sClassName);
        if (!pNewFrame)
            return nullptr;

        pNewFrame->SetParent(pParent);
        pNewFrame->SetName(sName);

        if (GetUIObjectByName(pNewFrame->GetName()))
        {
            Error(CLASS_NAME,
                "An object with the name \""+pNewFrame->GetName()+"\" already exists."
            );
            pNewFrame.Delete();
            return nullptr;
        }

        AddUIObject(pNewFrame);
        pNewFrame->CreateGlue();

        if (pParent)
        {
            pParent->AddChild(pNewFrame);
            pNewFrame->SetLevel(pParent->GetFrameLevel() + 1);
        }
        else
            pNewFrame->SetLevel(0);

        if (!sInheritance.IsEmpty(true))
        {
            s_ctnr<s_str> lObjects = sInheritance.Cut(",");
            s_ctnr<s_str>::iterator iter;
            foreach (iter, lObjects)
            {
                iter->Trim(' ');
                s_ptr<GUI::UIObject> pObj = GetUIObjectByName(*iter, true);
                if (pObj)
                {
                    if (pNewFrame->IsObjectType(pObj->GetObjectType()))
                    {
                        // Inherit from the other Frame
                        pNewFrame->CopyFrom(pObj);
                    }
                    else
                    {
                        Warning(CLASS_NAME,
                            "\""+pNewFrame->GetName()+"\" ("+pNewFrame->GetObjectType()+") cannot inherit "
                            "from \""+(*iter)+"\" ("+pObj->GetObjectType()+"). Inheritance skipped."
                        );
                    }
                }
                else
                {
                    Warning(CLASS_NAME,
                        "Couldn't find inherited object \""+(*iter)+"\". Inheritance skipped."
                    );
                }
            }
        }

        pNewFrame->SetNewlyCreated();
        pNewFrame->On("Load");
        pNewFrame->NotifyLoaded();

        return pNewFrame;
    }

    s_ptr<GUI::LayeredRegion> GUIManager::CreateLayeredRegion( const s_str& sClassName )
    {
        if (sClassName == "Texture")
            return new GUI::Texture(this);
        else if (sClassName == "FontString")
            return new GUI::FontString(this);
        else
        {
            if (lCustomRegionList_.Find(sClassName))
                return (*lCustomRegionList_[sClassName])(this);
            else
            {
                Warning(CLASS_NAME, "Unkown LayeredRegion class : \""+sClassName+"\".");
                return nullptr;
            }
        }
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
                    ++i;
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

        if (pObj)
        {
            s_map< s_str, s_ptr<GUI::UIObject> >::iterator iterNamedObj = lNamedList->Get(pObj->GetName());
            if (iterNamedObj == lNamedList->end())
            {
                s_uint i = 0;
                s_map< s_uint, s_ptr<GUI::UIObject> >::iterator iterObj = lObjectList_.Get(i);
                while (iterObj != lObjectList_.End())
                {
                    ++i;
                    iterObj = lObjectList_.Get(i);
                }
                lObjectList_[i] = pObj;
                (*lNamedList)[pObj->GetName()] = pObj;
                pObj->SetID(i);

                if (!pObj->IsVirtual())
                {
                    if (!pObj->GetParent())
                        lMainObjectList_[i] = pObj;

                    s_ptr<GUI::Frame> pFrame = s_ptr<GUI::Frame>::DynamicCast(pObj);
                    if (pFrame)
                        lFrameList_[i] = pFrame;
                }

                return true;
            }
            else
            {
                Warning(CLASS_NAME,
                    "A "+s_str(pObj->IsVirtual() ? "virtual" : "")+" widget with the name \""
                    +pObj->GetName()+"\" already exists."
                );
                return false;
            }
        }
        else
        {
            Warning(CLASS_NAME, "Adding a null widget.");
            return false;
        }
    }

    void GUIManager::RemoveUIObject( s_ptr<GUI::UIObject> pObj )
    {
        if (!pObj)
            return;

        lObjectList_.Erase(pObj->GetID());

        if (pObj->IsVirtual())
        {
            lNamedVirtualObjectList_.Erase(pObj->GetName());
        }
        else
        {
            lNamedObjectList_.Erase(pObj->GetName());

            if (!pObj->GetParent())
                lMainObjectList_.Erase(pObj->GetID());

            s_ptr<GUI::Frame> pFrame = s_ptr<GUI::Frame>::DynamicCast(pObj);
            if (pFrame)
                lFrameList_.Erase(pObj->GetID());
        }

        if (!pObj->IsManuallyRendered())
            FireBuildStrataList();

        if (pMovedObject_ == pObj)
            StopMoving(pObj);

        if (pSizedObject_ == pObj)
            StopSizing(pObj);
    }

    void GUIManager::NotifyObjectHasParent( s_ptr<GUI::UIObject> pObj )
    {
        lMainObjectList_.Erase(pObj->GetID());
    }

    s_ptr<const GUI::UIObject> GUIManager::GetUIObject( const s_uint& uiID ) const
    {
        s_map< s_uint, s_ptr<GUI::UIObject> >::const_iterator iter = lObjectList_.Get(uiID);
        if (iter != lObjectList_.End())
            return iter->second;
        else
            return nullptr;
    }

    s_ptr<GUI::UIObject> GUIManager::GetUIObject( const s_uint& uiID )
    {
        s_map< s_uint, s_ptr<GUI::UIObject> >::iterator iter = lObjectList_.Get(uiID);
        if (iter != lObjectList_.End())
            return iter->second;
        else
            return nullptr;
    }

    s_ptr<const GUI::UIObject> GUIManager::GetUIObjectByName( const s_str& sName, const s_bool& bVirtual ) const
    {
        if (bVirtual)
        {
            s_map< s_str, s_ptr<GUI::UIObject> >::const_iterator iter = lNamedVirtualObjectList_.Get(sName);
            if (iter != lNamedVirtualObjectList_.End())
                return iter->second;
            else
                return nullptr;
        }
        else
        {
            s_map< s_str, s_ptr<GUI::UIObject> >::const_iterator iter = lNamedObjectList_.Get(sName);
            if (iter != lNamedObjectList_.End())
                return iter->second;
            else
                return nullptr;
        }
    }

    s_ptr<GUI::UIObject> GUIManager::GetUIObjectByName( const s_str& sName, const s_bool& bVirtual )
    {
        if (bVirtual)
        {
            s_map< s_str, s_ptr<GUI::UIObject> >::iterator iter = lNamedVirtualObjectList_.Get(sName);
            if (iter != lNamedVirtualObjectList_.End())
                return iter->second;
            else
                return nullptr;
        }
        else
        {
            s_map< s_str, s_ptr<GUI::UIObject> >::iterator iter = lNamedObjectList_.Get(sName);
            if (iter != lNamedObjectList_.End())
                return iter->second;
            else
                return nullptr;
        }
    }

    s_ptr<Lua::State> GUIManager::GetLua()
    {
        return pLua_;
    }

    void GUIManager::LoadAddOnTOC_( const s_str& sAddOnName, const s_str& sAddOnDirectory )
    {
        if (!lAddOnList_[sAddOnDirectory].Find(sAddOnName))
        {
            AddOn mAddOn;
            mAddOn.bEnabled = true;
            mAddOn.sMainDirectory = sAddOnDirectory.Cut("/").Back();
            mAddOn.sDirectory = sAddOnDirectory + "/" + sAddOnName;

            File mFile(mAddOn.sDirectory + "/" + sAddOnName + ".toc", File::I);

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

                                if (mAddOn.sUIVersion == sGameVersion_)
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
                                    if (!iterVar->IsEmpty(true))
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
                            mAddOn.lFileList.PushBack(mAddOn.sDirectory + "/" + sLine);
                        }
                    }
                }

                mFile.Close();

                if (mAddOn.sName == "")
                    Error(CLASS_NAME, "Missing AddOn name in "+mFile.GetName()+".");
                else
                    lAddOnList_[sAddOnDirectory][sAddOnName] = mAddOn;
            }
            else
            {
                Warning(CLASS_NAME,
                    "Missing TOC file for AddOn \""+sAddOnName+"\". Directory ignored."
                );
            }
        }
    }

    void GUIManager::LoadAddOnFiles_( s_ptr<AddOn> pAddOn )
    {
        pCurrentAddOn_ = pAddOn;
        s_ctnr<s_str>::iterator iterFile;
        foreach (iterFile, pAddOn->lFileList)
        {
            if (iterFile->Find(".lua"))
            {
                try
                {
                    pLua_->DoFile(*iterFile);
                }
                catch (LuaException& e)
                {
                    s_str sError = e.GetDescription();

                    Error("", sError);

                    Event mEvent("LUA_ERROR");
                    mEvent.Add(sError);
                    EventManager::GetSingleton()->FireEvent(mEvent);
                }
            }
            else if (iterFile->Find(".xml"))
            {
                this->ParseXMLFile_(*iterFile, pAddOn);
            }
        }

        s_str sSavedVariablesFile = "Saves/Interface/"+pAddOn->sMainDirectory+"/"+pAddOn->sName+".lua";
        if (File::Exists(sSavedVariablesFile))
        {
            try
            {
                pLua_->DoFile(sSavedVariablesFile);
            }
            catch (LuaException& e)
            {
                s_str sError = e.GetDescription();

                Error("", sError);

                Event mEvent("LUA_ERROR");
                mEvent.Add(sError);
                EventManager::GetSingleton()->FireEvent(mEvent);
            }
        }

        Event mEvent("ADDON_LOADED");
        mEvent.Add(pAddOn->sName);
        EventManager::GetSingleton()->FireEvent(mEvent);
    }

    void GUIManager::LoadAddOnDirectory_( const s_str& sDirectory )
    {
        Directory mDir(sDirectory);
        s_wptr<Directory> pSubDir;

        foreach_dir (pSubDir, mDir)
        {
            this->LoadAddOnTOC_(pSubDir->GetName(), sDirectory);
        }

        s_ctnr< s_ptr<AddOn> > lCoreAddOnStack;
        s_ctnr< s_ptr<AddOn> > lAddOnStack;
        s_bool bCore = false;

        File mFile(sDirectory + "/AddOns.txt", File::I);
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

        pCurrentAddOn_ = nullptr;
    }

    void GUIManager::SaveVariables_( s_ptr<AddOn> pAddOn )
    {
        if (!pAddOn->lSavedVariableList.IsEmpty())
        {
            File mFile("Saves/Interface/"+pAddOn->sMainDirectory+"/"+pAddOn->sName+".lua", File::O);
            s_ctnr<s_str>::iterator iterVariable;
            foreach (iterVariable, pAddOn->lSavedVariableList)
            {
                s_str sVariable = pLua_->SerializeGlobal(*iterVariable);
                if (!sVariable.IsEmpty())
                    mFile.WriteLine(sVariable);
            }
        }
    }

    void GUIManager::CreateLua()
    {
        if (!pLua_)
        {
            pLua_ = LuaManager::GetSingleton()->CreateLua();

            pLua_->Register<LuaGUIManager>();
            pLua_->Register<GUI::LuaVirtualGlue>();
            pLua_->Register<GUI::LuaUIObject>();
            pLua_->Register<GUI::LuaFrame>();
            pLua_->Register<GUI::LuaButton>();
            pLua_->Register<GUI::LuaCheckButton>();
            pLua_->Register<GUI::LuaEditBox>();
            pLua_->Register<GUI::LuaMessageFrame>();
            pLua_->Register<GUI::LuaScrollFrame>();
            pLua_->Register<GUI::LuaScrollingMessageFrame>();
            pLua_->Register<GUI::LuaSlider>();
            pLua_->Register<GUI::LuaStatusBar>();
            pLua_->Register<GUI::LuaLayeredRegion>();
            pLua_->Register<GUI::LuaTexture>();
            pLua_->Register<GUI::LuaFontString>();

            s_ptr<LuaGUIManager> pLuaMgr = pLua_->PushNew<LuaGUIManager>();
            pLuaMgr->SetManager(this);
            pLua_->SetGlobal("_MGR");

            pLua_->DoFile("KeyCodes.lua");
            pLua_->DoFile("ClassHelper.lua");

            pLua_->Register("SetKeyBinding", l_SetKeyBinding);
            pLua_->Register("CreateFrame",   l_CreateFrame);
            pLua_->Register("DeleteFrame",   l_DeleteFrame);
            pLua_->Register("GetLocale",     l_GetLocale);
            pLua_->Register("Log",           l_Log);
            pLua_->Register("Exit",          l_Exit);

            // Create the "this" stack
            iThisStackSize_ = 0u;
            pLua_->NewTable();
            pLua_->SetGlobal("_this_stack");
        }
    }

    void GUIManager::ReadFiles()
    {
        if (bClosed_)
        {
            bLoadingUI_ = true;
            s_ctnr<s_str>::iterator iterDirectory;
            foreach (iterDirectory, lGUIDirectoryList_)
            {
                this->LoadAddOnDirectory_(*iterDirectory);
            }

            if (bEnableCaching_)
            {
                // Get the active strata list
                s_map< s_uint, s_ptr<GUI::Frame> >::iterator iterFrame;
                foreach (iterFrame, lFrameList_)
                {
                    s_ptr<GUI::Frame> pFrame = iterFrame->second;
                    if (!pFrame->IsManuallyRendered())
                    {
                        lStrataList_[pFrame->GetFrameStrata()];
                    }
                }

                // Create their render target
                s_map<FrameStrata, Strata>::iterator iterStrata;
                foreach (iterStrata, lStrataList_)
                {
                    CreateStrataRenderTarget_(iterStrata->second);
                }
            }

            bLoadingUI_ = false;
            bClosed_ = false;
        }
    }

    void GUIManager::LoadUI()
    {
        CreateLua();
        ReadFiles();
    }

    void GUIManager::CloseUI()
    {
        if (!bClosed_)
        {
            s_ctnr<s_str>::iterator iterDirectory;
            foreach (iterDirectory, lGUIDirectoryList_)
            {
                s_map<s_str, AddOn>::iterator iterAddOn;
                foreach (iterAddOn, lAddOnList_[*iterDirectory])
                {
                    this->SaveVariables_(&iterAddOn->second);
                }
            }

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

            lFrameList_.Clear();

            s_map<FrameStrata, Strata>::iterator iterStrata;
            foreach (iterStrata, lStrataList_)
            {
                iterStrata->second.lLevelList.Clear();
                iterStrata->second.bRedraw = true;
            }

            lAddOnList_.Clear();

            LuaManager::GetSingleton()->CloseLua(pLua_);
            pLua_ = nullptr;

            pOveredFrame_ = nullptr;
            pFocusedEditBox_ = nullptr;
            pMovedObject_ = nullptr;
            pSizedObject_ = nullptr;
            pMovedAnchor_ = nullptr;
            bObjectMoved_ = false;
            iThisStackSize_ = 0;

            if (bClearFontsOnClose_)
                GUI::FontManager::GetSingleton()->ClearFonts();

            bClosed_ = true;
        }
    }

    void GUIManager::ReloadUI()
    {
        Log("Closing UI...");
        CloseUI();
        Log("Done. Loading UI...");
        LoadUI();
        Log("Done.");
    }

    void GUIManager::RenderUI()
    {
        if (bEnableCaching_)
        {
            if (pSprite_)
                pSprite_->Render(0, 0);
        }
        else
        {
            s_map<FrameStrata, Strata>::iterator iterStrata;
            foreach (iterStrata, lStrataList_)
            {
                Strata& mStrata = iterStrata->second;
                s_map<s_int, Level>::const_iterator iterLevel;
                foreach (iterLevel, mStrata.lLevelList)
                {
                    const Level& mLevel = iterLevel->second;

                    s_ctnr< s_ptr<GUI::Frame> >::const_iterator iterFrame;
                    foreach (iterFrame, mLevel.lFrameList)
                    {
                        s_ptr<GUI::Frame> pFrame = *iterFrame;
                        if (!pFrame->IsNewlyCreated())
                            pFrame->Render();
                    }
                }

                ++mStrata.uiRedrawCount;
            }
        }
    }

    void GUIManager::CreateStrataRenderTarget( FrameStrata mFrameStrata )
    {
        CreateStrataRenderTarget_(lStrataList_[mFrameStrata]);
    }

    void GUIManager::CreateStrataRenderTarget_( Strata& mStrata )
    {
        if (!pRenderTarget_)
        {
            try
            {
                pRenderTarget_ = new GUI::RenderTarget(uiScreenWidth_, uiScreenHeight_);
            }
            catch (Exception& e)
            {
                Error(CLASS_NAME,
                    "Unable to create RenderTarget for GUI caching :\n"+e.GetDescription()
                );

                bEnableCaching_ = false;
                return;
            }

            pSprite_ = s_refptr<GUI::Sprite>(new GUI::Sprite(this, pRenderTarget_));
        }

        if (!mStrata.pRenderTarget)
        {
            try
            {
                mStrata.pRenderTarget = new GUI::RenderTarget(uiScreenWidth_, uiScreenHeight_);
            }
            catch (Exception& e)
            {
                Error(CLASS_NAME,
                    "Unable to create RenderTarget for strata "+mStrata.uiID+" :\n"+e.GetDescription()
                );

                bEnableCaching_ = false;
                return;
            }

            mStrata.pSprite = s_refptr<GUI::Sprite>(new GUI::Sprite(this, mStrata.pRenderTarget));
        }
    }

    void GUIManager::RenderStrata_( Strata& mStrata )
    {
        if (!mStrata.pRenderTarget)
            CreateStrataRenderTarget_(mStrata);

        if (mStrata.pRenderTarget)
        {
            Begin(mStrata.pRenderTarget);
            mStrata.pRenderTarget->Clear(Color::VOID);

            s_map<s_int, Level>::const_iterator iterLevel;
            foreach (iterLevel, mStrata.lLevelList)
            {
                const Level& mLevel = iterLevel->second;

                s_ctnr< s_ptr<GUI::Frame> >::const_iterator iterFrame;
                foreach (iterFrame, mLevel.lFrameList)
                {
                    s_ptr<GUI::Frame> pFrame = *iterFrame;
                    if (!pFrame->IsNewlyCreated())
                        pFrame->Render();
                }
            }

            End();

            ++mStrata.uiRedrawCount;
        }
    }

    const s_bool& GUIManager::IsLoadingUI() const
    {
        return bLoadingUI_;
    }

    void GUIManager::FireBuildStrataList()
    {
        bBuildStrataList_ = true;
    }

    void GUIManager::Update( const s_float& fDelta )
    {
        if (pMovedObject_ || pSizedObject_)
        {
            fMouseMovementX_ += InputManager::GetSingleton()->GetMouseRawDX();
            fMouseMovementY_ += InputManager::GetSingleton()->GetMouseRawDY();
        }

        if (pMovedObject_)
        {
            switch (mConstraint_)
            {
                case Vector::CONSTRAINT_XY :
                case Vector::CONSTRAINT_NONE :
                    pMovedAnchor_->SetAbsOffset(
                        iMovementStartPositionX_ + s_int(fMouseMovementX_),
                        iMovementStartPositionY_ + s_int(fMouseMovementY_)
                    );
                    break;
                case Vector::CONSTRAINT_ZX :
                case Vector::CONSTRAINT_X :
                    pMovedAnchor_->SetAbsOffset(
                        iMovementStartPositionX_ + s_int(fMouseMovementX_),
                        iMovementStartPositionY_
                    );
                    break;
                case Vector::CONSTRAINT_YZ :
                case Vector::CONSTRAINT_Y :
                    pMovedAnchor_->SetAbsOffset(
                        iMovementStartPositionX_,
                        iMovementStartPositionY_ + s_int(fMouseMovementY_)
                    );
                    break;
                default : break;
            }

            pMovedObject_->FireUpdateBorders();
        }
        else if (pSizedObject_)
        {
            if (bResizeWidth_)
            {
                s_uint uiWidth;
                if (bResizeFromRight_)
                    uiWidth = s_uint(
                        s_int::Max(0, s_int(uiResizeStartW_) + s_int(fMouseMovementX_))
                    );
                else
                    uiWidth = s_uint(
                        s_int::Max(0, s_int(uiResizeStartW_) - s_int(fMouseMovementX_))
                    );

                pSizedObject_->SetAbsWidth(uiWidth);
            }
            if (bResizeHeight_)
            {
                s_uint uiHeight;
                if (bResizeFromBottom_)
                    uiHeight = s_uint(
                        s_int::Max(0, s_int(uiResizeStartH_) + s_int(fMouseMovementY_))
                    );
                else
                    uiHeight = s_uint(
                        s_int::Max(0, s_int(uiResizeStartH_) - s_int(fMouseMovementY_))
                    );

                pSizedObject_->SetAbsHeight(uiHeight);
            }
        }

        // Update anchors for all widgets
        s_map< s_uint, s_ptr<GUI::UIObject> >::iterator iterObj;
        foreach (iterObj, lObjectList_)
        {
            if (!iterObj->second->IsVirtual())
                iterObj->second->UpdateAnchors();
        }

        // ... then update logics on main widgets from parent to childs.
        foreach (iterObj, lMainObjectList_)
        {
            if (!iterObj->second->IsVirtual())
                iterObj->second->Update();
        }

        if (bBuildStrataList_)
        {
            s_map<FrameStrata, Strata>::iterator iterStrata;
            foreach (iterStrata, lStrataList_)
            {
                iterStrata->second.lLevelList.Clear();
                iterStrata->second.bRedraw = true;
            }

            s_map< s_uint, s_ptr<GUI::Frame> >::iterator iterFrame;
            foreach (iterFrame, lFrameList_)
            {
                s_ptr<GUI::Frame> pFrame = iterFrame->second;
                if (!pFrame->IsManuallyRendered())
                {
                    Strata& mStrata = lStrataList_[pFrame->GetFrameStrata()];
                    mStrata.lLevelList[pFrame->GetFrameLevel()].lFrameList.PushBack(pFrame);

                    mStrata.uiID = pFrame->GetFrameStrata();
                }
            }
        }

        if (bEnableCaching_)
        {
            s_bool bRedraw;
            s_map<FrameStrata, Strata>::iterator iterStrata;
            foreach (iterStrata, lStrataList_)
            {
                Strata& mStrata = iterStrata->second;

                if (mStrata.bRedraw)
                {
                    RenderStrata_(mStrata);
                    bRedraw = true;
                }

                mStrata.bRedraw = false;
            }

            if (bRedraw && pRenderTarget_)
            {
                Begin(pRenderTarget_);
                pRenderTarget_->Clear(Color::VOID);

                s_map<FrameStrata, Strata>::const_iterator iterStrata;
                foreach (iterStrata, lStrataList_)
                {
                    if (iterStrata->second.pSprite)
                        iterStrata->second.pSprite->Render(0, 0);
                }

                End();
            }
        }

        if ( bBuildStrataList_ || bObjectMoved_ ||
            (InputManager::GetSingleton()->GetMouseRawDX() != 0.0f) ||
            (InputManager::GetSingleton()->GetMouseRawDY() != 0.0f))
        {
            s_int iX = s_int(InputManager::GetSingleton()->GetMousePosX());
            s_int iY = s_int(InputManager::GetSingleton()->GetMousePosY());
            s_ptr<GUI::Frame> pOveredFrame;

            s_map<FrameStrata, Strata>::const_iterator iterStrata = lStrataList_.End();
            while (iterStrata != lStrataList_.Begin() && !pOveredFrame)
            {
                --iterStrata;
                const Strata& mStrata = iterStrata->second;

                s_map<s_int, Level>::const_iterator iterLevel = mStrata.lLevelList.End();
                while (iterLevel != mStrata.lLevelList.Begin() && !pOveredFrame)
                {
                    --iterLevel;
                    const Level& mLevel = iterLevel->second;

                    s_ctnr< s_ptr<GUI::Frame> >::const_iterator iterFrame;
                    foreach (iterFrame, mLevel.lFrameList)
                    {
                        s_ptr<GUI::Frame> pFrame = *iterFrame;
                        if (pFrame->IsMouseEnabled() && pFrame->IsVisible() && pFrame->IsInFrame(iX, iY))
                        {
                            pOveredFrame = pFrame;
                            break;
                        }
                    }
                }
            }

            if (pOveredFrame && !pOveredFrame->IsWorldInputAllowed())
                InputManager::GetSingleton()->BlockClicks("WORLD");
            else
                InputManager::GetSingleton()->AllowClicks("WORLD");

            if (pOveredFrame != pOveredFrame_)
            {
                if (pOveredFrame_)
                    pOveredFrame_->NotifyMouseInFrame(false, iX, iY);

                pOveredFrame_ = pOveredFrame;
            }

            if (pOveredFrame_)
                pOveredFrame_->NotifyMouseInFrame(true, iX, iY);
        }

        bObjectMoved_ = false;
        bBuildStrataList_ = false;

        static s_bool bFirstIteration = true;
        if (bFirstIteration)
        {
            EventManager::GetSingleton()->FireEvent(Event("ENTERING_WORLD"));
            bFirstIteration = false;
        }

        ++uiFrameNumber_;
    }

    void GUIManager::StartMoving( s_ptr<GUI::UIObject> pObj, s_ptr<GUI::Anchor> pAnchor, Vector::Constraint mConstraint )
    {
        pSizedObject_ = nullptr;
        pMovedObject_ = pObj;
        fMouseMovementX_ = 0.0f;
        fMouseMovementY_ = 0.0f;

        if (pMovedObject_)
        {
            mConstraint_ = mConstraint;
            if (pAnchor)
            {
                pMovedAnchor_ = pAnchor;
                iMovementStartPositionX_ = pMovedAnchor_->GetAbsOffsetX();
                iMovementStartPositionY_ = pMovedAnchor_->GetAbsOffsetY();
            }
            else
            {
                pMovedObject_->ClearAllPoints();
                const s_array<s_int,4>& lBorders = pMovedObject_->GetBorders();
                pMovedObject_->SetAbsPoint(
                    GUI::ANCHOR_TOPLEFT, "", GUI::ANCHOR_TOPLEFT,
                    lBorders[GUI::BORDER_LEFT], lBorders[GUI::BORDER_TOP]
                );
                pMovedAnchor_ = pMovedObject_->ModifyPoint(GUI::ANCHOR_TOPLEFT);

                iMovementStartPositionX_ = lBorders[GUI::BORDER_LEFT];
                iMovementStartPositionY_ = lBorders[GUI::BORDER_TOP];
            }
        }
    }

    void GUIManager::StopMoving( s_ptr<GUI::UIObject> pObj )
    {
        if (pMovedObject_ == pObj)
        {
            pMovedObject_ = nullptr;
            pMovedAnchor_ = nullptr;
        }
    }

    s_bool GUIManager::IsMoving( s_ptr<GUI::UIObject> pObj ) const
    {
        return (pMovedObject_ == pObj);
    }

    void GUIManager::StartSizing( s_ptr<GUI::UIObject> pObj, GUI::AnchorPoint mPoint )
    {
        pMovedObject_  = nullptr;
        pSizedObject_ = pObj;
        fMouseMovementX_ = 0.0f;
        fMouseMovementY_ = 0.0f;

        if (pSizedObject_)
        {
            const s_array<s_int,4>& lBorders = pSizedObject_->GetBorders();

            GUI::AnchorPoint mOppositePoint = GUI::ANCHOR_CENTER;
            s_int iOffX;
            s_int iOffY;
            switch (mPoint)
            {
                case GUI::ANCHOR_TOPLEFT :
                case GUI::ANCHOR_TOP :
                    mOppositePoint = GUI::ANCHOR_BOTTOMRIGHT;
                    iOffX = lBorders[GUI::BORDER_RIGHT];
                    iOffY = lBorders[GUI::BORDER_BOTTOM];
                    bResizeFromRight_ = false;
                    bResizeFromBottom_ = false;
                    break;
                case GUI::ANCHOR_TOPRIGHT :
                case GUI::ANCHOR_RIGHT :
                    mOppositePoint = GUI::ANCHOR_BOTTOMLEFT;
                    iOffX = lBorders[GUI::BORDER_LEFT];
                    iOffY = lBorders[GUI::BORDER_BOTTOM];
                    bResizeFromRight_ = true;
                    bResizeFromBottom_ = false;
                    break;
                case GUI::ANCHOR_BOTTOMRIGHT :
                case GUI::ANCHOR_BOTTOM :
                    mOppositePoint = GUI::ANCHOR_TOPLEFT;
                    iOffX = lBorders[GUI::BORDER_LEFT];
                    iOffY = lBorders[GUI::BORDER_TOP];
                    bResizeFromRight_ = true;
                    bResizeFromBottom_ = true;
                    break;
                case GUI::ANCHOR_BOTTOMLEFT :
                case GUI::ANCHOR_LEFT :
                    mOppositePoint = GUI::ANCHOR_TOPRIGHT;
                    iOffX = lBorders[GUI::BORDER_RIGHT];
                    iOffY = lBorders[GUI::BORDER_TOP];
                    bResizeFromRight_ = false;
                    bResizeFromBottom_ = true;
                    break;
                case GUI::ANCHOR_CENTER :
                    Error(CLASS_NAME,
                        "Can't resize \""+pObj->GetName()+"\" from its center."
                    );
                    pSizedObject_ = nullptr;
                    return;
            }

            pSizedObject_->ClearAllPoints();
            pSizedObject_->SetAbsPoint(mOppositePoint, "", GUI::ANCHOR_TOPLEFT, iOffX, iOffY);

            uiResizeStartW_ = pSizedObject_->GetAbsWidth();
            uiResizeStartH_ = pSizedObject_->GetAbsHeight();

            if (mPoint == GUI::ANCHOR_LEFT || mPoint == GUI::ANCHOR_RIGHT)
            {
                bResizeWidth_ = true;
                bResizeHeight_ = false;
            }
            else if (mPoint == GUI::ANCHOR_TOP || mPoint == GUI::ANCHOR_BOTTOM)
            {
                bResizeWidth_ = false;
                bResizeHeight_ = true;
            }
            else
            {
                bResizeWidth_ = true;
                bResizeHeight_ = true;
            }
        }
    }

    void GUIManager::StopSizing( s_ptr<GUI::UIObject> pObj )
    {
        if (pSizedObject_ == pObj)
            pSizedObject_ = nullptr;
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

    void GUIManager::NotifyObjectMoved()
    {
        bObjectMoved_ = true;
    }

    void GUIManager::FireRedraw( FrameStrata mStrata ) const
    {
        s_map<FrameStrata, Strata>::const_iterator iter = lStrataList_.Get(mStrata);
        if (iter != lStrataList_.End())
            iter->second.bRedraw = true;
    }

    void GUIManager::ToggleCaching()
    {
        bEnableCaching_ = !bEnableCaching_;

        if (bEnableCaching_)
        {
            s_map<FrameStrata, Strata>::iterator iterStrata;
            foreach (iterStrata, lStrataList_)
            {
                iterStrata->second.bRedraw = true;
            }
        }
    }

    void GUIManager::EnableCaching( const s_bool& bEnable )
    {
        if (bEnableCaching_ != bEnable)
            ToggleCaching();
    }

    const s_bool& GUIManager::IsCachingEnabled() const
    {
        return bEnableCaching_;
    }

    void GUIManager::ClearFontsOnClose( const s_bool& bClear )
    {
        bClearFontsOnClose_ = bClear;
    }

    s_ptr<const GUI::Frame> GUIManager::GetOveredFrame() const
    {
        return pOveredFrame_;
    }

    void GUIManager::RequestFocus( s_ptr<GUI::EditBox> pEditBox )
    {
        if (pFocusedEditBox_)
            pFocusedEditBox_->NotifyFocus(false);

        pFocusedEditBox_ = pEditBox;

        if (pFocusedEditBox_)
        {
            pFocusedEditBox_->NotifyFocus(true);
            InputManager::GetSingleton()->SetFocus(true, pFocusedEditBox_);
        }
        else
            InputManager::GetSingleton()->SetFocus(false);
    }

    void GUIManager::SetKeyBinding( const s_uint& uiKey, const s_str& sLuaString )
    {
        lKeyBindingList_[uiKey][KEY_UNASSIGNED][KEY_UNASSIGNED] = sLuaString;
    }

    void GUIManager::SetKeyBinding( const s_uint& uiKey, const s_uint& uiModifier, const s_str& sLuaString )
    {
        lKeyBindingList_[uiKey][uiModifier][KEY_UNASSIGNED] = sLuaString;
    }

    void GUIManager::SetKeyBinding( const s_uint& uiKey, const s_uint& uiModifier1, const s_uint& uiModifier2, const s_str& sLuaString )
    {
        lKeyBindingList_[uiKey][uiModifier1][uiModifier2] = sLuaString;
    }

    void GUIManager::RemoveKeyBinding( const s_uint& uiKey )
    {
        s_map< s_uint, s_map< s_uint, s_map<s_uint, s_str> > >::iterator iter1 = lKeyBindingList_.Get(uiKey);
        if (iter1 != lKeyBindingList_.End())
        {
            s_map< s_uint, s_map<s_uint, s_str> >::iterator iter2 = iter1->second.Get(KEY_UNASSIGNED);
            if (iter2 != iter1->second.End())
            {
                s_map<s_uint, s_str>::iterator iter3 = iter2->second.Get(KEY_UNASSIGNED);
                if (iter3 != iter2->second.End())
                {
                    iter2->second.Erase(iter3);

                    if (iter2->second.GetSize() == 0)
                        iter1->second.Erase(iter2);

                    if (iter1->second.GetSize() == 0)
                        lKeyBindingList_.Erase(iter1);
                }
            }
        }
    }

    void GUIManager::RemoveKeyBinding( const s_uint& uiKey, const s_uint& uiModifier )
    {
        s_map< s_uint, s_map< s_uint, s_map<s_uint, s_str> > >::iterator iter1 = lKeyBindingList_.Get(uiKey);
        if (iter1 != lKeyBindingList_.End())
        {
            s_map< s_uint, s_map<s_uint, s_str> >::iterator iter2 = iter1->second.Get(uiModifier);
            if (iter2 != iter1->second.End())
            {
                s_map<s_uint, s_str>::iterator iter3 = iter2->second.Get(KEY_UNASSIGNED);
                if (iter3 != iter2->second.End())
                {
                    iter2->second.Erase(iter3);

                    if (iter2->second.GetSize() == 0)
                        iter1->second.Erase(iter2);

                    if (iter1->second.GetSize() == 0)
                        lKeyBindingList_.Erase(iter1);
                }
            }
        }
    }

    void GUIManager::RemoveKeyBinding( const s_uint& uiKey, const s_uint& uiModifier1, const s_uint& uiModifier2 )
    {
        s_map< s_uint, s_map< s_uint, s_map<s_uint, s_str> > >::iterator iter1 = lKeyBindingList_.Get(uiKey);
        if (iter1 != lKeyBindingList_.End())
        {
            s_map< s_uint, s_map<s_uint, s_str> >::iterator iter2 = iter1->second.Get(uiModifier1);
            if (iter2 != iter1->second.End())
            {
                s_map<s_uint, s_str>::iterator iter3 = iter2->second.Get(uiModifier2);
                if (iter3 != iter2->second.End())
                {
                    iter2->second.Erase(iter3);

                    if (iter2->second.GetSize() == 0)
                        iter1->second.Erase(iter2);

                    if (iter1->second.GetSize() == 0)
                        lKeyBindingList_.Erase(iter1);
                }
            }
        }
    }

    s_int GUIManager::GetHighestLevel( FrameStrata mFrameStrata ) const
    {
        s_map<FrameStrata, Strata>::const_iterator iterStrata = lStrataList_.Get(mFrameStrata);
        if (iterStrata != lStrataList_.End())
        {
            if (!iterStrata->second.lLevelList.IsEmpty())
            {
                s_map<s_int, Level>::const_iterator iterLevel = iterStrata->second.lLevelList.End();
                --iterLevel;
                return iterLevel->first;
            }
        }

        return 0;
    }

    void GUIManager::ThisStackPush(s_ptr<GUI::Frame> pFrame)
    {
        pLua_->GetGlobal("_this_stack");
        pLua_->GetGlobal("this");
        pLua_->SetField(iThisStackSize_);
        pLua_->Pop();

        pFrame->PushOnLua(pLua_);
        pLua_->SetGlobal("this");

        ++iThisStackSize_;
    }

    void GUIManager::ThisStackPop()
    {
        if (iThisStackSize_ != 0)
        {
            --iThisStackSize_;

            pLua_->GetGlobal("_this_stack");
            pLua_->GetField(iThisStackSize_);
            pLua_->SetGlobal("this");
            pLua_->PushNil();
            pLua_->SetField(iThisStackSize_);
            pLua_->Pop();
        }
    }

    void GUIManager::ParseXMLFile_( const s_str& sFile, s_ptr<AddOn> pAddOn )
    {
        XML::Document mDoc(sFile, "Interface/UI.def");
        if (mDoc.Check())
        {
            s_ptr<XML::Block> pElemBlock;
            foreach_block (pElemBlock, mDoc.GetMainBlock())
            {
                if (pElemBlock->GetName() == "Script")
                {
                    try
                    {
                        pLua_->DoFile(pAddOn->sDirectory + "/" + pElemBlock->GetAttribute("file"));
                    }
                    catch (LuaException& e)
                    {
                        s_str sError = e.GetDescription();

                        Error("", sError);

                        Event mEvent("LUA_ERROR");
                        mEvent.Add(sError);
                        EventManager::GetSingleton()->FireEvent(mEvent);
                    }
                }
                else if (pElemBlock->GetName() == "Include")
                {
                    this->ParseXMLFile_(pAddOn->sDirectory + "/" + pElemBlock->GetAttribute("file"), pAddOn);
                }
                else
                {
                    s_ptr<GUI::UIObject> pUIObject = CreateUIObject(pElemBlock->GetName());

                    try
                    {
                        s_ptr<GUI::Frame> pFrame = s_ptr<GUI::Frame>::DynamicCast(pUIObject);

                        if (pFrame)
                        {
                            pFrame->SetAddOn(GetCurrentAddOn());

                            pFrame->ParseBlock(pElemBlock);

                            if (!pFrame->IsVirtual())
                                pFrame->On("Load");

                            pFrame->NotifyLoaded();
                        }
                        else
                        {
                            // TODO : Allow virtual regions to be created at root
                            Warning(CLASS_NAME, "Creating Texture or FontString at root level is forbidden. Skipped.");
                            pUIObject.Delete();
                        }
                    }
                    catch (const GUIException& e)
                    {
                        pUIObject.Delete();
                        Error("", e.GetDescription());
                    }
                    catch (...)
                    {
                        pUIObject.Delete();
                        throw;
                    }
                }
            }
        }
    }

    s_ptr<AddOn> GUIManager::GetCurrentAddOn()
    {
        return pCurrentAddOn_;
    }

    s_str GUIManager::ParseFileName( const s_str& sFileName ) const
    {
        s_str sNewFile = sFileName;
        if (sNewFile.StartsWith("|"))
        {
            sNewFile[0] = '/';
            if (pCurrentAddOn_)
                sNewFile = pCurrentAddOn_->sDirectory + sNewFile;
        }
        return sNewFile;
    }

    void GUIManager::OnEvent( const Event& mEvent )
    {
        if (mEvent.GetName() == "KEY_PRESSED")
        {
            s_uint uiKey = mEvent.Get(0)->Get<s_uint>();
            s_ptr<InputManager> pInputMgr = InputManager::GetSingleton();
            s_bool bCaptured;

            s_map< s_uint, s_map< s_uint, s_map<s_uint, s_str> > >::const_iterator iter1 = lKeyBindingList_.Get(uiKey);
            s_map< s_uint, s_map<s_uint, s_str> >::const_iterator iter2;
            s_map<s_uint, s_str>::const_iterator iter3;

            if (iter1 != lKeyBindingList_.End())
            {
                foreach (iter2, iter1->second)
                {
                    if (iter2->first == KEY_UNASSIGNED || !pInputMgr->KeyIsDown((KeyCode)iter2->first.Get()))
                        continue;

                    foreach (iter3, iter2->second)
                    {
                        if (iter3->first == KEY_UNASSIGNED || !pInputMgr->KeyIsDown((KeyCode)iter3->first.Get()))
                            continue;

                        try
                        {
                            pLua_->DoString(iter3->second);
                        }
                        catch (LuaException& e)
                        {
                            Error("Binded action : "+pInputMgr->GetKeyName(
                                    (KeyCode)uiKey.Get(), (KeyCode)iter2->first.Get(), (KeyCode)iter3->first.Get()
                                ), e.GetDescription()
                            );
                        }

                        bCaptured = true;
                        break;
                    }

                    if (bCaptured) break;

                    iter3 = iter2->second.Get(KEY_UNASSIGNED);
                    if (iter3 != iter2->second.End())
                    {
                        try
                        {
                            pLua_->DoString(iter3->second);
                        }
                        catch (LuaException& e)
                        {
                            Error("Binded action : "+pInputMgr->GetKeyName(
                                    (KeyCode)uiKey.Get(), (KeyCode)iter2->first.Get()
                                ), e.GetDescription()
                            );
                        }
                    }
                }

                iter2 = iter1->second.Get(KEY_UNASSIGNED);
                if (iter2 != iter1->second.End())
                {
                    iter3 = iter2->second.Get(KEY_UNASSIGNED);
                    if (iter3 != iter2->second.End())
                    {
                        try
                        {
                            pLua_->DoString(iter3->second);
                        }
                        catch (LuaException& e)
                        {
                            Error("Binded action : "+pInputMgr->GetKeyName((KeyCode)uiKey.Get()),
                                e.GetDescription()
                            );
                        }
                    }
                }
            }
        }
    }

    void GUIManager::Begin( s_ptr<GUI::RenderTarget> pTarget )
    {
        pImpl_->Begin(pTarget);
    }

    void GUIManager::End()
    {
        pImpl_->End();
    }

    void GUIManager::PrintStatistics()
    {
        Log("GUI Statistics :");
        Log("    Strata redraw percent :");
        s_map<FrameStrata, Strata>::const_iterator iterStrata;
        foreach (iterStrata, lStrataList_)
        {
            const Strata& mStrata = iterStrata->second;
            Log("     - ["+mStrata.uiID+"] : "+s_str::Convert(100.0f*s_float(mStrata.uiRedrawCount)/s_float(uiFrameNumber_), 2, 1)+"%");
        }
    }

    void GUIManager::PrintUI() const
    {
        if (lAddOnList_.GetSize() >= 1)
        {
            Log("\n\n######################## Loaded AddOns ########################\n");
            s_map< s_str, s_map<s_str, AddOn> >::const_iterator iterDirectory;
            foreach (iterDirectory, lAddOnList_)
            {
                Log("# Directory : "+iterDirectory->first+"\n|-###");
                s_map<s_str, AddOn>::const_iterator iterAdd;
                foreach (iterAdd, iterDirectory->second)
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
            s_map< s_uint, s_ptr<GUI::UIObject> >::const_iterator iterObj;
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

    LuaGUIManager::LuaGUIManager( lua_State* pLua )
    {
        lua_newtable(pLua);
        iRef_ = luaL_ref(pLua, LUA_REGISTRYINDEX);
        pLua_ = pLua;
    }

    LuaGUIManager::~LuaGUIManager()
    {
        luaL_unref(pLua_, LUA_REGISTRYINDEX, iRef_);
    }

    void LuaGUIManager::SetManager( s_ptr<GUIManager> pMgr )
    {
        pMgr_ = pMgr;
    }

    s_ptr<GUIManager> LuaGUIManager::GetManager()
    {
        return pMgr_;
    }

    int LuaGUIManager::GetDataTable( lua_State* pLua )
    {
        lua_getref(pLua, iRef_);
        return 1;
    }

    const char LuaGUIManager::className[] = "GUIManager";
    const char* LuaGUIManager::classList[] = {"GUIManager", 0};
    Lunar<LuaGUIManager>::RegType LuaGUIManager::methods[] = {
        {"dt", &LuaGUIManager::GetDataTable},
        {0,0}
    };

    int l_SetKeyBinding( lua_State* pLua )
    {
        Lua::Function mFunc("SetKeyBinding", pLua);
        mFunc.Add(0, "key", Lua::TYPE_NUMBER);
        mFunc.Add(1, "code", Lua::TYPE_STRING);
        mFunc.Add(1, "nil", Lua::TYPE_NIL);
        mFunc.NewParamSet();
        mFunc.Add(0, "key", Lua::TYPE_NUMBER);
        mFunc.Add(1, "modifier", Lua::TYPE_NUMBER);
        mFunc.Add(2, "code", Lua::TYPE_STRING);
        mFunc.Add(2, "nil", Lua::TYPE_NIL);
        mFunc.NewParamSet();
        mFunc.Add(0, "key", Lua::TYPE_NUMBER);
        mFunc.Add(1, "modifier1", Lua::TYPE_NUMBER);
        mFunc.Add(2, "modifier2", Lua::TYPE_NUMBER);
        mFunc.Add(3, "code", Lua::TYPE_STRING);
        mFunc.Add(3, "nil", Lua::TYPE_NIL);

        if (mFunc.Check())
        {
            s_ptr<Lua::State> pState = mFunc.GetState();
            pState->GetGlobal("_MGR");
            s_ptr<GUIManager> pGUIMgr = pState->Get<LuaGUIManager>()->GetManager();
            pState->Pop();

            s_uint uiKey = s_uint(mFunc.Get(0)->GetNumber());

            if (mFunc.GetParamSetRank() == 0)
            {
                if (mFunc.IsProvided(1) && mFunc.Get(1)->GetType() == Lua::TYPE_STRING)
                    pGUIMgr->SetKeyBinding(uiKey, mFunc.Get(1)->GetString());
                else
                    pGUIMgr->RemoveKeyBinding(uiKey);
            }
            else if (mFunc.GetParamSetRank() == 1)
            {
                s_uint uiModifier = s_uint(mFunc.Get(1)->GetNumber());

                if (mFunc.IsProvided(2) && mFunc.Get(2)->GetType() == Lua::TYPE_STRING)
                    pGUIMgr->SetKeyBinding(uiKey, uiModifier, mFunc.Get(2)->GetString());
                else
                    pGUIMgr->RemoveKeyBinding(uiKey, uiModifier);
            }
            else
            {
                s_uint uiModifier1 = s_uint(mFunc.Get(1)->GetNumber());
                s_uint uiModifier2 = s_uint(mFunc.Get(2)->GetNumber());

                if (mFunc.IsProvided(3) && mFunc.Get(3)->GetType() == Lua::TYPE_STRING)
                    pGUIMgr->SetKeyBinding(uiKey, uiModifier1, uiModifier2, mFunc.Get(3)->GetString());
                else
                    pGUIMgr->RemoveKeyBinding(uiKey, uiModifier1, uiModifier2);
            }
        }

        return mFunc.Return();
    }

    int l_CreateFrame( lua_State* pLua )
    {
        Lua::Function mFunc("CreateFrame", pLua, 1);
        mFunc.Add(0, "type", Lua::TYPE_STRING);
        mFunc.Add(1, "name", Lua::TYPE_STRING);
        mFunc.Add(2, "parent", Lua::TYPE_USERDATA, true);
        mFunc.Add(2, "parent", Lua::TYPE_NIL, true);
        mFunc.Add(3, "inherits", Lua::TYPE_STRING, true);

        if (mFunc.Check())
        {
            s_str sType = mFunc.Get(0)->GetString();
            s_str sName = mFunc.Get(1)->GetString();

            s_ptr<Lua::State> pState = mFunc.GetState();
            pState->GetGlobal("_MGR");
            s_ptr<GUIManager> pGUIMgr = pState->Get<LuaGUIManager>()->GetManager();
            pState->Pop();

            s_ptr<GUI::Frame> pParent;
            if (mFunc.IsProvided(2) && mFunc.Get(2)->GetType() == Lua::TYPE_USERDATA)
            {
                s_ptr<GUI::LuaUIObject> pObj = mFunc.Get(2)->Get<GUI::LuaUIObject>();
                if (pObj)
                {
                    pParent = s_ptr<GUI::Frame>::DynamicCast(pObj->GetParent());
                    if (!pParent)
                    {
                        Warning(mFunc.GetName(),
                            "The second argument of "+mFunc.GetName()+" must be a Frame "
                            "(got a "+pObj->GetParent()->GetObjectType()+")."
                        );
                    }
                }
            }

            s_str sInheritance;
            if (mFunc.Get(3)->IsProvided())
                sInheritance = mFunc.Get(3)->GetString();

            s_ptr<GUI::Frame> pNewFrame = pGUIMgr->CreateFrame(
                sType, sName, pParent, sInheritance
            );

            if (pNewFrame)
            {
                pNewFrame->PushOnLua(pState);
                mFunc.NotifyPushed();
            }
            else
                mFunc.PushNil();
        }

        return mFunc.Return();
    }

    int l_DeleteFrame( lua_State* pLua )
    {
        Lua::Function mFunc("DeleteFrame", pLua);
        mFunc.Add(0, "frame", Lua::TYPE_USERDATA);

        if (mFunc.Check())
        {
            s_ptr<GUI::LuaUIObject> pLuaObj = mFunc.Get(0)->Get<GUI::LuaUIObject>();
            if (pLuaObj)
            {
                s_ptr<GUI::Frame> pFrame = s_ptr<GUI::Frame>::DynamicCast(pLuaObj->GetParent());
                if (pFrame)
                {
                    s_ctnr< s_ptr<GUI::UIObject> > lList = pFrame->ClearLinks();

                    s_ptr<Lua::State> pState = mFunc.GetState();
                    pState->GetGlobal("_MGR");
                    s_ptr<GUIManager> pGUIMgr = pState->Get<LuaGUIManager>()->GetManager();
                    pState->Pop();

                    s_ctnr< s_ptr<GUI::UIObject> >::iterator iterObject;
                    foreach (iterObject, lList)
                    {
                        s_ptr<GUI::UIObject> pObj = *iterObject;
                        pGUIMgr->RemoveUIObject(pObj);
                        pObj.Delete();
                    }
                }
                else
                {
                    Error(mFunc.GetName(), "Argument 1 must be a Frame.");
                    return mFunc.Return();
                }
            }
            else
            {
                Error(mFunc.GetName(), "Argument 1 must be a Frame.");
                return mFunc.Return();
            }
        }

        return mFunc.Return();
    }

    int l_GetLocale( lua_State* pLua )
    {
        Lua::Function mFunc("GetLocale", pLua, 1);

        mFunc.Push(LocaleManager::GetSingleton()->GetLocale());

        return mFunc.Return();
    }

    int l_Log( lua_State* pLua )
    {
        Lua::Function mFunc("Log", pLua, 1);
        mFunc.Add(0, "string", Lua::TYPE_STRING);
        mFunc.Add(0, "number", Lua::TYPE_NUMBER);
        mFunc.Add(0, "bool", Lua::TYPE_BOOLEAN);

        if (mFunc.Check(false))
        {
            s_ptr<Lua::Argument> pArg = mFunc.Get(0);
            if (pArg->GetType() == Lua::TYPE_STRING)
                Log(pArg->GetString());
            else if (pArg->GetType() == Lua::TYPE_NUMBER)
                Log(pArg->GetNumber());
            else if (pArg->GetType() == Lua::TYPE_BOOLEAN)
                Log(pArg->GetBool());
        }
        else
            Log("");

        return mFunc.Return();
    }

    int l_Exit( lua_State* pLua )
    {
        Lua::Function mFunc("Exit", pLua);

        Shutdown();

        return mFunc.Return();
    }
}
