/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "frost_editbox.h"

#include "frost_frame.h"
#include "frost_fontstring.h"
#include "frost_texture.h"
#include "frost_guimanager.h"
#include "frost_sprite.h"
#include "frost_inputmanager.h"
#include "frost_localemanager.h"
#include <frost_utils_timers.h>
#include <frost_utils_event.h>

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str EditBox::CLASS_NAME = "GUI::EditBox";

EditBox::EditBox(s_ptr<GUIManager> pManager) : Frame(pManager)
{
    lType_.PushBack("EditBox");

    dBlinkSpeed_ = 0.5;
    pCarretTimer_ = s_refptr<PeriodicTimer>(new PeriodicTimer(
        dBlinkSpeed_, PeriodicTimer::START_FIRST_TICK, false
    ));

    iterCarretPos_ = sUnicodeText_.Begin();

    uiSelectionStartPos_ = s_uint::NaN;
    uiSelectionEndPos_ = s_uint::NaN;

    mHighlightColor_ = Color(0.5f, 1.0f, 1.0f, 1.0f);

    dKeyRepeatSpeed_ = 0.05;
    uiLastKeyPressed_ = s_uint::NaN;

    RegisterForDrag((s_str("LeftButton")));
}

EditBox::~EditBox()
{
    SetFocus(false);
}

s_bool EditBox::CanUseScript( const s_str& sScriptName ) const
{
    if (Frame::CanUseScript(sScriptName))
        return true;
    else if ((sScriptName == "OnChar") ||
        (sScriptName == "OnCursorChanged") ||
        (sScriptName == "OnEditFocusGained") ||
        (sScriptName == "OnEditFocusLost") ||
        (sScriptName == "OnEnterPressed") ||
        (sScriptName == "OnEscapePressed") ||
        (sScriptName == "OnSpacePressed") ||
        (sScriptName == "OnTabPressed") ||
        (sScriptName == "OnTextChanged") ||
        (sScriptName == "OnTextSet"))
        return true;
    else
        return false;
}

void EditBox::CopyFrom( s_ptr<UIObject> pObj )
{
    Frame::CopyFrom(pObj);

    s_ptr<EditBox> pEditBox = s_ptr<EditBox>::DynamicCast(pObj);

    if (pEditBox)
    {
        this->SetMaxLetters(pEditBox->GetMaxLetters());
        this->SetBlinkSpeed(pEditBox->GetBlinkSpeed());
        this->SetNumericOnly(pEditBox->IsNumericOnly());
        this->SetPositiveOnly(pEditBox->IsPositiveOnly());
        this->SetIntegerOnly(pEditBox->IsIntegerOnly());
        this->EnablePasswordMode(pEditBox->IsPasswordModeEnabled());
        this->SetMultiLine(pEditBox->IsMultiLine());
        this->SetMaxHistoryLines(pEditBox->GetMaxHistoryLines());
        this->EnableAutoFocus(pEditBox->IsAutoFocusEnabled());
        this->SetTextInsets(pEditBox->GetTextInsets());

        s_ptr<FontString> pFS = pEditBox->GetFontString();
        if (pFS)
        {
            this->CreateFontString_();
            if (this->IsVirtual())
                pFontString_->SetVirtual();
            pFontString_->SetName(pFS->GetName());
            if (!pManager_->AddUIObject(pFontString_))
            {
                Warning(lType_.Back(),
                    "Trying to add \""+pFS->GetName()+"\" to \""+sName_+"\",\n"
                    "but its name was already taken : \""+pFontString_->GetName()+"\". Skipped."
                );
                pFontString_.Delete();
            }
            else
            {
                if (!IsVirtual())
                    pFontString_->CreateGlue();
                this->AddRegion(pFontString_);
                pFontString_->CopyFrom(pFS);

                if (!IsVirtual())
                    pFontString_->EnableFormatting(false);

                pFontString_->NotifyLoaded();
            }
        }
    }
}

void EditBox::Update()
{
    Frame::Update();

    if (bMouseDragged_ && uiSelectionStartPos_.IsValid())
    {
        s_uint uiPos = GetLetterIDAt_(iMousePosX_, iMousePosY_);
        if (uiSelectionEndPos_.IsValid() != uiPos.IsValid() || uiPos != uiSelectionEndPos_)
        {
            if (uiPos.IsValid())
            {
                HighlightText(uiSelectionStartPos_, uiPos);
                iterCarretPos_ = sUnicodeText_.Begin() + uiPos.Get();
                UpdateCarretPosition_();
            }
            else
            {
                s_uint uiTemp = uiSelectionStartPos_;
                UnlightText();
                uiSelectionStartPos_ = uiTemp;
                iterCarretPos_ = sUnicodeText_.Begin() + uiSelectionStartPos_.Get();
                UpdateCarretPosition_();
            }
        }
    }

    if (bFocus_ && pCarretTimer_->Ticks())
    {
        if (!pCarret_)
            CreateCarret_();

        if (pCarret_)
        {
            if (pCarret_->IsShown())
                pCarret_->Hide();
            else
                pCarret_->Show();
        }
    }

    if (bFocus_ && uiLastKeyPressed_.IsValid() &&
        InputManager::GetSingleton()->KeyIsDownLong((KeyCode)uiLastKeyPressed_.Get(), true))
    {
        if (!pKeyRepeatTimer_)
        {
            pKeyRepeatTimer_ = s_refptr<PeriodicTimer>(new PeriodicTimer(
                dKeyRepeatSpeed_, PeriodicTimer::START_FIRST_TICK, true
            ));
        }

        if (pKeyRepeatTimer_->Ticks())
        {
            ProcessKey_(uiLastKeyPressed_);
        }
    }
}

void EditBox::OnEvent( const Event& mEvent )
{
    Frame::OnEvent(mEvent);

    if (!pManager_->IsInputEnabled())
        return;

    if (mEvent.GetName() == "MOUSE_PRESSED" && bMouseInFrame_)
    {
        SetFocus(true);
        UnlightText();

        MoveCarretAt_(iMousePosX_, iMousePosY_);
    }

    if (mEvent.GetName() == "KEY_PRESSED" && bFocus_)
    {
        s_uint uiChar = mEvent[0].Get<s_uint>();
        if (uiChar == KEY_RETURN || uiChar == KEY_NUMPADENTER)
        {
            On("EnterPressed");
        }
        else if (uiChar == KEY_END)
        {
            s_uint uiPreviousCarretPos = iterCarretPos_ - sUnicodeText_.Begin();

            iterCarretPos_ = sUnicodeText_.End();
            UpdateCarretPosition_();

            if (InputManager::GetSingleton()->ShiftPressed())
            {
                if (bSelectedText_)
                    HighlightText(uiSelectionStartPos_, iterCarretPos_ - sUnicodeText_.Begin());
                else
                    HighlightText(uiPreviousCarretPos, iterCarretPos_ - sUnicodeText_.Begin());
            }
            else
                UnlightText();

            return;
        }
        else if (uiChar == KEY_HOME)
        {
            s_uint uiPreviousCarretPos = iterCarretPos_ - sUnicodeText_.Begin();

            iterCarretPos_ = sUnicodeText_.Begin();
            UpdateCarretPosition_();

            if (InputManager::GetSingleton()->ShiftPressed())
            {
                if (bSelectedText_)
                    HighlightText(uiSelectionStartPos_, iterCarretPos_ - sUnicodeText_.Begin());
                else
                    HighlightText(uiPreviousCarretPos, iterCarretPos_ - sUnicodeText_.Begin());
            }
            else
                UnlightText();

            return;
        }
        else if (uiChar == KEY_TAB)
        {
            On("TabPressed");
        }
        else if (uiChar == KEY_SPACE)
        {
            On("SpacePressed");
        }

        uiLastKeyPressed_ = uiChar;

        ProcessKey_(uiChar);
    }
    else if (mEvent.GetName() == "KEY_RELEASED")
    {
        s_uint uiChar = mEvent[0].Get<s_uint>();

        if (uiChar == KEY_ESCAPE)
        {
            On("EscapePressed");
            return;
        }

        if (uiChar == uiLastKeyPressed_)
        {
            uiLastKeyPressed_ = s_uint::NaN;
            pKeyRepeatTimer_ = nullptr;
        }
    }
}

void EditBox::On( const s_str& sScriptName, s_ptr<Event> pEvent )
{
    if (bFocus_ && (sScriptName == "KeyUp" || sScriptName == "KeyDown"))
        return;

    if (lDefinedScriptList_.Find(sScriptName))
    {
        s_ptr<Lua::State> pLua = pManager_->GetLua();

        if (sScriptName == "Char")
        {
            // Set key name
            if (pEvent)
            {
                pLua->PushString(pEvent->Get(0)->Get<s_str>());
                pLua->SetGlobal("arg1");
            }
        }
    }

    Frame::On(sScriptName, pEvent);

    if (sScriptName == "Hide")
        SetFocus(false);

    if (sScriptName == "SizeChanged")
    {
        UpdateDisplayedText_();
        UpdateFontString_();
        UpdateCarretPosition_();
    }

    if (sScriptName == "DragStart")
    {
        uiSelectionEndPos_ = uiSelectionStartPos_ = GetLetterIDAt_(iMousePosX_, iMousePosY_);
    }
}

void EditBox::CreateGlue()
{
    if (bVirtual_)
    {
        s_ptr<Lua::State> pLua = pManager_->GetLua();
        pLua->PushNumber(uiID_);
        lGlueList_.PushBack(pLua->PushNew<LuaVirtualGlue>());
        pLua->SetGlobal(sLuaName_);
        pLua->Pop();
    }
    else
    {
        s_ptr<Lua::State> pLua = pManager_->GetLua();
        pLua->PushString(sLuaName_);
        lGlueList_.PushBack(pLua->PushNew<LuaEditBox>());
        pLua->SetGlobal(sLuaName_);
        pLua->Pop();
    }
}

void EditBox::SetText( const s_str& sText )
{
    if (sText != sText_)
    {
        UnlightText();
        sText_ = sText;
        CheckText_();
        UpdateDisplayedText_();
        iterCarretPos_ = sUnicodeText_.End();
        UpdateFontString_();
        UpdateCarretPosition_();
        On("TextSet");
        On("TextChanged");
    }
}

const s_str& EditBox::GetText() const
{
    return sText_;
}

void EditBox::UnlightText()
{
    uiSelectionStartPos_ = s_uint::NaN;
    uiSelectionEndPos_   = s_uint::NaN;
    bSelectedText_ = false;

    if (pHighlight_)
        pHighlight_->Hide();
}

void EditBox::HighlightText( const s_uint& uiStart, const s_uint& uiEnd, const s_bool& bForceUpdate )
{
    if (!pHighlight_)
        CreateHighlight_();

    if (!pHighlight_)
        return;

    s_uint uiLeft  = s_uint::Min(uiStart, uiEnd);
    s_uint uiRight = s_uint::Max(uiStart, uiEnd);

    if (uiLeft == uiRight || uiRight < uiDisplayPos_ || uiLeft >= uiDisplayPos_ + sDisplayedText_.GetSize())
        pHighlight_->Hide();
    else
        pHighlight_->Show();

    if (!uiSelectionStartPos_.IsValid() || !uiSelectionEndPos_.IsValid() ||
        (uiSelectionStartPos_ != uiStart) || (uiSelectionEndPos_ != uiEnd) ||
        bForceUpdate)
    {
        if (uiLeft != uiRight && uiRight >= uiDisplayPos_ &&
            uiLeft < uiDisplayPos_ + sDisplayedText_.GetSize() &&
            pFontString_ && pFontString_->GetTextObject())
        {
            bSelectedText_ = true;

            s_int iLeftPos;
            s_int iRightPos = pFontString_->GetRight() - pFontString_->GetLeft();

            s_wptr<Text> pText = pFontString_->GetTextObject();
            const s_ctnr<Text::Letter>& lLetters = pText->GetLetterCache();

            s_ctnr<Text::Letter>::const_iterator iter;
            foreach (iter, lLetters)
            {
                s_uint uiPos = iter - lLetters.Begin() + uiDisplayPos_;

                if (uiPos == uiLeft)
                    iLeftPos = s_int(iter->fX1) + lTextInsets_[BORDER_LEFT];

                if (uiPos == uiRight - 1)
                {
                    iRightPos = s_int(iter->fX2) + lTextInsets_[BORDER_LEFT];
                    break;
                }
            }

            pHighlight_->SetAbsPoint(ANCHOR_LEFT,  sName_, ANCHOR_LEFT, iLeftPos,  0);
            pHighlight_->SetAbsPoint(ANCHOR_RIGHT, sName_, ANCHOR_LEFT, iRightPos, 0);
        }
    }

    uiSelectionStartPos_ = uiStart;
    uiSelectionEndPos_   = uiEnd;
}

void EditBox::SetHighlightColor( const Color& mColor )
{
    if (mHighlightColor_ != mColor)
    {
        mHighlightColor_ = mColor;

        if (!pHighlight_)
            CreateHighlight_();

        if (!pHighlight_)
            return;

        pHighlight_->SetColor(mHighlightColor_);
    }
}

void EditBox::InsertAfterCursor( const s_str& sText )
{
    if (!sText.IsEmpty())
    {
        if (bNumericOnly_ && !sText.IsNumber())
            return;

        s_ustr sUStr = UTF8ToUnicode(sText);
        if (sUnicodeText_.GetSize() + sUStr.GetSize() <= uiMaxLetters_)
        {
            UnlightText();
            iterCarretPos_ = sUnicodeText_.Insert(sUStr, iterCarretPos_);
            sText_ = UnicodeToUTF8(sUnicodeText_);
            uiNumLetters_ = sUnicodeText_.GetSize();

            UpdateDisplayedText_();
            UpdateFontString_();
            UpdateCarretPosition_();
        }
    }
}

void EditBox::SetMaxLetters( const s_uint& uiMaxLetters )
{
    if (uiMaxLetters_ == 0 && !uiMaxLetters_.IsInfinite())
    {
        uiMaxLetters_ = s_uint::INF;
        return;
    }

    if (uiMaxLetters_ != uiMaxLetters)
    {
        uiMaxLetters_ = uiMaxLetters;

        s_uint uiCarretPos = iterCarretPos_ - sUnicodeText_.Begin();

        CheckText_();

        if (uiCarretPos > uiMaxLetters_)
        {
            iterCarretPos_ = sUnicodeText_.End();
            UpdateDisplayedText_();
            UpdateFontString_();
            UpdateCarretPosition_();
        }
        else
            iterCarretPos_ = sUnicodeText_.Begin() + uiCarretPos.Get();
    }
}

const s_uint& EditBox::GetMaxLetters() const
{
    return uiMaxLetters_;
}

const s_uint& EditBox::GetNumLetters() const
{
    return uiNumLetters_;
}

void EditBox::SetBlinkSpeed( const s_double& dBlinkSpeed )
{
    if (dBlinkSpeed_ != dBlinkSpeed)
    {
        dBlinkSpeed_ = dBlinkSpeed;
        pCarretTimer_ = s_refptr<PeriodicTimer>(new PeriodicTimer(
            dBlinkSpeed_, PeriodicTimer::START_FIRST_TICK, false
        ));
    }
}

const s_double& EditBox::GetBlinkSpeed() const
{
    return dBlinkSpeed_;
}

void EditBox::SetNumericOnly( const s_bool& bNumericOnly )
{
    if (bNumericOnly_ != bNumericOnly)
    {
        bNumericOnly_ = bNumericOnly;

        if (bNumericOnly_)
        {
            CheckText_();
            iterCarretPos_ = sUnicodeText_.End();
            UpdateDisplayedText_();
            UpdateCarretPosition_();
        }
    }
}

void EditBox::SetPositiveOnly( const s_bool& bPositiveOnly )
{
    if (bPositiveOnly_ != bPositiveOnly)
    {
        bPositiveOnly_ = bPositiveOnly;

        if (bNumericOnly_ && bPositiveOnly_)
        {
            CheckText_();
            iterCarretPos_ = sUnicodeText_.End();
            UpdateDisplayedText_();
            UpdateCarretPosition_();
        }
    }
}

void EditBox::SetIntegerOnly( const s_bool& bIntegerOnly )
{
    if (bIntegerOnly_ != bIntegerOnly)
    {
        bIntegerOnly_ = bIntegerOnly;

        if (bIntegerOnly_ && bPositiveOnly_)
        {
            CheckText_();
            iterCarretPos_ = sUnicodeText_.End();
            UpdateDisplayedText_();
            UpdateCarretPosition_();
        }
    }
}

const s_bool& EditBox::IsNumericOnly() const
{
    return bNumericOnly_;
}

const s_bool& EditBox::IsPositiveOnly() const
{
    return bPositiveOnly_;
}

const s_bool& EditBox::IsIntegerOnly() const
{
    return bIntegerOnly_;
}

void EditBox::EnablePasswordMode( const s_bool& bEnable )
{
    if (bPasswordMode_ != bEnable)
    {
        bPasswordMode_ = bEnable;

        UpdateDisplayedText_();
        UpdateFontString_();
        UpdateCarretPosition_();
    }
}

const s_bool& EditBox::IsPasswordModeEnabled() const
{
    return bPasswordMode_;
}

void EditBox::SetMultiLine( const s_bool& bMultiLine )
{
    if (bMultiLine_ != bMultiLine)
    {
        bMultiLine_ = bMultiLine;

        if (pFontString_)
            pFontString_->SetWordWrap(bMultiLine_, bMultiLine_);

        CheckText_();
        iterCarretPos_ = sUnicodeText_.End();
        UpdateDisplayedText_();
        UpdateCarretPosition_();
    }
}

const s_bool& EditBox::IsMultiLine() const
{
    return bMultiLine_;
}

void EditBox::SetMaxHistoryLines( const s_uint& uiMaxHistoryLines )
{
    if (uiMaxHistoryLines == 0 && !uiMaxHistoryLines_.IsInfinite())
    {
        uiMaxHistoryLines_ = s_uint::INF;
        return;
    }

    if (uiMaxHistoryLines_ != uiMaxHistoryLines)
    {
        uiMaxHistoryLines_ = uiMaxHistoryLines;

        while (lHistoryLineList_.GetSize() > uiMaxHistoryLines_)
        {
            lHistoryLineList_.PopFront();
        }
    }
}

const s_uint& EditBox::GetMaxHistoryLines() const
{
    return uiMaxHistoryLines_;
}

void EditBox::AddHistoryLine( const s_str& sHistoryLine )
{
    lHistoryLineList_.PushBack(sHistoryLine);

    while (lHistoryLineList_.GetSize() > uiMaxHistoryLines_)
    {
        lHistoryLineList_.PopFront();
    }
}

const s_ctnr<s_str>& EditBox::GetHistoryLines() const
{
    return lHistoryLineList_;
}

void EditBox::EnableAutoFocus( const s_bool& bEnable )
{
    bAutoFocus_ = bEnable;
}

const s_bool& EditBox::IsAutoFocusEnabled() const
{
    return bAutoFocus_;
}

void EditBox::SetFocus( const s_bool& bFocus )
{
    if (bFocus)
        pManager_->RequestFocus(this);
    else
        pManager_->RequestFocus(nullptr);
}

void EditBox::SetArrowsIgnored( const s_bool& bArrowsIgnored )
{
    bArrowsIgnored_ = bArrowsIgnored;
}

void EditBox::SetTextInsets( const s_int& iLeft, const s_int& iRight, const s_int& iTop, const s_int& iBottom )
{
    SetTextInsets((iLeft, iRight, iTop, iBottom));
}

void EditBox::SetTextInsets( const s_array<s_int,4>& lInsets )
{
    lTextInsets_ = lInsets;

    if (pFontString_)
    {
        pFontString_->ClearAllPoints();
        pFontString_->SetAbsPoint(
            ANCHOR_TOPLEFT, sName_, ANCHOR_TOPLEFT, lTextInsets_[BORDER_LEFT], lTextInsets_[BORDER_TOP]
        );
        pFontString_->SetAbsPoint(
            ANCHOR_BOTTOMRIGHT, sName_, ANCHOR_BOTTOMRIGHT, lTextInsets_[BORDER_RIGHT], lTextInsets_[BORDER_BOTTOM]
        );

        UpdateDisplayedText_();
        UpdateFontString_();
        UpdateCarretPosition_();
    }
}

const s_array<s_int,4>& EditBox::GetTextInsets() const
{
    return lTextInsets_;
}

void EditBox::NotifyFocus( const s_bool& bFocus )
{
    if (bFocus_ != bFocus)
    {
        bFocus_ = bFocus;
        if (bFocus_)
        {
            if (!pCarret_)
                CreateCarret_();

            if (pCarret_)
                pCarret_->Show();

            pCarretTimer_->Zero();

            lQueuedEventList_.PushBack("EditFocusGained");
        }
        else
        {
            if (pCarret_)
                pCarret_->Hide();

            lQueuedEventList_.PushBack("EditFocusLost");
        }
    }
}

s_ptr<FontString> EditBox::GetFontString()
{
    return pFontString_;
}

s_ptr<FontString> EditBox::CreateFontString_()
{
    if (!pFontString_)
    {
        pFontString_ = new FontString(pManager_);
        pFontString_->SetSpecial();
        pFontString_->SetParent(this);
        pFontString_->SetDrawLayer(LAYER_ARTWORK);
        pFontString_->SetWordWrap(bMultiLine_, bMultiLine_);
    }

    return pFontString_;
}

void EditBox::CreateHighlight_()
{
    pHighlight_ = new Texture(pManager_);
    pHighlight_->SetSpecial();
    pHighlight_->SetParent(this);
    pHighlight_->SetDrawLayer(LAYER_HIGHLIGHT);
    pHighlight_->SetName("$parentHighlight");

    if (!pManager_->AddUIObject(pHighlight_))
    {
        Warning(lType_.Back(),
            "Trying to create highlight texture for \""+sName_+"\",\n"
            "but its name was already taken : \""+pHighlight_->GetName()+"\". Skipped."
        );
        pHighlight_.Delete();
        return;
    }

    pHighlight_->CreateGlue();
    AddRegion(pHighlight_);

    pHighlight_->SetAbsPoint(
        ANCHOR_TOP,    sName_, ANCHOR_TOP,    0,  lTextInsets_[BORDER_TOP]
    );
    pHighlight_->SetAbsPoint(
        ANCHOR_BOTTOM, sName_, ANCHOR_BOTTOM, 0, -lTextInsets_[BORDER_BOTTOM]
    );

    pHighlight_->SetColor(mHighlightColor_);

    pHighlight_->NotifyLoaded();
}

void EditBox::CreateCarret_()
{
    if (pFontString_ && pFontString_->GetTextObject())
    {
        pCarret_ = new Texture(pManager_);
        pCarret_->SetSpecial();
        pCarret_->SetParent(this);
        pCarret_->SetDrawLayer(LAYER_HIGHLIGHT);
        pCarret_->SetName("$parentCarret");

        if (!pManager_->AddUIObject(pCarret_))
        {
            Warning(lType_.Back(),
                "Trying to create carret texture for \""+sName_+"\",\n"
                "but its name was already taken : \""+pCarret_->GetName()+"\". Skipped."
            );
            pCarret_.Delete();
            return;
        }

        pCarret_->CreateGlue();
        AddRegion(pCarret_);

        s_refptr<Sprite> pSprite = pFontString_->GetTextObject()->CreateSprite((uint)(uchar)'|');
        pSprite->SetColor(pFontString_->GetTextColor());

        pCarret_->SetSprite(pSprite);
        pCarret_->SetAbsPoint(ANCHOR_LEFT, sName_, ANCHOR_LEFT, lTextInsets_[BORDER_LEFT] - 1, 0);

        pCarret_->NotifyLoaded();
    }
}

void EditBox::CheckText_()
{
    sUnicodeText_ = UTF8ToUnicode(sText_);

    if (!sUnicodeText_.IsNumber() && bNumericOnly_)
        sUnicodeText_ = "";
    else
    {
        while (sUnicodeText_.GetSize() > uiMaxLetters_)
            sUnicodeText_.PopBack();
    }

    uiNumLetters_ = sUnicodeText_.GetSize();

    sText_ = UnicodeToUTF8(sUnicodeText_);
}

void EditBox::UpdateDisplayedText_()
{
    if (pFontString_ && pFontString_->GetTextObject())
    {
        if (bPasswordMode_)
            sDisplayedText_ = s_ustr('*', sUnicodeText_.GetSize());
        else
            sDisplayedText_ = sUnicodeText_;

        if (!bMultiLine_)
        {
            s_wptr<Text> pTextObject = pFontString_->GetTextObject();

            if (pTextObject->GetBoxWidth().IsValid())
            {
                sDisplayedText_.EraseFromStart(uiDisplayPos_);

                while (pTextObject->GetStringWidth(sDisplayedText_) > pTextObject->GetBoxWidth())
                {
                    sDisplayedText_.PopBack();
                }
            }
        }
    }
}

void EditBox::UpdateFontString_()
{
    if (!pFontString_)
        return;

    pFontString_->SetText(UnicodeToUTF8(sDisplayedText_));

    if (bSelectedText_)
        HighlightText(uiSelectionStartPos_, uiSelectionEndPos_, true);
}

void EditBox::UpdateCarretPosition_()
{
    if (pFontString_ && pFontString_->GetTextObject() && pCarret_)
    {
        if (sDisplayedText_.IsEmpty())
        {
            AnchorPoint mPoint;
            s_int iOffset = 0;
            switch (pFontString_->GetJustifyH())
            {
                case Text::ALIGN_LEFT :
                    mPoint = ANCHOR_LEFT;
                    iOffset = lTextInsets_[BORDER_LEFT] - 1;
                    break;
                case Text::ALIGN_CENTER :
                    mPoint = ANCHOR_CENTER;
                    break;
                case Text::ALIGN_RIGHT :
                    mPoint = ANCHOR_RIGHT;
                    iOffset = -lTextInsets_[BORDER_RIGHT] - 1;
                    break;
                default : mPoint = ANCHOR_LEFT; break;
            }
            pCarret_->SetAbsPoint(
                ANCHOR_LEFT, sName_, mPoint, iOffset, 0
            );
            return;
        }

        s_wptr<Text> pText = pFontString_->GetTextObject();
        s_ustr::iterator iterDisplayCarret;

        if (bMultiLine_)
        {
            iterDisplayCarret = sDisplayedText_.Begin() + (iterCarretPos_ - sUnicodeText_.Begin() - uiDisplayPos_).Get();
        }
        else
        {
            s_uint uiGlobalPos = iterCarretPos_ - sUnicodeText_.Begin();

            if (uiDisplayPos_ > uiGlobalPos)
            {
                // The carret has been positioned before the start of the displayed string
                s_float fBoxWidth = pText->GetBoxWidth();
                s_float fLeftStringMaxSize = fBoxWidth*0.25f;
                s_float fLeftStringSize = 0.0f;
                s_ustr sLeftString;

                s_ustr::iterator iter = iterCarretPos_;
                while ( (iter != sUnicodeText_.Begin()) && (fLeftStringSize < fLeftStringMaxSize) )
                {
                    --iter;
                    sLeftString.PushFront(*iter);
                    fLeftStringSize = pText->GetStringWidth(sLeftString);
                }

                uiDisplayPos_ = iter - sUnicodeText_.Begin();
                UpdateDisplayedText_();
                UpdateFontString_();
            }

            s_uint uiCarretPos = uiGlobalPos - uiDisplayPos_;
            if (uiCarretPos > sDisplayedText_.GetSize())
            {
                // The carret has been positioned after the end of the displayed string
                s_float fBoxWidth = pText->GetBoxWidth();
                s_float fLeftStringMaxSize = fBoxWidth*0.75f;
                s_float fLeftStringSize = 0.0f;
                s_ustr sLeftString;

                s_ustr::iterator iter = iterCarretPos_;
                while ( (iterCarretPos_ != sUnicodeText_.Begin()) && (fLeftStringSize < fLeftStringMaxSize) )
                {
                    --iter;
                    sLeftString.PushFront(*iter);
                    fLeftStringSize = pText->GetStringWidth(sLeftString);
                }

                uiDisplayPos_ = iter - sUnicodeText_.Begin();
                UpdateDisplayedText_();
                UpdateFontString_();

                uiCarretPos = uiGlobalPos - uiDisplayPos_;
            }

            iterDisplayCarret = sDisplayedText_.Begin() + uiCarretPos.Get();
        }

        const s_ctnr<Text::Letter>& lLetters = pText->GetLetterCache();
        s_ctnr<Text::Letter>::const_iterator iterLetter = lLetters.Begin();

        s_float fYOffset = 0.0f;

        s_ustr::const_iterator iter;
        for (iter = sDisplayedText_.Begin(); iter != iterDisplayCarret; ++iter)
        {
            s_float fLastY = iterLetter->fY1;
            ++iterLetter;
            if (iterLetter != lLetters.End() && fLastY != iterLetter->fY1)
                fYOffset += iterLetter->fY1 - fLastY;
        }

        if (iterLetter == lLetters.Begin())
        {
            pCarret_->SetAbsPoint(
                ANCHOR_LEFT, sName_, ANCHOR_LEFT,
                lTextInsets_[BORDER_LEFT] + s_int(iterLetter->fX1) - 1, s_int(fYOffset)
            );
        }
        else
        {
            --iterLetter;
            pCarret_->SetAbsPoint(
                ANCHOR_LEFT, sName_, ANCHOR_LEFT,
                lTextInsets_[BORDER_LEFT] + s_int(iterLetter->fX2) - 1, s_int(fYOffset)
            );
        }
    }
}

s_bool EditBox::AddChar_( const s_str& sChar )
{
    if (sChar.IsEmpty())
        return false;

    if (bSelectedText_)
        RemoveChar_();

    s_ustr sUnicode = UTF8ToUnicode(sChar);
    if (uiNumLetters_ + sUnicode.GetSize() >= uiMaxLetters_)
        return false;

    if (bNumericOnly_)
    {
        if (sChar.GetLength() > 1)
            return false;

        if (sChar[0] == '.')
        {
            static uint UNICODE_DOT = UTF8ToUnicode('.');

            if (bIntegerOnly_)
                return false;

            if (sUnicodeText_.Find(UNICODE_DOT))
                return false;
        }
        else if (sChar[0] == '+' || sChar[0] == '-')
        {
            static uint UNICODE_PLUS = UTF8ToUnicode('+');
            static uint UNICODE_MINUS = UTF8ToUnicode('-');

            if (bPositiveOnly_)
                return false;

            if (iterCarretPos_ != sUnicodeText_.Begin() ||
                sUnicodeText_.Find(UNICODE_PLUS) || sUnicodeText_.Find(UNICODE_MINUS))
                return false;
        }
        else if (sChar[0] < '0' || sChar[0] > '9')
            return false;
    }

    s_ustr::iterator iter;
    foreach (iter, sUnicode)
    {
        iterCarretPos_ = sUnicodeText_.Insert(*iter, iterCarretPos_);
        ++uiNumLetters_;
    }

    sText_ = UnicodeToUTF8(sUnicodeText_);

    UpdateDisplayedText_();
    UpdateFontString_();
    UpdateCarretPosition_();

    if (pCarret_)
        pCarret_->Show();

    pCarretTimer_->Zero();

    On("TextChanged");

    return true;
}

s_bool EditBox::RemoveChar_()
{
    if (bSelectedText_)
    {
        if (uiSelectionStartPos_ != uiSelectionEndPos_)
        {
            s_uint uiLeft = s_uint::Min(uiSelectionStartPos_, uiSelectionEndPos_);
            s_uint uiRight = s_uint::Max(uiSelectionStartPos_, uiSelectionEndPos_);

            sUnicodeText_.EraseRange(uiLeft, uiRight);
            sText_ = UnicodeToUTF8(sUnicodeText_);
            uiNumLetters_ = sUnicodeText_.GetSize();

            iterCarretPos_ = sUnicodeText_.Begin() + uiLeft.Get();
            On("TextChanged");
        }

        UnlightText();
    }
    else
    {
        if (iterCarretPos_ == sUnicodeText_.End())
            return false;

        iterCarretPos_ = sUnicodeText_.Erase(iterCarretPos_);
        sText_ = UnicodeToUTF8(sUnicodeText_);
        --uiNumLetters_;
        On("TextChanged");
    }

    UpdateDisplayedText_();
    UpdateFontString_();
    UpdateCarretPosition_();

    if (pCarret_)
        pCarret_->Show();

    pCarretTimer_->Zero();

    return true;
}

s_uint EditBox::GetLetterIDAt_( const s_int& iX, const s_int& iY )
{
    if (pFontString_ && pFontString_->GetTextObject())
    {
        s_wptr<Text> pText = pFontString_->GetTextObject();
        const s_ctnr<Text::Letter>& lLetters = pText->GetLetterCache();

        if (lLetters.IsEmpty())
            return uiDisplayPos_;

        s_float fX = s_float(iX - lBorderList_[BORDER_LEFT] - lTextInsets_[BORDER_LEFT]);
        s_float fY = s_float(iY - lBorderList_[BORDER_TOP]  - lTextInsets_[BORDER_TOP]);

        if (!bMultiLine_)
        {
            if (iX < lBorderList_[BORDER_LEFT] + lTextInsets_[BORDER_LEFT])
                return uiDisplayPos_;
            else if (iX > lBorderList_[BORDER_RIGHT] - lTextInsets_[BORDER_RIGHT])
                return lLetters.GetSize() + uiDisplayPos_;

            s_ctnr<Text::Letter>::const_iterator iter;
            foreach (iter, lLetters)
            {
                if (fX < (iter->fX1 + iter->fX2)/2.0f)
                    return (iter - lLetters.Begin()) + uiDisplayPos_;
            }

            return lLetters.GetSize() + uiDisplayPos_;
        }
        else
        {
            // TODO : Implement multi line EditBox
            return uiDisplayPos_;
        }

    }

    return s_uint::NaN;
}

s_bool EditBox::MoveCarretAt_( const s_int& iX, const s_int& iY )
{
    s_uint uiPos = GetLetterIDAt_(iX, iY);
    if (uiPos.IsValid())
    {
        iterCarretPos_ = sUnicodeText_.Begin() + uiPos.Get();
        UpdateCarretPosition_();
        return true;
    }
    else
        return false;
}

s_bool EditBox::MoveCarretHorizontally_( const s_bool& bForward )
{
    if (bForward)
    {
        if (iterCarretPos_ != sUnicodeText_.End())
        {
            ++iterCarretPos_;
            UpdateCarretPosition_();

            if (pCarret_)
                pCarret_->Show();

            pCarretTimer_->Zero();

            return true;
        }
        else
            return false;
    }
    else
    {
        if (iterCarretPos_ != sUnicodeText_.Begin())
        {
            --iterCarretPos_;
            UpdateCarretPosition_();

            if (pCarret_)
                pCarret_->Show();

            pCarretTimer_->Zero();

            return true;
        }
        else
            return false;
    }
}

s_bool EditBox::MoveCarretVertically_( const s_bool& bDown )
{
    if (bMultiLine_)
    {
        // TODO : Implement multi line EditBox
        return false;
    }
    else
    {
        s_ustr::iterator iterOld = iterCarretPos_;

        if (bDown)
            iterCarretPos_ = sUnicodeText_.End();
        else
            iterCarretPos_ = sUnicodeText_.Begin();

        if (iterOld != iterCarretPos_)
        {
            UpdateCarretPosition_();

            if (pCarret_)
                pCarret_->Show();

            pCarretTimer_->Zero();

            return true;
        }
        else
            return false;
    }
}

void EditBox::ProcessKey_( const s_uint& uiKey )
{
    if (uiKey == KEY_RETURN || uiKey == KEY_NUMPADENTER)
    {
        if (bMultiLine_)
        {
            if (AddChar_("\n"))
            {
                Event mKeyEvent;
                mKeyEvent.Add(s_str("\n"));
                On("OnChar", &mKeyEvent);
            }
        }
    }
    else if (uiKey == KEY_BACK)
    {
        if (bSelectedText_)
        {
            RemoveChar_();
        }
        else
        {
            if (MoveCarretHorizontally_(false))
                RemoveChar_();
        }
    }
    else if (uiKey == KEY_DELETE)
    {
        RemoveChar_();
    }
    else
    {
        if (uiKey == KEY_LEFT || uiKey == KEY_RIGHT || uiKey == KEY_UP || uiKey == KEY_DOWN)
        {
            if (!bArrowsIgnored_)
            {
                s_uint uiPreviousCarretPos = iterCarretPos_ - sUnicodeText_.Begin();

                if (uiKey == KEY_LEFT)
                    MoveCarretHorizontally_(false);
                else if (uiKey == KEY_RIGHT)
                    MoveCarretHorizontally_(true);
                else if (uiKey == KEY_UP)
                    MoveCarretVertically_(false);
                else if (uiKey == KEY_DOWN)
                    MoveCarretVertically_(true);

                if (InputManager::GetSingleton()->ShiftPressed())
                {
                    if (bSelectedText_)
                        HighlightText(uiSelectionStartPos_, iterCarretPos_ - sUnicodeText_.Begin());
                    else
                        HighlightText(uiPreviousCarretPos, iterCarretPos_ - sUnicodeText_.Begin());
                }

                else
                    UnlightText();
            }

            return;
        }

        if (sComboKey_.IsEmpty())
        {
            s_str sChar = InputManager::GetSingleton()->GetKeyString((KeyCode)uiKey.Get());

            if (LocaleManager::GetSingleton()->IsComboKey(sChar))
            {
                sComboKey_ = sChar;
            }
            else if (AddChar_(sChar))
            {
                Event mKeyEvent;
                mKeyEvent.Add(sChar);
                On("OnChar", &mKeyEvent);
            }
        }
        else
        {
            s_str sChar = InputManager::GetSingleton()->GetKeyComboString(sComboKey_, (KeyCode)uiKey.Get());

            if (AddChar_(sChar))
            {
                Event mKeyEvent;
                mKeyEvent.Add(sChar);
                On("OnChar", &mKeyEvent);
            }

            sComboKey_ = "";
        }
    }
}
