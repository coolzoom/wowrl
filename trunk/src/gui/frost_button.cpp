/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_gui_button.h"

#include "gui/frost_gui_frame.h"
#include "gui/frost_guimanager.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str Button::CLASS_NAME = "GUI::Button";

// TODO : Implementer Button

Button::Button() : Frame()
{
}

void Button::CreateGlue()
{
    s_ptr<Lua::State> pLua = GUIManager::GetSingleton()->GetLua();
    pLua->PushString(sName_);
    LuaButton* pNewGlue;
    pGlue_ = pNewGlue = new LuaButton(pLua->GetState());
    Lunar<LuaButton>::push(pLua->GetState(), pNewGlue);
    pLua->SetGlobal(sName_);
}
