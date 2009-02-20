/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_gui_scrollingmessageframe.h"

#include "gui/frost_gui_frame.h"
#include "frost_guimanager.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str ScrollingMessageFrame::CLASS_NAME = "GUI::ScrollingMessageFrame";

ScrollingMessageFrame::ScrollingMessageFrame() : Frame()
{
}

void ScrollingMessageFrame::CreateGlue()
{
    s_ptr<Lua::State> pLua = GUIManager::GetSingleton()->GetLua();
    pLua->PushString(sName_);
    LuaScrollingMessageFrame* pNewGlue;
    pGlue_ = pNewGlue = new LuaScrollingMessageFrame(pLua->GetState());
    Lunar<LuaScrollingMessageFrame>::push(pLua->GetState(), pNewGlue);
    pLua->SetGlobal(sName_);
}
