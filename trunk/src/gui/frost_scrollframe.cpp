/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_scrollframe.h"

#include "gui/frost_frame.h"
#include "gui/frost_texture.h"
#include "gui/frost_guimanager.h"
#include "gui/frost_spritemanager.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str ScrollFrame::CLASS_NAME = "GUI::ScrollFrame";

ScrollFrame::ScrollFrame() : Frame()
{
    lType_.PushBack("ScrollFrame");
}

ScrollFrame::~ScrollFrame()
{
    SpriteManager::GetSingleton()->DeleteRenderTarget(pScrollRenderTarget_);
}

s_bool ScrollFrame::CanUseScript( const s_str& sScriptName ) const
{
    if (Frame::CanUseScript(sScriptName))
        return true;
    else if ((sScriptName == "OnHorizontalScroll") ||
        (sScriptName == "OnScrollRangeChanged") ||
        (sScriptName == "OnVerticalScroll"))
        return true;
    else
        return false;
}

void ScrollFrame::On( const s_str& sScriptName, s_ptr<Event> pEvent )
{
    Frame::On(sScriptName, pEvent);

    if (sScriptName == "SizeChanged")
        bUpdateScrollRenderTarget_ = true;
}

void ScrollFrame::CopyFrom( s_ptr<UIObject> pObj )
{
    Frame::CopyFrom(pObj);

    s_ptr<ScrollFrame> pScrollFrame = s_ptr<ScrollFrame>::DynamicCast(pObj);

    if (pScrollFrame)
    {
        this->SetHorizontalScroll(pScrollFrame->GetHorizontalScroll());
        this->SetVerticalScroll(pScrollFrame->GetVerticalScroll());

        if (pScrollFrame->GetScrollChild())
        {
            pScrollChild_ = GUIManager::GetSingleton()->CreateFrame(pScrollFrame->GetScrollChild()->GetObjectType());
            if (pScrollChild_)
            {
                pScrollChild_->SetManuallyRendered(true);
                pScrollChild_->SetParent(this);
                if (this->IsVirtual())
                    pScrollChild_->SetVirtual();
                pScrollChild_->SetName(pScrollFrame->GetScrollChild()->GetRawName());
                if (!GUIManager::GetSingleton()->AddUIObject(pScrollChild_))
                {
                    Warning(lType_.Back(),
                        "Trying to add \""+pScrollChild_->GetName()+"\" to \""+sName_+"\", but its name was already taken : \""
                        +pScrollChild_->GetName()+"\". Skipped."
                    );
                    pScrollChild_.Delete();
                }
                else
                {
                    pScrollChild_->CreateGlue();
                    this->AddChild(pScrollChild_);
                    pScrollChild_->CopyFrom(pScrollFrame->GetScrollChild());
                    if (!this->IsVirtual())
                        pScrollChild_->On("Load");
                }
            }
        }
    }
}

void ScrollFrame::SetScrollChild( s_ptr<Frame> pFrame )
{
    if (pScrollChild_)
    {
        pScrollChild_->SetManuallyRendered(false);
        pScrollChild_->GetPoint(ANCHOR_TOPLEFT)->SetAbsOffset(
            lBorderList_[BORDER_LEFT] - iHorizontalScroll_,
            lBorderList_[BORDER_TOP]  - iVerticalScroll_
        );
    }
    else if (!IsVirtual() && !pScrollTexture_)
    {
        // Create the scroll texture
        pScrollTexture_ = new Texture();
        pScrollTexture_->SetSpecial();
        pScrollTexture_->SetParent(this);
        pScrollTexture_->SetDrawLayer("ARTWORK");
        pScrollTexture_->SetName("$parentScrollTexture");

        if (!GUIManager::GetSingleton()->AddUIObject(pScrollTexture_))
        {
            Warning(lType_.Back(),
                "Trying to create scroll texture for \""+sName_+"\",\n"
                "but its name was already taken : \""+pScrollTexture_->GetName()+"\". Skipped."
            );
            pScrollTexture_.Delete();
            return;
        }

        pScrollTexture_->CreateGlue();
        AddRegion(pScrollTexture_);

        pScrollTexture_->SetAllPoints(this);

        bUpdateScrollRenderTarget_ = true;
    }

    pScrollChild_ = pFrame;

    if (pScrollChild_)
    {
        pScrollChild_->SetManuallyRendered(true);
        pScrollChild_->ClearAllPoints();
        if (pScrollChild_->GetParent() != this)
        {
            pScrollChild_->SetParent(this);
            AddChild(pScrollChild_);
        }
        pScrollChild_->SetAbsPoint(ANCHOR_TOPLEFT, "", ANCHOR_TOPLEFT, -iHorizontalScroll_, -iVerticalScroll_);

        iHorizontalScrollRange_ = s_int(pScrollChild_->GetAbsWidth()) - s_int(uiAbsWidth_);
        if (iHorizontalScrollRange_ < 0) iHorizontalScrollRange_ = 0;

        iVerticalScrollRange_ = s_int(pScrollChild_->GetAbsHeight()) - s_int(uiAbsHeight_);
        if (iVerticalScrollRange_ < 0) iVerticalScrollRange_ = 0;

        On("ScrollRangeChanged");

        bUpdateScrollRange_ = false;
    }
}

s_ptr<Frame> ScrollFrame::GetScrollChild()
{
    return pScrollChild_;
}

void ScrollFrame::SetHorizontalScroll( const s_int& iHorizontalScroll )
{
    iHorizontalScroll_ = iHorizontalScroll;
    lQueuedEventList_.PushBack("HorizontalScroll");

    pScrollChild_->GetPoint(ANCHOR_TOPLEFT)->SetAbsOffset(-iHorizontalScroll_, -iVerticalScroll_);
}

const s_int& ScrollFrame::GetHorizontalScroll() const
{
    return iHorizontalScroll_;
}

const s_int& ScrollFrame::GetHorizontalScrollRange() const
{
    return iHorizontalScrollRange_;
}

void ScrollFrame::SetVerticalScroll( const s_int& iVerticalScroll )
{
    iVerticalScroll_ = iVerticalScroll;
    lQueuedEventList_.PushBack("VerticalScroll");

    pScrollChild_->GetPoint(ANCHOR_TOPLEFT)->SetAbsOffset(-iHorizontalScroll_, -iVerticalScroll_);
}

const s_int& ScrollFrame::GetVerticalScroll() const
{
    return iVerticalScroll_;
}

const s_int& ScrollFrame::GetVerticalScrollRange() const
{
    return iVerticalScrollRange_;
}

void ScrollFrame::Update()
{
    // TODO : ## voir pourquoi seul 1 px de la RT est rendu
    s_uint uiOldChildWidth;
    s_uint uiOldChildHeight;

    if (pScrollChild_)
    {
        uiOldChildWidth = pScrollChild_->GetAbsWidth();
        uiOldChildHeight = pScrollChild_->GetAbsHeight();
    }

    Frame::Update();

    if ( pScrollChild_ && (uiOldChildWidth != pScrollChild_->GetAbsWidth() || uiOldChildHeight != pScrollChild_->GetAbsHeight()) )
        bUpdateScrollRange_ = true;

    if (bUpdateScrollRenderTarget_ && pScrollTexture_)
    {
        if (!pScrollRenderTarget_)
        {
            pScrollRenderTarget_ = SpriteManager::GetSingleton()->CreateRenderTarget(uiAbsWidth_, uiAbsHeight_);
        }
        else
        {
            pScrollRenderTarget_->SetDimensions(uiAbsWidth_, uiAbsHeight_);
            bUpdateScrollRange_ = true;
        }

        pScrollTexture_->SetTexture(pScrollRenderTarget_);

        bUpdateScrollRenderTarget_ = false;
    }

    if (bUpdateScrollRange_)
    {
        iHorizontalScrollRange_ = s_int(pScrollChild_->GetAbsWidth()) - s_int(uiAbsWidth_);
        if (iHorizontalScrollRange_ < 0) iHorizontalScrollRange_ = 0;

        iVerticalScrollRange_ = s_int(pScrollChild_->GetAbsHeight()) - s_int(uiAbsHeight_);
        if (iVerticalScrollRange_ < 0) iVerticalScrollRange_ = 0;

        On("ScrollRangeChanged");

        bUpdateScrollRange_ = false;
    }

    if (pScrollChild_ && pScrollRenderTarget_)
    {
        s_ptr<SpriteManager> pSpriteMgr = SpriteManager::GetSingleton();

        pSpriteMgr->Begin(pScrollRenderTarget_);
        pSpriteMgr->Clear(Color::VOID);

        pScrollChild_->Render();

        pSpriteMgr->End();
    }
}

void ScrollFrame::CreateGlue()
{
    if (bVirtual_)
    {
        s_ptr<Lua::State> pLua = GUIManager::GetSingleton()->GetLua();
        pLua->PushNumber(uiID_);
        lGlueList_.PushBack(
            pLua->Push<LuaVirtualGlue>(new LuaVirtualGlue(pLua->GetState()))
        );
        pLua->SetGlobal(sLuaName_);
    }
    else
    {
        s_ptr<Lua::State> pLua = GUIManager::GetSingleton()->GetLua();
        pLua->PushString(sLuaName_);
        lGlueList_.PushBack(
            pLua->Push<LuaScrollFrame>(new LuaScrollFrame(pLua->GetState()))
        );
        pLua->SetGlobal(sLuaName_);
    }
}
