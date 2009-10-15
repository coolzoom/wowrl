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
    mObjectType_ = OJBECT_TYPE_BUTTON;
    lType_.PushBack("Button");
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

void Button::CopyFrom( s_ptr<UIObject> pObj )
{
    Frame::CopyFrom(pObj);

    s_ptr<Button> pButton = s_ptr<Button>::DynamicCast(pObj);

    if (pButton)
    {
        this->SetText(pButton->GetText());

        if (pButton->GetNormalTexture())
        {
            this->CreateNormalTexture();
            if (this->IsVirtual())
                pNormalTexture_->SetVirtual();
            pNormalTexture_->SetParent(this);
            pNormalTexture_->SetName(pButton->GetNormalTexture()->GetName());
            pNormalTexture_->SetDrawLayer(LAYER_BORDER);
            if (!GUIManager::GetSingleton()->AddUIObject(pNormalTexture_))
            {
                Warning(lType_.Back(),
                    "Trying to add \""+pButton->GetNormalTexture()->GetName()+"\" to \""+sName_+"\", but its name was already taken : \""
                    +pNormalTexture_->GetName()+"\". Skipped."
                );
                pNormalTexture_.Delete();
            }
            else
            {
                this->AddRegion(pNormalTexture_);
                pNormalTexture_->CopyFrom(pButton->GetNormalTexture());
                if (!pNormalTexture_->IsVirtual())
                    pNormalTexture_->CreateGlue();
            }
        }
        if (pButton->GetPushedTexture())
        {
            this->CreatePushedTexture();
            if (this->IsVirtual())
                pPushedTexture_->SetVirtual();
            pPushedTexture_->SetParent(this);
            pPushedTexture_->SetName(pButton->GetPushedTexture()->GetName());
            pPushedTexture_->SetDrawLayer(LAYER_BORDER);
            if (!GUIManager::GetSingleton()->AddUIObject(pPushedTexture_))
            {
                Warning(lType_.Back(),
                    "Trying to add \""+pButton->GetPushedTexture()->GetName()+"\" to \""+sName_+"\", but its name was already taken : \""
                    +pPushedTexture_->GetName()+"\". Skipped."
                );
                pPushedTexture_.Delete();
            }
            else
            {
                this->AddRegion(pPushedTexture_);
                pPushedTexture_->CopyFrom(pButton->GetPushedTexture());
                if (!pPushedTexture_->IsVirtual())
                    pPushedTexture_->CreateGlue();
            }
        }
        if (pButton->GetHighlightTexture())
        {
            this->CreateHighlightTexture();
            if (this->IsVirtual())
                pHighlightTexture_->SetVirtual();
            pHighlightTexture_->SetParent(this);
            pHighlightTexture_->SetName(pButton->GetHighlightTexture()->GetName());
            pHighlightTexture_->SetDrawLayer(LAYER_HIGHLIGHT);
            if (!GUIManager::GetSingleton()->AddUIObject(pHighlightTexture_))
            {
                Warning(lType_.Back(),
                    "Trying to add \""+pButton->GetHighlightTexture()->GetName()+"\" to \""+sName_+"\", but its name was already taken : \""
                    +pHighlightTexture_->GetName()+"\". Skipped."
                );
                pHighlightTexture_.Delete();
            }
            else
            {
                this->AddRegion(pHighlightTexture_);
                pHighlightTexture_->CopyFrom(pButton->GetHighlightTexture());
                if (!pHighlightTexture_->IsVirtual())
                    pHighlightTexture_->CreateGlue();
            }
        }
        if (pButton->GetDisabledTexture())
        {
            this->CreateDisabledTexture();
            if (this->IsVirtual())
                pDisabledTexture_->SetVirtual();
            pDisabledTexture_->SetParent(this);
            pDisabledTexture_->SetName(pButton->GetDisabledTexture()->GetName());
            pDisabledTexture_->SetDrawLayer(LAYER_BORDER);
            if (!GUIManager::GetSingleton()->AddUIObject(pDisabledTexture_))
            {
                Warning(lType_.Back(),
                    "Trying to add \""+pButton->GetDisabledTexture()->GetName()+"\" to \""+sName_+"\", but its name was already taken : \""
                    +pDisabledTexture_->GetName()+"\". Skipped."
                );
                pDisabledTexture_.Delete();
            }
            else
            {
                this->AddRegion(pDisabledTexture_);
                pDisabledTexture_->CopyFrom(pButton->GetDisabledTexture());
                if (!pDisabledTexture_->IsVirtual())
                    pDisabledTexture_->CreateGlue();
            }
        }

        if (pButton->GetNormalText())
        {
            this->CreateNormalText();
            if (this->IsVirtual())
                pNormalText_->SetVirtual();
            pNormalText_->SetParent(this);
            pNormalText_->SetName(pButton->GetNormalText()->GetName());
            pNormalText_->SetDrawLayer(LAYER_ARTWORK);
            if (!GUIManager::GetSingleton()->AddUIObject(pNormalText_))
            {
                Warning(lType_.Back(),
                    "Trying to add \""+pButton->GetNormalText()->GetName()+"\" to \""+sName_+"\", but its name was already taken : \""
                    +pNormalText_->GetName()+"\". Skipped."
                );
                pNormalText_.Delete();
            }
            else
            {
                this->AddRegion(pNormalText_);
                pNormalText_->CopyFrom(pButton->GetNormalText());
                if (!pNormalText_->IsVirtual())
                    pNormalText_->CreateGlue();
            }
        }
        if (pButton->GetHighlightText())
        {
            this->CreateHighlightText();
            if (this->IsVirtual())
                pHighlightText_->SetVirtual();
            pHighlightText_->SetParent(this);
            pHighlightText_->SetName(pButton->GetHighlightText()->GetName());
            pHighlightText_->SetDrawLayer(LAYER_ARTWORK);
            if (!GUIManager::GetSingleton()->AddUIObject(pHighlightText_))
            {
                Warning(lType_.Back(),
                    "Trying to add \""+pButton->GetHighlightText()->GetName()+"\" to \""+sName_+"\", but its name was already taken : \""
                    +pHighlightText_->GetName()+"\". Skipped."
                );
                pHighlightText_.Delete();
            }
            else
            {
                this->AddRegion(pHighlightText_);
                pHighlightText_->CopyFrom(pButton->GetHighlightText());
                if (!pHighlightText_->IsVirtual())
                    pHighlightText_->CreateGlue();
            }
        }
        if (pButton->GetDisabledText())
        {
            this->CreateDisabledText();
                if (this->IsVirtual())
                    pDisabledText_->SetVirtual();
            pDisabledText_->SetParent(this);
            pDisabledText_->SetName(pButton->GetDisabledText()->GetName());
            pDisabledText_->SetDrawLayer(LAYER_ARTWORK);
            if (!GUIManager::GetSingleton()->AddUIObject(pDisabledText_))
            {
                Warning(lType_.Back(),
                    "Trying to add \""+pButton->GetDisabledText()->GetName()+"\" to \""+sName_+"\", but its name was already taken : \""
                    +pDisabledText_->GetName()+"\". Skipped."
                );
                pDisabledText_.Delete();
            }
            else
            {
                this->AddRegion(pDisabledText_);
                pDisabledText_->CopyFrom(pButton->GetDisabledText());
                if (!pDisabledText_->IsVirtual())
                    pDisabledText_->CreateGlue();
            }
        }

        this->SetPushedTextOffset(pButton->GetPushedTextOffset());

        if (!pButton->IsEnabled())
            this->Disable();
    }
    else
    {
        Error(lType_.Back(),
            "Trying to derive \""+sName_+"\" from \""+pObj->GetName()+"\", but they are not of the same type "
            "(respectively "+this->GetObjectType()+" and "+pObj->GetObjectType()+")."
        );
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
        pNormalTexture_->SetSpecial();
    }
}

void Button::CreatePushedTexture()
{
    if (!pPushedTexture_)
    {
        pPushedTexture_ = new Texture();
        pPushedTexture_->SetSpecial();
    }
}

void Button::CreateDisabledTexture()
{
    if (!pDisabledTexture_)
    {
        pDisabledTexture_ = new Texture();
        pDisabledTexture_->SetSpecial();
    }
}

void Button::CreateHighlightTexture()
{
    if (!pHighlightTexture_)
    {
        pHighlightTexture_ = new Texture();
        pHighlightTexture_->SetSpecial();
    }
}

void Button::CreateNormalText()
{
    if (!pNormalText_)
    {
        pNormalText_ = new FontString();
        pNormalText_->SetSpecial();
        pCurrentFontString_ = pNormalText_;
    }
}

void Button::CreateHighlightText()
{
    if (!pHighlightText_)
    {
        pHighlightText_ = new FontString();
        pHighlightText_->SetSpecial();
    }
}

void Button::CreateDisabledText()
{
    if (!pDisabledText_)
    {
        pDisabledText_ = new FontString();
        pDisabledText_->SetSpecial();
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

