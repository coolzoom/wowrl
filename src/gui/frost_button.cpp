/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_button.h"

#include "gui/frost_frame.h"
#include "gui/frost_guimanager.h"
#include "gui/frost_fontstring.h"
#include "gui/frost_texture.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str Button::CLASS_NAME = "GUI::Button";

Button::Button() : Frame()
{
}

void Button::CreateGlue()
{
    s_ptr<Lua::State> pLua = GUIManager::GetSingleton()->GetLua();
    pLua->PushString(sName_);
    lGlueList_.PushBack(
        pLua->Push<LuaButton>(new LuaButton(pLua->GetState()))
    );
    pLua->SetGlobal(sName_);
}

void Button::On( const s_str& sScriptName, s_ptr<Event> pEvent )
{
    Frame::On(sScriptName, pEvent);

    if (IsEnabled())
    {
        if (sScriptName == "Enter")
        {
            Highlight();
        }

        if (sScriptName == "Leave")
        {
            Unlight();
        }

        if (sScriptName == "MouseDown")
        {
            Push();
        }

        if (sScriptName == "MouseUp")
        {
            Release();
            On("Click");
        }
    }
}

void Button::OnEvent( const Event& mEvent )
{
    Frame::OnEvent(mEvent);

    if (mEvent.GetName() == "MOUSE_DOUBLE_CLICKED")
    {
        if (bMouseInFrame_)
        {
            On("DoubleClicked");
        }
    }
}

void Button::SetText( const s_str& sText )
{
    sText_ = sText;

    if (pNormalText_)
        pNormalText_->SetText(sText);

    if (pHighlightText_)
        pHighlightText_->SetText(sText);

    if (pDisabledText_)
        pDisabledText_->SetText(sText);
}

const s_str& Button::GetText() const
{
    return sText_;
}

void Button::CreateNormalTexture()
{
    if (!pNormalTexture_)
    {
        pNormalTexture_ = new Texture();
    }
}

void Button::CreatePushedTexture()
{
    if (!pPushedTexture_)
    {
        pPushedTexture_ = new Texture();
    }
}

void Button::CreateDisabledTexture()
{
    if (!pDisabledTexture_)
    {
        pDisabledTexture_ = new Texture();
    }
}

void Button::CreateHighlightTexture()
{
    if (!pHighlightTexture_)
    {
        pHighlightTexture_ = new Texture();
    }
}

void Button::CreateNormalText()
{
    if (!pNormalText_)
    {
        pNormalText_ = new FontString();
        pNormalText_->SetText(sText_);
    }
}

void Button::CreateHighlightText()
{
    if (!pHighlightText_)
    {
        pHighlightText_ = new FontString();
        pHighlightText_->SetText(sText_);
    }
}

void Button::CreateDisabledText()
{
    if (!pDisabledText_)
    {
        pDisabledText_ = new FontString();
        pDisabledText_->SetText(sText_);
    }
}

s_ptr<Texture> Button::GetNormalTexture()
{
    return pNormalTexture_;
}

s_ptr<Texture> Button::GetPushedTexture()
{
    return pPushedTexture_;
}

s_ptr<Texture> Button::GetDisabledTexture()
{
    return pDisabledTexture_;
}

s_ptr<Texture> Button::GetHighlightTexture()
{
    return pHighlightTexture_;
}

s_ptr<FontString> Button::GetNormalText()
{
    return pNormalText_;
}

s_ptr<FontString> Button::GetHighlightText()
{
    return pHighlightText_;
}

s_ptr<FontString> Button::GetDisabledText()
{
    return pDisabledText_;
}

void Button::SetNormalTexture( s_ptr<Texture> pTexture )
{
    pNormalTexture_ = pTexture;
}

void Button::SetPushedTexture( s_ptr<Texture> pTexture )
{
    pPushedTexture_ = pTexture;
}

void Button::SetDisabledTexture( s_ptr<Texture> pTexture )
{
    pDisabledTexture_ = pTexture;
}

void Button::SetHighlightTexture( s_ptr<Texture> pTexture )
{
    pHighlightTexture_ = pTexture;
}

void Button::SetNormalText( s_ptr<FontString> pFont )
{
    pNormalText_ = pFont;
}

void Button::SetHighlightText( s_ptr<FontString> pFont )
{
    pHighlightText_ = pFont;
}

void Button::SetDisabledText( s_ptr<FontString> pFont )
{
    pDisabledText_ = pFont;
}

s_ptr<FontString> Button::GetCurrentFontString()
{
    return pCurrentFontString_;
}

void Button::Disable()
{
    if (IsEnabled())
    {
        mState_ = BUTTON_DISABLED;
        if (pDisabledTexture_)
        {
            if (pNormalTexture_)
                pNormalTexture_->Hide();
            if (pPushedTexture_)
                pPushedTexture_->Hide();

            pDisabledTexture_->Show();
        }
        else
        {
            if (pNormalTexture_)
                pNormalTexture_->Show();
            if (pPushedTexture_)
                pPushedTexture_->Hide();
        }

        if (pDisabledText_)
        {
            if (pNormalText_)
                pNormalText_->Hide();

            pDisabledText_->Show();
            pCurrentFontString_ = pDisabledText_;
        }
        else
        {
            if (pNormalText_)
                pNormalText_->Show();

            pCurrentFontString_ = pNormalText_;
        }
    }
}

void Button::Enable()
{
    if (!IsEnabled())
    {
        mState_ = BUTTON_UP;
        if (pDisabledTexture_)
        {
            if (pNormalTexture_)
                pNormalTexture_->Show();
            if (pPushedTexture_)
                pPushedTexture_->Hide();

            pDisabledTexture_->Hide();
        }
        else
        {
            if (pNormalTexture_)
                pNormalTexture_->Show();
            if (pPushedTexture_)
                pPushedTexture_->Hide();
        }

        if (pNormalText_)
            pNormalText_->Show();

        pCurrentFontString_ = pNormalText_;

        if (pDisabledText_)
            pDisabledText_->Hide();
    }
}

s_bool Button::IsEnabled() const
{
    return (mState_ != BUTTON_DISABLED);
}

void Button::Push()
{
    if (IsEnabled())
    {
        pPushedTexture_->Show();
        if (pNormalTexture_)
            pNormalTexture_->Hide();
    }
}

void Button::Release()
{
    if (IsEnabled())
    {
        pPushedTexture_->Hide();
        if (pNormalTexture_)
            pNormalTexture_->Show();
    }
}

void Button::Highlight()
{
    if (!bHighlighted_)
    {
        if (pHighlightTexture_)
            pHighlightTexture_->Show();

        if (pCurrentFontString_)
            pCurrentFontString_->Hide();
        pCurrentFontString_ = pHighlightText_;
        if (pCurrentFontString_)
            pCurrentFontString_->Show();

        bHighlighted_ = true;
    }
}

void Button::Unlight()
{
    if (!bLockHighlight_ && bHighlighted_)
    {
        if (pHighlightTexture_)
            pHighlightTexture_->Hide();

        if (pCurrentFontString_)
            pCurrentFontString_->Hide();

        switch (mState_)
        {
            case BUTTON_UP       : pCurrentFontString_ = pNormalText_; break;
            case BUTTON_DOWN     : pCurrentFontString_ = pNormalText_; break;
            case BUTTON_DISABLED : pCurrentFontString_ = pDisabledText_; break;
        }

        if (pCurrentFontString_)
            pCurrentFontString_->Show();

        bHighlighted_ = false;
    }
}

ButtonState Button::GetButtonState() const
{
    return mState_;
}

void Button::LockHighlight()
{
    Highlight();
    bLockHighlight_ = true;
}

void Button::UnlockHighlight()
{
    if (!bMouseInFrame_)
        Unlight();

    bLockHighlight_ = false;
}

void Button::SetPushedTextOffset( const s_array<s_int,2>& lOffset )
{
    lPushedTextOffset_ = lOffset;
}

const s_array<s_int,2>& Button::GetPushedTextOffset() const
{
    return lPushedTextOffset_;
}

