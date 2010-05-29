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

Button::Button() : Frame(), mState_(STATE_UP)
{
    lType_.PushBack("Button");
}

Button::~Button()
{
}

s_str Button::Serialize( const s_str& sTab ) const
{
    return Frame::Serialize(sTab);
}

void Button::CreateGlue()
{
    if (bVirtual_)
    {
        s_ptr<Lua::State> pLua = GUIManager::GetSingleton()->GetLua();
        pLua->PushNumber(uiID_);
        lGlueList_.PushBack(pLua->PushNew<LuaVirtualGlue>());
        pLua->SetGlobal(sLuaName_);
    }
    else
    {
        s_ptr<Lua::State> pLua = GUIManager::GetSingleton()->GetLua();
        pLua->PushString(sName_);
        lGlueList_.PushBack(pLua->PushNew<LuaButton>());
        pLua->SetGlobal(sLuaName_);
    }
}

s_bool Button::CanUseScript( const s_str& sScriptName ) const
{
    if (Frame::CanUseScript(sScriptName))
        return true;
    else if ((sScriptName == "OnClick") ||
        (sScriptName == "OnDoubleClick"))
        return true;
    else
        return false;
}

void Button::On( const s_str& sScriptName, s_ptr<Event> pEvent )
{
    Frame::On(sScriptName, pEvent);

    if (IsEnabled())
    {
        if (sScriptName == "Enter")
            Highlight();

        if (sScriptName == "Leave")
            Unlight();

        if (sScriptName == "MouseDown")
            Push();

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

    if (mEvent.GetName() == "MOUSE_DOUBLE_CLICKED" && bMouseInFrame_)
    {
        On("DoubleClicked");
    }
}

void Button::CopyFrom( s_ptr<UIObject> pObj )
{
    Frame::CopyFrom(pObj);

    s_ptr<Button> pButton = s_ptr<Button>::DynamicCast(pObj);

    if (pButton)
    {
        this->SetText(pButton->GetText());

        if (pButton->GetNormalTexture())
        {
            this->CreateNormalTexture_();
            if (this->IsVirtual())
                pNormalTexture_->SetVirtual();
            pNormalTexture_->SetName(pButton->GetNormalTexture()->GetName());
            if (!GUIManager::GetSingleton()->AddUIObject(pNormalTexture_))
            {
                Warning(lType_.Back(),
                    "Trying to add \""+pButton->GetNormalTexture()->GetName()+"\" to \""+sName_+"\",\n"
                    "but its name was already taken : \""+pNormalTexture_->GetName()+"\". Skipped."
                );
                pNormalTexture_.Delete();
            }
            else
            {
                if (!IsVirtual())
                    pNormalTexture_->CreateGlue();
                this->AddRegion(pNormalTexture_);
                pNormalTexture_->CopyFrom(pButton->GetNormalTexture());
            }
        }
        if (pButton->GetPushedTexture())
        {
            this->CreatePushedTexture_();
            if (this->IsVirtual())
                pPushedTexture_->SetVirtual();
            pPushedTexture_->SetName(pButton->GetPushedTexture()->GetName());
            if (!GUIManager::GetSingleton()->AddUIObject(pPushedTexture_))
            {
                Warning(lType_.Back(),
                    "Trying to add \""+pButton->GetPushedTexture()->GetName()+"\" to \""+sName_+"\",\n"
                    "but its name was already taken : \""+pPushedTexture_->GetName()+"\". Skipped."
                );
                pPushedTexture_.Delete();
            }
            else
            {
                if (!IsVirtual())
                    pPushedTexture_->CreateGlue();
                this->AddRegion(pPushedTexture_);
                pPushedTexture_->CopyFrom(pButton->GetPushedTexture());
            }
        }
        if (pButton->GetHighlightTexture())
        {
            this->CreateHighlightTexture_();
            if (this->IsVirtual())
                pHighlightTexture_->SetVirtual();
            pHighlightTexture_->SetName(pButton->GetHighlightTexture()->GetName());
            if (!GUIManager::GetSingleton()->AddUIObject(pHighlightTexture_))
            {
                Warning(lType_.Back(),
                    "Trying to add \""+pButton->GetHighlightTexture()->GetName()+"\" to \""+sName_+"\",\n"
                    "but its name was already taken : \""+pHighlightTexture_->GetName()+"\". Skipped."
                );
                pHighlightTexture_.Delete();
            }
            else
            {
                if (!IsVirtual())
                    pHighlightTexture_->CreateGlue();
                this->AddRegion(pHighlightTexture_);
                pHighlightTexture_->CopyFrom(pButton->GetHighlightTexture());
            }
        }
        if (pButton->GetDisabledTexture())
        {
            this->CreateDisabledTexture_();
            if (this->IsVirtual())
                pDisabledTexture_->SetVirtual();
            pDisabledTexture_->SetName(pButton->GetDisabledTexture()->GetName());
            if (!GUIManager::GetSingleton()->AddUIObject(pDisabledTexture_))
            {
                Warning(lType_.Back(),
                    "Trying to add \""+pButton->GetDisabledTexture()->GetName()+"\" to \""+sName_+"\",\n"
                    "but its name was already taken : \""+pDisabledTexture_->GetName()+"\". Skipped."
                );
                pDisabledTexture_.Delete();
            }
            else
            {
                if (!IsVirtual())
                    pDisabledTexture_->CreateGlue();
                this->AddRegion(pDisabledTexture_);
                pDisabledTexture_->CopyFrom(pButton->GetDisabledTexture());
            }
        }

        if (pButton->GetNormalText())
        {
            this->CreateNormalText_();
            if (this->IsVirtual())
                pNormalText_->SetVirtual();
            pNormalText_->SetName(pButton->GetNormalText()->GetName());
            if (!GUIManager::GetSingleton()->AddUIObject(pNormalText_))
            {
                Warning(lType_.Back(),
                    "Trying to add \""+pButton->GetNormalText()->GetName()+"\" to \""+sName_+"\",\n"
                    "but its name was already taken : \""+pNormalText_->GetName()+"\". Skipped."
                );
                pNormalText_.Delete();
            }
            else
            {
                if (!IsVirtual())
                    pNormalText_->CreateGlue();
                this->AddRegion(pNormalText_);
                pNormalText_->CopyFrom(pButton->GetNormalText());
            }
        }
        if (pButton->GetHighlightText())
        {
            this->CreateHighlightText_();
            if (this->IsVirtual())
                pHighlightText_->SetVirtual();
            pHighlightText_->SetName(pButton->GetHighlightText()->GetName());
            if (!GUIManager::GetSingleton()->AddUIObject(pHighlightText_))
            {
                Warning(lType_.Back(),
                    "Trying to add \""+pButton->GetHighlightText()->GetName()+"\" to \""+sName_+"\",\n"
                    "but its name was already taken : \""+pHighlightText_->GetName()+"\". Skipped."
                );
                pHighlightText_.Delete();
            }
            else
            {
                if (!IsVirtual())
                    pHighlightText_->CreateGlue();
                this->AddRegion(pHighlightText_);
                pHighlightText_->CopyFrom(pButton->GetHighlightText());
            }
        }
        if (pButton->GetDisabledText())
        {
            this->CreateDisabledText_();
                if (this->IsVirtual())
                    pDisabledText_->SetVirtual();
            pDisabledText_->SetName(pButton->GetDisabledText()->GetName());
            if (!GUIManager::GetSingleton()->AddUIObject(pDisabledText_))
            {
                Warning(lType_.Back(),
                    "Trying to add \""+pButton->GetDisabledText()->GetName()+"\" to \""+sName_+"\",\n"
                    "but its name was already taken : \""+pDisabledText_->GetName()+"\". Skipped."
                );
                pDisabledText_.Delete();
            }
            else
            {
                if (!IsVirtual())
                    pDisabledText_->CreateGlue();
                this->AddRegion(pDisabledText_);
                pDisabledText_->CopyFrom(pButton->GetDisabledText());
            }
        }

        this->SetPushedTextOffset(pButton->GetPushedTextOffset());

        if (!pButton->IsEnabled())
            this->Disable();
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

s_ptr<Texture> Button::CreateNormalTexture_()
{
    if (!pNormalTexture_)
    {
        pNormalTexture_ = new Texture();
        pNormalTexture_->SetSpecial();
        pNormalTexture_->SetParent(this);
        pNormalTexture_->SetDrawLayer("BORDER");
    }

    return pNormalTexture_;
}

s_ptr<Texture> Button::CreatePushedTexture_()
{
    if (!pPushedTexture_)
    {
        pPushedTexture_ = new Texture();
        pPushedTexture_->SetSpecial();
        pPushedTexture_->SetParent(this);
        pPushedTexture_->SetDrawLayer("BORDER");
    }

    return pPushedTexture_;
}

s_ptr<Texture> Button::CreateDisabledTexture_()
{
    if (!pDisabledTexture_)
    {
        pDisabledTexture_ = new Texture();
        pDisabledTexture_->SetSpecial();
        pDisabledTexture_->SetParent(this);
        pDisabledTexture_->SetDrawLayer("BORDER");
    }

    return pDisabledTexture_;
}

s_ptr<Texture> Button::CreateHighlightTexture_()
{
    if (!pHighlightTexture_)
    {
        pHighlightTexture_ = new Texture();
        pHighlightTexture_->SetSpecial();
        pHighlightTexture_->SetParent(this);
        pHighlightTexture_->SetDrawLayer("HIGHLIGHT");
    }

    return pHighlightTexture_;
}

s_ptr<FontString> Button::CreateNormalText_()
{
    if (!pNormalText_)
    {
        pNormalText_ = new FontString();
        pNormalText_->SetSpecial();
        pNormalText_->SetParent(this);
        pNormalText_->SetDrawLayer("ARTWORK");
        pCurrentFontString_ = pNormalText_;
    }

    return pNormalText_;
}

s_ptr<FontString> Button::CreateHighlightText_()
{
    if (!pHighlightText_)
    {
        pHighlightText_ = new FontString();
        pHighlightText_->SetSpecial();
        pHighlightText_->SetParent(this);
        pHighlightText_->SetDrawLayer("ARTWORK");
    }

    return pHighlightText_;
}

s_ptr<FontString> Button::CreateDisabledText_()
{
    if (!pDisabledText_)
    {
        pDisabledText_ = new FontString();
        pDisabledText_->SetSpecial();
        pDisabledText_->SetParent(this);
        pDisabledText_->SetDrawLayer("ARTWORK");
    }

    return pDisabledText_;
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

// TODO : ### fix lua glues delete

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
    if (pNormalText_ == pCurrentFontString_)
        pCurrentFontString_ = pFont;

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
        mState_ = STATE_DISABLED;
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

        Unlight();
    }
}

void Button::Enable()
{
    if (!IsEnabled())
    {
        mState_ = STATE_UP;
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
    return (mState_ != STATE_DISABLED);
}

void Button::Push()
{
    if (IsEnabled())
    {
        if (pPushedTexture_)
            pPushedTexture_->Show();
        if (pNormalTexture_)
            pNormalTexture_->Hide();

        if (pNormalText_)
            pNormalText_->SetOffsets(lPushedTextOffset_);
    }
}

void Button::Release()
{
    if (IsEnabled())
    {
        if (pPushedTexture_)
            pPushedTexture_->Hide();
        if (pNormalTexture_)
            pNormalTexture_->Show();

        if (pNormalText_)
            pNormalText_->SetOffsets(0, 0);
    }
}

void Button::Highlight()
{
    if (!bHighlighted_)
    {
        if (pHighlightTexture_)
        {
            pHighlightTexture_->Show();
        }

        if (pHighlightText_)
        {
            if (pCurrentFontString_)
                pCurrentFontString_->Hide();
            pCurrentFontString_ = pHighlightText_;
            pCurrentFontString_->Show();
        }

        bHighlighted_ = true;
    }
}

void Button::Unlight()
{
    if (!bLockHighlight_ && bHighlighted_)
    {
        if (pHighlightTexture_)
        {
            pHighlightTexture_->Hide();
        }

        if (pHighlightText_)
        {
            if (pCurrentFontString_)
                pCurrentFontString_->Hide();

            switch (mState_)
            {
                case STATE_UP       : pCurrentFontString_ = pNormalText_; break;
                case STATE_DOWN     : pCurrentFontString_ = pNormalText_; break;
                case STATE_DISABLED : pCurrentFontString_ = pDisabledText_; break;
            }

            if (pCurrentFontString_)
                pCurrentFontString_->Show();
        }


        bHighlighted_ = false;
    }
}

Button::State Button::GetButtonState() const
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

