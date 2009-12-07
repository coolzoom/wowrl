/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_checkbutton.h"

#include "gui/frost_frame.h"
#include "gui/frost_guimanager.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str CheckButton::CLASS_NAME = "GUI::CheckButton";

// TODO : Implement CheckButton

CheckButton::CheckButton() : Button()
{
    lType_.PushBack("CheckButton");
}

CheckButton::~CheckButton()
{
}

void CheckButton::CreateGlue()
{
    s_ptr<Lua::State> pLua = GUIManager::GetSingleton()->GetLua();
    pLua->PushString(sName_);
    lGlueList_.PushBack(
        pLua->Push<LuaCheckButton>(new LuaCheckButton(pLua->GetState()))
    );
    pLua->SetGlobal(sName_);
}
