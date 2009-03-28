/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_gui_editbox.h"

#include "gui/frost_gui_frame.h"
#include "gui/frost_guimanager.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str EditBox::CLASS_NAME = "GUI::EditBox";

// TODO : Implementer EditBox

EditBox::EditBox() : Frame()
{
}

void EditBox::CreateGlue()
{
    s_ptr<Lua::State> pLua = GUIManager::GetSingleton()->GetLua();
    pLua->PushString(sName_);
    LuaEditBox* pNewGlue;
    pGlue_ = pNewGlue = new LuaEditBox(pLua->GetState());
    Lunar<LuaEditBox>::push(pLua->GetState(), pNewGlue);
    pLua->SetGlobal(sName_);
}
