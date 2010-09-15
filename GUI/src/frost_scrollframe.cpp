/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "frost_scrollframe.h"

#include "frost_frame.h"
#include "frost_texture.h"
#include "frost_guimanager.h"
#include "frost_inputmanager.h"
#include "frost_rendertarget.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str ScrollFrame::CLASS_NAME = "GUI::ScrollFrame";

ScrollFrame::ScrollFrame(s_ptr<GUIManager> pManager) : Frame(pManager)
{
    lType_.PushBack("ScrollFrame");
}

ScrollFrame::~ScrollFrame()
{
    pScrollRenderTarget_.Delete();
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
        bRebuildScrollRenderTarget_ = true;
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
            s_ptr<Frame> pScrollChild = pManager_->CreateFrame(pScrollFrame->GetScrollChild()->GetObjectType());
            if (pScrollChild)
            {
                pScrollChild->SetParent(this);
                if (this->IsVirtual())
                    pScrollChild->SetVirtual();
                pScrollChild->SetName(pScrollFrame->GetScrollChild()->GetRawName());
                if (!pManager_->AddUIObject(pScrollChild))
                {
                    Warning(lType_.Back(),
                        "Trying to add \""+pScrollChild->GetName()+"\" to \""+sName_+"\", but its name was already taken : \""
                        +pScrollChild->GetName()+"\". Skipped."
                    );
                    pScrollChild.Delete();
                }
                else
                {
                    pScrollChild->CreateGlue();
                    this->AddChild(pScrollChild);
                    pScrollChild->CopyFrom(pScrollFrame->GetScrollChild());
                    if (!this->IsVirtual())
                        pScrollChild->On("Load");

                    this->SetScrollChild(pScrollChild);
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
        pScrollChild_->ModifyPoint(ANCHOR_TOPLEFT)->SetAbsOffset(
            lBorderList_[BORDER_LEFT] - iHorizontalScroll_,
            lBorderList_[BORDER_TOP]  - iVerticalScroll_
        );

        lScrollChildList_.Clear();
        lScrollStrataList_.Clear();
    }
    else if (!IsVirtual() && !pScrollTexture_)
    {
        // Create the scroll texture
        pScrollTexture_ = new Texture(pManager_);
        pScrollTexture_->SetSpecial();
        pScrollTexture_->SetParent(this);
        pScrollTexture_->SetDrawLayer("ARTWORK");
        pScrollTexture_->SetName("$parentScrollTexture");

        if (!pManager_->AddUIObject(pScrollTexture_))
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

        if (pScrollRenderTarget_)
            pScrollTexture_->SetTexture(pScrollRenderTarget_);

        bRebuildScrollRenderTarget_ = true;
    }

    pScrollChild_ = pFrame;

    if (pScrollChild_)
    {
        if (pScrollChild_->GetParent() != this)
        {
            pScrollChild_->SetParent(this);
            AddChild(pScrollChild_);
        }
        pScrollChild_->SetManuallyRendered(true, this);
        pScrollChild_->ClearAllPoints();
        pScrollChild_->SetAbsPoint(ANCHOR_TOPLEFT, "", ANCHOR_TOPLEFT, -iHorizontalScroll_, -iVerticalScroll_);

        AddToScrollChildList_(pScrollChild_);

        iHorizontalScrollRange_ = s_int(pScrollChild_->GetAbsWidth()) - s_int(uiAbsWidth_);
        if (iHorizontalScrollRange_ < 0) iHorizontalScrollRange_ = 0;

        iVerticalScrollRange_ = s_int(pScrollChild_->GetAbsHeight()) - s_int(uiAbsHeight_);
        if (iVerticalScrollRange_ < 0) iVerticalScrollRange_ = 0;

        On("ScrollRangeChanged");

        bUpdateScrollRange_ = false;
    }

    bRebuildScrollStrataList_ = true;
    FireRedraw();
}

s_ptr<Frame> ScrollFrame::GetScrollChild()
{
    return pScrollChild_;
}

void ScrollFrame::SetHorizontalScroll( const s_int& iHorizontalScroll )
{
    if (iHorizontalScroll_ != iHorizontalScroll)
    {
        iHorizontalScroll_ = iHorizontalScroll;
        lQueuedEventList_.PushBack("HorizontalScroll");

        pScrollChild_->ModifyPoint(ANCHOR_TOPLEFT)->SetAbsOffset(-iHorizontalScroll_, -iVerticalScroll_);
        FireRedraw();
    }
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
    if (iVerticalScroll_ != iVerticalScroll)
    {
        iVerticalScroll_ = iVerticalScroll;
        lQueuedEventList_.PushBack("VerticalScroll");

        pScrollChild_->ModifyPoint(ANCHOR_TOPLEFT)->SetAbsOffset(-iHorizontalScroll_, -iVerticalScroll_);
        FireRedraw();
    }
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
    s_uint uiOldChildWidth;
    s_uint uiOldChildHeight;

    if (pScrollChild_)
    {
        uiOldChildWidth = pScrollChild_->GetAbsWidth();
        uiOldChildHeight = pScrollChild_->GetAbsHeight();
    }

    Frame::Update();

    if ( pScrollChild_ && (uiOldChildWidth != pScrollChild_->GetAbsWidth() || uiOldChildHeight != pScrollChild_->GetAbsHeight()) )
    {
        bUpdateScrollRange_ = true;
        FireRedraw();
    }

    if (IsVisible())
    {
        if (bRebuildScrollRenderTarget_ && pScrollTexture_)
        {
            RebuildScrollRenderTarget_();
            bRebuildScrollRenderTarget_ = false;
            FireRedraw();
        }

        if (bUpdateScrollRange_)
        {
            UpdateScrollRange_();
            bUpdateScrollRange_ = false;
        }

        if (bRebuildScrollStrataList_)
        {
            RebuildScrollStrataList_();
            bRebuildScrollStrataList_ = false;
            FireRedraw();
        }

        if (pScrollChild_)
        {
            UpdateScrollChildInput_();
        }

        if (pScrollChild_ && pScrollRenderTarget_ && bRedrawScrollRenderTarget_)
        {
            RenderScrollStrataList_();
            bRedrawScrollRenderTarget_ = false;
        }
    }
}

void ScrollFrame::UpdateScrollRange_()
{
    iHorizontalScrollRange_ = s_int(pScrollChild_->GetAbsWidth()) - s_int(uiAbsWidth_);
    if (iHorizontalScrollRange_ < 0) iHorizontalScrollRange_ = 0;

    iVerticalScrollRange_ = s_int(pScrollChild_->GetAbsHeight()) - s_int(uiAbsHeight_);
    if (iVerticalScrollRange_ < 0) iVerticalScrollRange_ = 0;

    On("ScrollRangeChanged");
}

void ScrollFrame::UpdateScrollChildInput_()
{
    s_int iX = iMousePosX_ - lBorderList_[BORDER_LEFT];
    s_int iY = iMousePosY_ - lBorderList_[BORDER_TOP];

    if (bMouseInScrollTexture_)
    {
        s_ptr<Frame> pOveredFrame;

        s_map<FrameStrata, Strata>::const_iterator iterStrata = lScrollStrataList_.End();
        while (iterStrata != lScrollStrataList_.Begin() && !pOveredFrame)
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

        if (pOveredFrame != pOveredScrollChild_)
        {
            if (pOveredScrollChild_)
                pOveredScrollChild_->NotifyMouseInFrame(false, iX, iY);

            pOveredScrollChild_ = pOveredFrame;
        }

        if (pOveredScrollChild_)
            pOveredScrollChild_->NotifyMouseInFrame(true, iX, iY);
    }
    else if (pOveredScrollChild_)
    {
        pOveredScrollChild_->NotifyMouseInFrame(false, iX, iY);
        pOveredScrollChild_ = nullptr;
    }
}

void ScrollFrame::RebuildScrollRenderTarget_()
{
    if (!pScrollRenderTarget_)
    {
        if (uiAbsWidth_.IsValid() && uiAbsHeight_.IsValid())
            pScrollRenderTarget_ = new RenderTarget(uiAbsWidth_, uiAbsHeight_);
    }
    else
    {
        if (uiAbsWidth_.IsValid() && uiAbsHeight_.IsValid())
        {
            pScrollRenderTarget_->SetDimensions(uiAbsWidth_, uiAbsHeight_);
            bUpdateScrollRange_ = true;
        }
    }

    if (pScrollTexture_ && pScrollRenderTarget_)
        pScrollTexture_->SetTexture(pScrollRenderTarget_);
}

void ScrollFrame::RebuildScrollStrataList_()
{
    lScrollStrataList_.Clear();

    s_map< s_uint, s_ptr<GUI::Frame> >::iterator iterFrame;
    foreach (iterFrame, lScrollChildList_)
    {
        s_ptr<GUI::Frame> pFrame = iterFrame->second;
        lScrollStrataList_[pFrame->GetFrameStrata()].
            lLevelList[pFrame->GetFrameLevel()].
                lFrameList.PushBack(pFrame);
    }
}

void ScrollFrame::RenderScrollStrataList_()
{
    pManager_->Begin(pScrollRenderTarget_);
    pScrollRenderTarget_->Clear(Color::VOID);

    s_map<FrameStrata, Strata>::const_iterator iterStrata;
    foreach (iterStrata, lScrollStrataList_)
    {
        const Strata& mStrata = iterStrata->second;

        s_map<s_int, Level>::const_iterator iterLevel;
        foreach (iterLevel, mStrata.lLevelList)
        {
            const Level& mLevel = iterLevel->second;

            s_ctnr< s_ptr<Frame> >::const_iterator iterFrame;
            foreach (iterFrame, mLevel.lFrameList)
            {
                s_ptr<Frame> pFrame = *iterFrame;
                if (!pFrame->IsNewlyCreated())
                    pFrame->Render();
            }
        }
    }

    pManager_->End();
}

s_bool ScrollFrame::IsInFrame( const s_int& iX, const s_int& iY ) const
{
    if (pScrollTexture_)
        return Frame::IsInFrame(iX, iY) || pScrollTexture_->IsInRegion(iX, iY);
    else
        return Frame::IsInFrame(iX, iY);
}

void ScrollFrame::NotifyMouseInFrame( const s_bool& bMouseInFrame, const s_int& iX, const s_int& iY )
{
    Frame::NotifyMouseInFrame(bMouseInFrame, iX, iY);
    bMouseInScrollTexture_ = (bMouseInFrame && pScrollTexture_ && pScrollTexture_->IsInRegion(iX, iY));
}

void ScrollFrame::FireRedraw()
{
    bRedrawScrollRenderTarget_ = true;
    NotifyRendererNeedRedraw();
}

void ScrollFrame::NotifyChildStrataChanged( s_ptr<Frame> pChild )
{
    if (pChild == pScrollChild_)
    {
        bRebuildScrollStrataList_ = true;
    }
    else
    {
        if (pParentFrame_)
            pParentFrame_->NotifyChildStrataChanged(this);
        else
            pManager_->FireBuildStrataList();
    }
}

void ScrollFrame::CreateGlue()
{
    if (bVirtual_)
    {
        s_ptr<Lua::State> pLua = pManager_->GetLua();
        pLua->PushNumber(uiID_);
        lGlueList_.PushBack(pLua->PushNew<LuaVirtualGlue>());
        pLua->SetGlobal(sLuaName_);
    }
    else
    {
        s_ptr<Lua::State> pLua = pManager_->GetLua();
        pLua->PushString(sLuaName_);
        lGlueList_.PushBack(pLua->PushNew<LuaScrollFrame>());
        pLua->SetGlobal(sLuaName_);
    }
}

void ScrollFrame::AddToScrollChildList_( s_ptr<Frame> pChild )
{
    lScrollChildList_[pChild->GetID()] = pChild;
    s_map< s_uint, s_ptr<Frame> >::const_iterator iterChild;
    foreach (iterChild, pChild->GetChildren())
    {
        AddToScrollChildList_(iterChild->second);
    }
}

void ScrollFrame::RemoveFromScrollChildList_( s_ptr<Frame> pChild )
{
    lScrollChildList_.Erase(pChild->GetID());
    s_map< s_uint, s_ptr<Frame> >::const_iterator iterChild;
    foreach (iterChild, pChild->GetChildren())
    {
        RemoveFromScrollChildList_(iterChild->second);
    }
}

void ScrollFrame::NotifyManuallyRenderedObject_( s_ptr<UIObject> pObject, const s_bool& bManuallyRendered )
{
    s_ptr<Frame> pFrame = s_ptr<Frame>::DynamicCast(pObject);
    if (pFrame)
    {
        if (bManuallyRendered)
            AddToScrollChildList_(pFrame);
        else
            RemoveFromScrollChildList_(pFrame);

        bRebuildScrollStrataList_ = true;
    }
}
