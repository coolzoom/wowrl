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

#include "gui/frost_anchor.h"
#include "gui/frost_uiobject.h"
#include "gui/frost_button.h"
#include "gui/frost_checkbutton.h"
#include "gui/frost_colorselect.h"
#include "gui/frost_cooldown.h"
#include "gui/frost_editbox.h"
#include "gui/frost_messageframe.h"
#include "gui/frost_scrollframe.h"
#include "gui/frost_frame.h"
#include "gui/frost_scrollingmessageframe.h"
#include "gui/frost_slider.h"
#include "gui/frost_statusbar.h"
#include "gui/frost_texture.h"
#include "gui/frost_fontstring.h"
#include "gui/frost_spritemanager.h"
#include "material/frost_materialmanager.h"
#include "lua/frost_lua.h"

#include "frost_inputmanager.h"
#include <xml/frost_xml_document.h>
#include <utils/frost_utils_file.h>
#include <utils/frost_utils_directory.h>

using namespace std;

namespace Frost
{
    const s_str GUIManager::CLASS_NAME = "GUIManager";
    const s_str AddOn::CLASS_NAME = "AddOn";
    const s_str Level::CLASS_NAME  = "GUI::Level";
    const s_str Strata::CLASS_NAME = "GUI::Strata";

    GUIManager::GUIManager()
    {
        Log("Creating "+CLASS_NAME+".");
        RegisterEvent("KEY_PRESSED");
    }

    GUIManager::~GUIManager()
    {
        Log("Closing "+CLASS_NAME+"...");
        CloseUI();

        s_map<FrameStrata, Strata>::iterator iterStrata;
        foreach (iterStrata, lStrataList_)
        {
            SpriteManager::GetSingleton()->DeleteRenderTarget(iterStrata->second.pRenderTarget);
        }
    }

    void GUIManager::Initialize()
    {
        bClosed_ = true;
        bEnableCaching_ = true;
    }

    void GUIManager::ReadConfig()
    {
        s_ptr<Engine> pEngine = Engine::GetSingleton();

        if (pEngine->IsConstantDefined("EnableGUICaching"))
            bEnableCaching_ = pEngine->GetBoolConstant("EnableGUICaching");
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

    s_bool GUIManager::CheckUIObjectName( const s_str& sName )
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
            return new GUI::Frame();
        else if (sClassName == "Button")
            return new GUI::Button();
        else if (sClassName == "CheckButton")
            return new GUI::CheckButton();
        else if (sClassName == "ColorSelect")
            return new GUI::ColorSelect();
        else if (sClassName == "Cooldown")
            return new GUI::Cooldown();
        else if (sClassName == "EditBox")
            return new GUI::EditBox();
        else if (sClassName == "MessageFrame")
            return new GUI::MessageFrame();
        else if (sClassName == "ScrollFrame")
            return new GUI::ScrollFrame();
        else if (sClassName == "ScrollingMessageFrame")
            return new GUI::ScrollingMessageFrame();
        else if (sClassName == "Slider")
            return new GUI::Slider();
        else if (sClassName == "StatusBar")
            return new GUI::StatusBar();
        else if (sClassName == "Texture")
            return new GUI::Texture();
        else if (sClassName == "FontString")
            return new GUI::FontString();
        else
        {
            Warning(CLASS_NAME, "Unkown UIObject class : \""+sClassName+"\".");
            return nullptr;
        }
    }

    s_ptr<GUI::Frame> GUIManager::CreateFrame( const s_str& sClassName )
    {
        if (sClassName == "Frame")
            return new GUI::Frame();
        else if (sClassName == "Button")
            return new GUI::Button();
        else if (sClassName == "CheckButton")
            return new GUI::CheckButton();
        else if (sClassName == "ColorSelect")
            return new GUI::ColorSelect();
        else if (sClassName == "Cooldown")
            return new GUI::Cooldown();
        else if (sClassName == "EditBox")
            return new GUI::EditBox();
        else if (sClassName == "MessageFrame")
            return new GUI::MessageFrame();
        else if (sClassName == "ScrollFrame")
            return new GUI::ScrollFrame();
        else if (sClassName == "ScrollingMessageFrame")
            return new GUI::ScrollingMessageFrame();
        else if (sClassName == "Slider")
            return new GUI::Slider();
        else if (sClassName == "StatusBar")
            return new GUI::StatusBar();
        else
        {
            Warning(CLASS_NAME, "Unkown Frame class : \""+sClassName+"\".");
            return nullptr;
        }
    }

    s_ptr<GUI::LayeredRegion> GUIManager::CreateLayeredRegion( const s_str& sClassName )
    {
        if (sClassName == "Texture")
            return new GUI::Texture();
        else if (sClassName == "FontString")
            return new GUI::FontString();
        else
        {
            Warning(CLASS_NAME, "Unkown LayeredRegion class : \""+sClassName+"\".");
            return nullptr;
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

    s_ptr<GUI::UIObject> GUIManager::GetUIObject( const s_uint& uiID )
    {
        if (lObjectList_.Find(uiID))
            return lObjectList_[uiID];
        else
            return nullptr;
    }

    s_ptr<GUI::UIObject> GUIManager::GetUIObjectByName( const s_str& sName, const s_bool& bVirtual )
    {
        if (bVirtual)
        {
            if (lNamedVirtualObjectList_.Find(sName))
                return lNamedVirtualObjectList_[sName];
            else
                return nullptr;
        }
        else
        {
            if (lNamedObjectList_.Find(sName))
                return lNamedObjectList_[sName];
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

                                s_str sGameVersion = Engine::GetSingleton()->GetStringConstant("GameVersion");
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

        EventManager::GetSingleton()->FireEvent(Event(
            mEvent
        ));
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

    void GUIManager::LoadUI()
    {
        if (bClosed_)
        {
            pLua_ = LuaManager::GetSingleton()->CreateLua();
            Lua::RegisterGUIClasses(pLua_);
            Lua::RegisterGlobalFuncs(pLua_);
            if (Engine::GetSingleton()->GetState() == Engine::STATE_EDITOR)
            {
                Lua::RegisterEngineClass(pLua_);
                Engine::GetSingleton()->CreateGlue(pLua_);
            }

            // Create the "this" stack
            iThisStackSize_ = 0u;
            pLua_->NewTable();
            pLua_->SetGlobal("_this_stack");

            bLoadingUI_ = true;
            s_ctnr<s_str>::iterator iterDirectory;
            foreach (iterDirectory, lGUIDirectoryList_)
            {
                this->LoadAddOnDirectory_(*iterDirectory);
            }

            bLoadingUI_ = false;
            bClosed_ = false;
        }
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
        if (bEnableCaching_)
        {
            s_map<FrameStrata, Strata>::const_iterator iterStrata;
            foreach (iterStrata, lStrataList_)
            {
                if (iterStrata->second.pSprite)
                    iterStrata->second.pSprite->Render(0, 0);
            }
        }
        else
        {
            s_map<FrameStrata, Strata>::iterator iterStrata;
            foreach (iterStrata, lStrataList_)
            {
                Strata& mStrata = iterStrata->second;
                s_map<s_uint, Level>::const_iterator iterLevel;
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

    void GUIManager::CreateStrataRenderTarget_( Strata& mStrata )
    {
        mStrata.pRenderTarget = SpriteManager::GetSingleton()->CreateRenderTarget(
            "StrataTarget_"+mStrata.uiID,
            Engine::GetSingleton()->GetScreenWidth(),
            Engine::GetSingleton()->GetScreenHeight()
        );

        if (!mStrata.pRenderTarget)
        {
            throw Exception(CLASS_NAME,
                "Unable to create RenderTarget for strata : "+mStrata.uiID+"."
            );
        }

        s_refptr<Material> pMat = MaterialManager::GetSingleton()->CreateMaterial2DFromRT(mStrata.pRenderTarget);

        mStrata.pSprite = s_refptr<Sprite>(new Sprite(pMat,
            s_float(mStrata.pRenderTarget->GetWidth()), s_float(mStrata.pRenderTarget->GetHeight())
        ));
    }

    void GUIManager::RenderStrata_( Strata& mStrata )
    {
        s_ptr<SpriteManager> pSpriteMgr = SpriteManager::GetSingleton();

        if (!mStrata.pRenderTarget)
            CreateStrataRenderTarget_(mStrata);

        pSpriteMgr->Begin(mStrata.pRenderTarget);
        pSpriteMgr->Clear(Color::VOID);

        s_map<s_uint, Level>::const_iterator iterLevel;
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

        pSpriteMgr->End();

        ++mStrata.uiRedrawCount;
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
            s_map<FrameStrata, Strata>::iterator iterStrata;
            foreach (iterStrata, lStrataList_)
            {
                Strata& mStrata = iterStrata->second;

                if (mStrata.bRedraw)
                    RenderStrata_(mStrata);

                mStrata.bRedraw = false;
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

                s_map<s_uint, Level>::const_iterator iterLevel = mStrata.lLevelList.End();
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

    void GUIManager::FireRedraw( FrameStrata mStrata )
    {
        lStrataList_[mStrata].bRedraw = true;
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

        Engine::GetSingleton()->SetConstant("EnableGUICaching", bEnableCaching_);
    }

    s_ptr<GUI::Frame> GUIManager::GetOveredFrame() const
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
        lKeyBindingList_[uiKey] = sLuaString;
    }

    void GUIManager::RemoveKeyBinding( const s_uint& uiKey )
    {
        if (lKeyBindingList_.Find(uiKey))
            lKeyBindingList_.Erase(uiKey);
    }

    s_uint GUIManager::GetHighestLevel( FrameStrata mFrameStrata ) const
    {
        s_map<FrameStrata, Strata>::const_iterator iterStrata = lStrataList_.Get(mFrameStrata);
        if (iterStrata != lStrataList_.End())
        {
            if (!iterStrata->second.lLevelList.IsEmpty())
            {
                s_map<s_uint, Level>::const_iterator iterLevel = iterStrata->second.lLevelList.End();
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
                            pFrame->SetAddOn(GUIManager::GetSingleton()->GetCurrentAddOn());

                            pFrame->ParseBlock(pElemBlock);

                            if (!pFrame->IsVirtual())
                                pFrame->On("Load");
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
            s_map<s_uint, s_str>::iterator iter = lKeyBindingList_.Get(uiKey);
            if (iter != lKeyBindingList_.End())
            {
                try
                {
                    pLua_->DoString(iter->second);
                }
                catch (LuaException& e)
                {
                    Error("Binded action : "+InputManager::GetSingleton()->GetKeyName((KeyCode)uiKey.Get()),
                        e.GetDescription()
                    );
                }
            }
        }
    }

    void GUIManager::PrintStatistics()
    {
        Log("GUI Statistics :");
        Log("    Strata redraw percent :");
        s_map<FrameStrata, Strata>::const_iterator iterStrata;
        foreach (iterStrata, lStrataList_)
        {
            const Strata& mStrata = iterStrata->second;
            Log("     - ["+mStrata.uiID+"] : "+s_str(100.0f*s_float(mStrata.uiRedrawCount)/s_float(uiFrameNumber_), 2, 1)+"%");
        }
    }

    void GUIManager::PrintUI()
    {
        if (lAddOnList_.GetSize() >= 1)
        {
            Log("\n\n######################## Loaded AddOns ########################\n");
            s_map< s_str, s_map<s_str, AddOn> >::iterator iterDirectory;
            foreach (iterDirectory, lAddOnList_)
            {
                Log("# Directory : "+iterDirectory->first+"\n|-###");
                s_map<s_str, AddOn>::iterator iterAdd;
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

