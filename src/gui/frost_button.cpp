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

    if ( pHighlightTexture_ && (sScriptName == "Enter") )
    {
        pHighlightTexture_->Show();
    }

    if ( pHighlightTexture_ && (sScriptName == "Leave") )
    {
        pHighlightTexture_->Hide();
        if (pPushedTexture_)
        {
            pPushedTexture_->Hide();
            if (pNormalTexture_)
                pNormalTexture_->Show();
        }
    }

    if ( pPushedTexture_ && (sScriptName == "MouseDown") )
    {
        pPushedTexture_->Show();
        if (pNormalTexture_)
            pNormalTexture_->Hide();
    }

    if ( pPushedTexture_ && (sScriptName == "MouseUp") )
    {
        pPushedTexture_->Hide();
        if (pNormalTexture_)
            pNormalTexture_->Show();

        On("Click");
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

s_ptr<FontString> Button::GetCurrentFontString()
{
    return NULL;
}

void Button::Disable()
{
}

void Button::Enable()
{
}

s_bool Button::IsEnabled() const
{
    return (mState_ != BUTTON_DISABLED);
}

ButtonState Button::GetButtonState() const
{
    return mState_;
}

void Button::LockHighlight()
{
}

void Button::UnlockHighlight()
{
}

