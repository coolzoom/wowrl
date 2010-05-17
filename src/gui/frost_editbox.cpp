/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_editbox.h"

#include "gui/frost_frame.h"
#include "gui/frost_fontstring.h"
#include "gui/frost_texture.h"
#include "gui/frost_guimanager.h"
#include "gui/frost_sprite.h"
#include "frost_inputmanager.h"
#include "frost_localemanager.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str EditBox::CLASS_NAME = "GUI::EditBox";

// TODO : Implement EditBox

EditBox::EditBox() : Frame()
{
    lType_.PushBack("EditBox");

    dBlinkSpeed_ = 0.5;
    pCarretTimer_ = s_refptr<PeriodicTimer>(new PeriodicTimer(
        dBlinkSpeed_, PeriodicTimer::START_FIRST_TICK, false
    ));

    iterCarretPos_ = sUnicodeText_.Begin();
}

EditBox::~EditBox()
{
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
            if (!GUIManager::GetSingleton()->AddUIObject(pFontString_))
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
            }
        }
    }
}

void EditBox::Update()
{
    Frame::Update();

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
}

void EditBox::OnEvent( const Event& mEvent )
{
    Frame::OnEvent(mEvent);

    if (mEvent.GetName() == "MOUSE_PRESSED" && bMouseInFrame_)
    {
        SetFocus(true);
    }

    if (mEvent.GetName() == "KEY_PRESSED" && bFocus_)
    {
        s_uint uiChar = mEvent[0].Get<s_uint>();
        if (uiChar == KEY_ESCAPE)
        {
            On("EscapePressed");
        }
        else if (uiChar == KEY_RETURN || uiChar == KEY_NUMPADENTER)
        {
            On("EnterPressed");

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
        else if (uiChar == KEY_BACK)
        {
            if (MoveCarretHorizontally_(false))
                RemoveChar_();
        }
        else if (uiChar == KEY_DELETE)
        {
            RemoveChar_();
        }
        else if (uiChar == KEY_END)
        {
            iterCarretPos_ = sUnicodeText_.End();
            UpdateCarretPosition_();
        }
        else if (uiChar == KEY_HOME)
        {
            iterCarretPos_ = sUnicodeText_.Begin();
            UpdateCarretPosition_();
        }
        else
        {
            if (uiChar == KEY_LEFT || uiChar == KEY_RIGHT || uiChar == KEY_UP || uiChar == KEY_DOWN)
            {
                if (!bArrowsIgnored_)
                {
                    if (uiChar == KEY_LEFT)
                        MoveCarretHorizontally_(false);
                    else if (uiChar == KEY_RIGHT)
                        MoveCarretHorizontally_(true);
                    else if (uiChar == KEY_UP)
                        MoveCarretVertically_(false);
                    else if (uiChar == KEY_DOWN)
                        MoveCarretVertically_(true);
                }

                return;
            }

            if (uiChar == KEY_TAB)
            {
                On("TabPressed");
            }
            else if (uiChar == KEY_SPACE)
            {
                On("SpacePressed");
            }

            if (sComboKey_.IsEmpty())
            {
                s_str sChar = InputManager::GetSingleton()->GetKeyString((KeyCode)uiChar.Get());

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
                s_str sChar = InputManager::GetSingleton()->GetKeyComboString(sComboKey_, (KeyCode)uiChar.Get());

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
}

void EditBox::On( const s_str& sScriptName, s_ptr<Event> pEvent )
{
    if (bFocus_ && (sScriptName == "KeyUp" || sScriptName == "KeyDown"))
        return;

    if (lDefinedScriptList_.Find(sScriptName))
    {
        s_ptr<Lua::State> pLua = GUIManager::GetSingleton()->GetLua();

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
}

void EditBox::CreateGlue()
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
            pLua->Push<LuaEditBox>(new LuaEditBox(pLua->GetState()))
        );
        pLua->SetGlobal(sLuaName_);
    }
}

void EditBox::SetText( const s_str& sText )
{
    if (sText != sText_)
    {
        sText_ = sText;
        CheckText_();
        UpdateDisplayedText_();
        iterCarretPos_ = sUnicodeText_.End();
        UpdateFontString_();
        UpdateCarretPosition_();
        On("TextSet");
    }
}

const s_str& EditBox::GetText() const
{
    return sText_;
}

void EditBox::HighlightText( const s_uint& uiStart, const s_uint& uiEnd )
{
    if (!pHighlight_)
        CreateHighlight_();
}

void EditBox::SetHighlightColor( const Color& mColor )
{
    if (!pHighlight_)
        CreateHighlight_();

    pHighlight_->SetColor(mColor);
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

        CheckText_();
        iterCarretPos_ = sUnicodeText_.End();
        UpdateDisplayedText_();
        UpdateCarretPosition_();
    }
}

const s_bool& EditBox::IsNumericOnly() const
{
    return bNumericOnly_;
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
        GUIManager::GetSingleton()->RequestFocus(this);
    else
        GUIManager::GetSingleton()->RequestFocus(nullptr);
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
        pFontString_ = new FontString();
        pFontString_->SetSpecial();
        pFontString_->SetParent(this);
        pFontString_->SetDrawLayer(LAYER_ARTWORK);
        pFontString_->SetWordWrap(bMultiLine_, bMultiLine_);
    }

    return pFontString_;
}

void EditBox::CreateHighlight_()
{
    pHighlight_ = new Texture();
    pHighlight_->SetSpecial();
    pHighlight_->SetParent(this);
    pHighlight_->SetDrawLayer(LAYER_HIGHLIGHT);
    pHighlight_->SetName("$parentHighlight");

    if (!GUIManager::GetSingleton()->AddUIObject(pHighlight_))
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
}

void EditBox::CreateCarret_()
{
    if (pFontString_ && pFontString_->GetTextObject())
    {
        pCarret_ = new Texture();
        pCarret_->SetSpecial();
        pCarret_->SetParent(this);
        pCarret_->SetDrawLayer(LAYER_HIGHLIGHT);
        pCarret_->SetName("$parentCarret");

        if (!GUIManager::GetSingleton()->AddUIObject(pCarret_))
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

        Quad mQuad = pFontString_->GetTextObject()->CreateQuad((uint)(uchar)'|');

        pCarret_->SetMaterial(mQuad.pMat);

        pCarret_->SetAbsWidth(s_uint(mQuad.lVertexArray[2].fX));
        pCarret_->SetAbsHeight(s_uint(mQuad.lVertexArray[2].fY));

        pCarret_->SetTexCoord(s_array<s_float,4>((
            mQuad.lVertexArray[0].fTX, mQuad.lVertexArray[2].fTX,
            mQuad.lVertexArray[0].fTY, mQuad.lVertexArray[2].fTY
        )));

        pCarret_->SetAbsPoint(ANCHOR_LEFT, sName_, ANCHOR_LEFT, lTextInsets_[BORDER_LEFT] - 1, 0);
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
}

void EditBox::UpdateCarretPosition_()
{
    if (pFontString_ && pFontString_->GetTextObject() && pCarret_)
    {
        if (sDisplayedText_.IsEmpty())
        {
            pCarret_->SetAbsPoint(
                ANCHOR_LEFT, sName_, ANCHOR_LEFT,
                lTextInsets_[BORDER_LEFT] - 1, 0
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
                while ( (iterCarretPos_ != sUnicodeText_.Begin()) && (fLeftStringSize < fLeftStringMaxSize) )
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
            if (*iter == '\n')
            {
                if (iterLetter == lLetters.Begin() || iterLetter == lLetters.End())
                    fYOffset += pText->GetLineHeight()*pText->GetLineSpacing();
            }
            else
            {
                s_float fLastY = iterLetter->fY1;
                ++iterLetter;
                if (iterLetter != lLetters.End() && fLastY != iterLetter->fY1)
                    fYOffset += iterLetter->fY1 - fLastY;
            }
        }

        if (iterLetter == lLetters.Begin())
            pCarret_->SetAbsPoint(
                ANCHOR_LEFT, sName_, ANCHOR_LEFT,
                lTextInsets_[BORDER_LEFT] - 1, s_int(fYOffset)
            );
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
    s_ustr sUnicode = UTF8ToUnicode(sChar);

    if (sChar.IsEmpty() || uiNumLetters_ + sUnicode.GetSize() >= uiMaxLetters_)
        return false;

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

    return true;
}

s_bool EditBox::RemoveChar_()
{
    if (iterCarretPos_ != sUnicodeText_.End())
    {
        iterCarretPos_ = sUnicodeText_.Erase(iterCarretPos_);
        sText_ = UnicodeToUTF8(sUnicodeText_);
        --uiNumLetters_;

        UpdateDisplayedText_();
        UpdateFontString_();
        UpdateCarretPosition_();

        if (pCarret_)
            pCarret_->Show();

        pCarretTimer_->Zero();

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
        // TODO : # Implement multi line EditBox
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
