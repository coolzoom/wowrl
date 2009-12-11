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
#include "lua/frost_lua.h"

#include "frost_inputmanager.h"
#include "xml/frost_xml_document.h"

using namespace std;

namespace Frost
{
    const s_str GUIManager::CLASS_NAME = "GUIManager";
    const s_str AddOn::CLASS_NAME = "AddOn";
    const s_str Level::CLASS_NAME  = "GUI::Level";
    const s_str Strata::CLASS_NAME = "GUI::Strata";

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

    void GUIManager::AddAddOnFolder( const s_str& sFolder )
    {
        if (!lGUIFolderList_.Find(sFolder))
            lGUIFolderList_.PushBack(sFolder);
    }

    void GUIManager::ClearAddOnFolderList()
    {
        lGUIFolderList_.Clear();
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
                return nullptr;
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
                return nullptr;
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

            File mFile(mAddOn.sFolder + "/" + sAddOnName + ".toc", File::I);

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
        pCurrentAddOn_ = pAddOn;
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

            s_ctnr<s_str>::iterator iterFolder;
            foreach (iterFolder, lGUIFolderList_)
            {
                this->LoadAddOnDirectory_(*iterFolder);
            }

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
        s_map<FrameStrata, Strata>::iterator iterStrata;
        foreach (iterStrata, lStrataList_)
        {
            Strata& mStrata = iterStrata->second;

            s_map<s_uint, Level>::iterator iterLevel;
            foreach (iterLevel, mStrata.lLevelList)
            {
                Level& mLevel = iterLevel->second;

                s_map< s_uint, s_ptr<GUI::Frame> >::iterator iterFrame;
                foreach (iterFrame, mLevel.lFrameList)
                {
                    s_ptr<GUI::Frame> pFrame = iterFrame->second;
                    if ( (pFrame != mLevel.pTopLevel) && (pFrame != mStrata.pTopStrata) )
                    {
                        pFrame->Render();
                    }
                }

                if (mLevel.pTopLevel)
                    mLevel.pTopLevel->Render();
            }

            if (mStrata.pTopStrata)
                mStrata.pTopStrata->Render();
        }
    }

    void GUIManager::FireBuildStrataList()
    {
        bBuildStrataList_ = true;
    }

    s_ptr<GUI::Frame> GUIManager::GetTopStrata( FrameStrata mStrata )
    {
        return lStrataList_[mStrata].pTopStrata;
    }

    s_ptr<GUI::Frame> GUIManager::GetTopLevel( FrameStrata mStrata, const s_uint& uiLevel )
    {
        return lStrataList_[mStrata].lLevelList[uiLevel].pTopLevel;
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
                case CONSTRAINT_NONE :
                    pMovedAnchor_->SetAbsOffset(
                        iMovementStartPositionX_ + s_int(fMouseMovementX_),
                        iMovementStartPositionY_ + s_int(fMouseMovementY_)
                    );
                    break;
                case CONSTRAINT_X :
                    pMovedAnchor_->SetAbsOffset(
                        iMovementStartPositionX_ + s_int(fMouseMovementX_),
                        iMovementStartPositionY_
                    );
                    break;
                case CONSTRAINT_Y :
                    pMovedAnchor_->SetAbsOffset(
                        iMovementStartPositionX_,
                        iMovementStartPositionY_ + s_int(fMouseMovementY_)
                    );
                    break;
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
            iterObj->second->UpdateAnchors();
        }

        // ... then update logics on main widgets from parent to childs.
        foreach (iterObj, lMainObjectList_)
        {
            iterObj->second->Update();
        }

        if (bBuildStrataList_)
        {
            lStrataList_.Clear();

            s_map< s_uint, s_ptr<GUI::Frame> >::iterator iterFrame;
            foreach (iterFrame, lFrameList_)
            {
                s_ptr<GUI::Frame> pFrame = iterFrame->second;
                s_ptr<Strata> pStrata = &lStrataList_[pFrame->GetFrameStrata()];
                s_ptr<Level> pLevel = &pStrata->lLevelList[pFrame->GetFrameLevel()];
                pLevel->lFrameList[pFrame->GetID()] = pFrame;

                if (pFrame->IsTopStrata())
                    pStrata->pTopStrata = pFrame;

                if (pFrame->IsTopLevel())
                    pLevel->pTopLevel = pFrame;
            }
        }

        if ( bBuildStrataList_ || bObjectMoved_ ||
            (InputManager::GetSingleton()->GetMouseRawDX() != 0.0f) ||
            (InputManager::GetSingleton()->GetMouseRawDY() != 0.0f))
        {
            s_int iX = s_int(InputManager::GetSingleton()->GetMousePosX());
            s_int iY = s_int(InputManager::GetSingleton()->GetMousePosY());
            s_ptr<GUI::Frame> pOveredFrame;

            s_map<FrameStrata, Strata>::iterator iterStrata = lStrataList_.End();
            while (iterStrata != lStrataList_.Begin() && !pOveredFrame)
            {
                --iterStrata;
                Strata& mStrata = iterStrata->second;

                if (mStrata.pTopStrata && mStrata.pTopStrata->IsInFrame(iX, iY) && mStrata.pTopStrata->IsVisible())
                {
                    pOveredFrame = mStrata.pTopStrata;
                    break;
                }
                else
                {
                    s_map<s_uint, Level>::iterator iterLevel = mStrata.lLevelList.End();
                    while (iterLevel != mStrata.lLevelList.Begin() && !pOveredFrame)
                    {
                        --iterLevel;
                        Level& mLevel = iterLevel->second;

                        if (mLevel.pTopLevel && mLevel.pTopLevel->IsInFrame(iX, iY) && mLevel.pTopLevel->IsVisible())
                        {
                            pOveredFrame = mLevel.pTopLevel;
                            break;
                        }
                        else
                        {
                            s_map< s_uint, s_ptr<GUI::Frame> >::iterator iterFrame;
                            foreach (iterFrame, mLevel.lFrameList)
                            {
                                s_ptr<GUI::Frame> pFrame = iterFrame->second;
                                if ( (pFrame != mLevel.pTopLevel) && (pFrame != mStrata.pTopStrata) )
                                {
                                    if (pFrame->IsInFrame(iX, iY) && pFrame->IsVisible() && pFrame->IsMouseEnabled())
                                    {
                                        pOveredFrame = pFrame;
                                        break;
                                    }
                                }
                            }

                            if (pOveredFrame)
                                break;
                        }

                        if (pOveredFrame)
                            break;
                    }
                }
            }

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
    }

    void GUIManager::StartMoving( s_ptr<GUI::UIObject> pObj, s_ptr<GUI::Anchor> pAnchor, Constraint mConstraint )
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
                pMovedAnchor_ = pMovedObject_->GetPoint(GUI::ANCHOR_TOPLEFT);

                iMovementStartPositionX_ = lBorders[GUI::BORDER_LEFT];
                iMovementStartPositionY_ = lBorders[GUI::BORDER_TOP];
            }
        }
    }

    void GUIManager::StopMoving( s_ptr<GUI::UIObject> pObj )
    {
        if (pMovedObject_ == pObj)
            pMovedObject_ = nullptr;
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

    s_ptr<GUI::Frame> GUIManager::GetOveredFrame() const
    {
        return pOveredFrame_;
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
                    pLua_->DoFile(pAddOn->sFolder + "/" + pElemBlock->GetAttribute("file"));
                }
                else if (pElemBlock->GetName() == "Include")
                {
                    this->ParseXMLFile_(pAddOn->sFolder + "/" + pElemBlock->GetAttribute("file"), pAddOn);
                }
                else
                {
                    s_ptr<GUI::UIObject> pUIObject;

                    if (pElemBlock->GetName() == "Frame")
                        pUIObject = new GUI::Frame();
                    else if (pElemBlock->GetName() == "Button")
                        pUIObject = new GUI::Button();
                    else if (pElemBlock->GetName() == "CheckButton")
                        pUIObject = new GUI::CheckButton();
                    else if (pElemBlock->GetName() == "ColorSelect")
                        pUIObject = new GUI::ColorSelect();
                    else if (pElemBlock->GetName() == "Cooldown")
                        pUIObject = new GUI::Cooldown();
                    else if (pElemBlock->GetName() == "EditBox")
                        pUIObject = new GUI::EditBox();
                    else if (pElemBlock->GetName() == "MessageFrame")
                        pUIObject = new GUI::MessageFrame();
                    else if (pElemBlock->GetName() == "ScrollFrame")
                        pUIObject = new GUI::ScrollFrame();
                    else if (pElemBlock->GetName() == "ScrollingMessageFrame")
                        pUIObject = new GUI::ScrollingMessageFrame();
                    else if (pElemBlock->GetName() == "Slider")
                        pUIObject = new GUI::Slider();
                    else if (pElemBlock->GetName() == "StatusBar")
                        pUIObject = new GUI::StatusBar();

                    // TODO : Allow virtual regions to be created at root

                    try
                    {
                        pUIObject->ParseBlock(pElemBlock);
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
                sNewFile = pCurrentAddOn_->sFolder + sNewFile;
        }
        return sNewFile;
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

