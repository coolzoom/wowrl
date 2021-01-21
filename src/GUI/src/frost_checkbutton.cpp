/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "frost_checkbutton.h"

#include "frost_frame.h"
#include "frost_guimanager.h"
#include "frost_texture.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str CheckButton::CLASS_NAME = "GUI::CheckButton";

CheckButton::CheckButton(s_ptr<GUIManager> pManager) : Button(pManager)
{
    lType_.PushBack("CheckButton");
}

CheckButton::~CheckButton()
{
}

s_str CheckButton::Serialize( const s_str& sTab ) const
{
    return Button::Serialize(sTab);
}

void CheckButton::CopyFrom( s_ptr<UIObject> pObj )
{
    Button::CopyFrom(pObj);

    s_ptr<CheckButton> pButton = s_ptr<CheckButton>::DynamicCast(pObj);

    if (pButton)
    {
        if (pButton->GetCheckedTexture())
        {
            this->CreateCheckedTexture_();
            if (this->IsVirtual())
                pCheckedTexture_->SetVirtual();
            pCheckedTexture_->SetName(pButton->GetCheckedTexture()->GetName());
            if (!pManager_->AddUIObject(pCheckedTexture_))
            {
                Warning(lType_.Back(),
                    "Trying to add \""+pButton->GetCheckedTexture()->GetName()+"\" to \""+sName_+"\",\n"
                    "but its name was already taken : \""+pCheckedTexture_->GetName()+"\". Skipped."
                );
                pCheckedTexture_.Delete();
            }
            else
            {
                if (!IsVirtual())
                    pCheckedTexture_->CreateGlue();
                pCheckedTexture_->SetDrawLayer(pButton->GetCheckedTexture()->GetDrawLayer());
                this->AddRegion(pCheckedTexture_);
                pCheckedTexture_->CopyFrom(pButton->GetCheckedTexture());
                pCheckedTexture_->NotifyLoaded();
            }
        }
        if (pButton->GetDisabledCheckedTexture())
        {
            this->CreateDisabledCheckedTexture_();
            if (this->IsVirtual())
                pDisabledCheckedTexture_->SetVirtual();
            pDisabledCheckedTexture_->SetName(pButton->GetDisabledCheckedTexture()->GetName());
            if (!pManager_->AddUIObject(pDisabledCheckedTexture_))
            {
                Warning(lType_.Back(),
                    "Trying to add \""+pButton->GetDisabledCheckedTexture()->GetName()+"\" to \""+sName_+"\",\n"
                    "but its name was already taken : \""+pDisabledCheckedTexture_->GetName()+"\". Skipped."
                );
                pDisabledTexture_.Delete();
            }
            else
            {
                if (!IsVirtual())
                    pDisabledCheckedTexture_->CreateGlue();
                pDisabledCheckedTexture_->SetDrawLayer(pButton->GetDisabledCheckedTexture()->GetDrawLayer());
                this->AddRegion(pDisabledCheckedTexture_);
                pDisabledCheckedTexture_->CopyFrom(pButton->GetDisabledCheckedTexture());
                pDisabledCheckedTexture_->NotifyLoaded();
            }
        }
    }
}

void CheckButton::Check()
{
    if (!bChecked_)
    {
        if (mState_ == STATE_DISABLED)
        {
            if (pDisabledCheckedTexture_)
                pDisabledCheckedTexture_->Show();
            else if (pCheckedTexture_)
                pCheckedTexture_->Show();
        }
        else
        {
            if (pCheckedTexture_)
                pCheckedTexture_->Show();
        }

        bChecked_ = true;
    }
}

void CheckButton::UnCheck()
{
    if (bChecked_)
    {
        if (pDisabledCheckedTexture_)
            pDisabledCheckedTexture_->Hide();

        if (pCheckedTexture_)
            pCheckedTexture_->Hide();

        bChecked_ = false;
    }
}

void CheckButton::Disable()
{
    Button::Disable();

    if (IsEnabled() && IsChecked() && pDisabledCheckedTexture_)
    {
        if (pCheckedTexture_)
            pCheckedTexture_->Hide();

        pDisabledCheckedTexture_->Show();
    }
}

void CheckButton::Enable()
{
    Button::Enable();

    if (!IsEnabled() && IsChecked() && pDisabledCheckedTexture_)
    {
        if (pCheckedTexture_)
            pCheckedTexture_->Show();

        pDisabledCheckedTexture_->Hide();
    }
}

void CheckButton::Release()
{
    Button::Release();

    if (IsChecked())
        UnCheck();
    else
        Check();
}

const s_bool& CheckButton::IsChecked()
{
    return bChecked_;
}

s_ptr<Texture> CheckButton::GetCheckedTexture()
{
    return pCheckedTexture_;
}

s_ptr<Texture> CheckButton::GetDisabledCheckedTexture()
{
    return pDisabledCheckedTexture_;
}

void CheckButton::SetCheckedTexture( s_ptr<Texture> pTexture )
{
    pCheckedTexture_ = pTexture;
}

void CheckButton::SetDisabledCheckedTexture( s_ptr<Texture> pTexture )
{
    pDisabledCheckedTexture_ = pTexture;
}

void CheckButton::CreateGlue()
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
        lGlueList_.PushBack(pLua->PushNew<LuaCheckButton>());
        pLua->SetGlobal(sLuaName_);
        pLua->Pop();
    }
}

s_ptr<Texture> CheckButton::CreateCheckedTexture_()
{
    if (!pCheckedTexture_)
    {
        pCheckedTexture_ = new Texture(pManager_);
        pCheckedTexture_->SetSpecial();
        pCheckedTexture_->SetParent(this);
        pCheckedTexture_->SetDrawLayer("ARTWORK");
    }

    return pCheckedTexture_;
}

s_ptr<Texture> CheckButton::CreateDisabledCheckedTexture_()
{
    if (!pDisabledCheckedTexture_)
    {
        pDisabledCheckedTexture_ = new Texture(pManager_);
        pDisabledCheckedTexture_->SetSpecial();
        pDisabledCheckedTexture_->SetParent(this);
        pDisabledCheckedTexture_->SetDrawLayer("ARTWORK");
    }

    return pDisabledCheckedTexture_;
}
