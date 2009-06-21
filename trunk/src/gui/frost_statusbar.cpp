/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_gui_statusbar.h"

#include "gui/frost_gui_frame.h"
#include "gui/frost_guimanager.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str StatusBar::CLASS_NAME = "GUI::StatusBar";

// TODO : Implementer StatusBar

StatusBar::StatusBar() : Frame()
{
}

void StatusBar::CreateGlue()
{
    s_ptr<Lua::State> pLua = GUIManager::GetSingleton()->GetLua();
    pLua->PushString(sName_);
    lGlueList_.PushBack(
        pLua->Push<LuaStatusBar>(new LuaStatusBar(pLua->GetState()))
    );
    pLua->SetGlobal(sName_);
}
