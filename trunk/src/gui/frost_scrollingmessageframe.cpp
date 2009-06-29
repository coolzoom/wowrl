/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_scrollingmessageframe.h"

#include "gui/frost_frame.h"
#include "gui/frost_guimanager.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str ScrollingMessageFrame::CLASS_NAME = "GUI::ScrollingMessageFrame";

// TODO : Implementer ScrollingMessageFrame

ScrollingMessageFrame::ScrollingMessageFrame() : Frame()
{
}

void ScrollingMessageFrame::CreateGlue()
{
    s_ptr<Lua::State> pLua = GUIManager::GetSingleton()->GetLua();
    pLua->PushString(sName_);
    lGlueList_.PushBack(
        pLua->Push<LuaScrollingMessageFrame>(new LuaScrollingMessageFrame(pLua->GetState()))
    );
    pLua->SetGlobal(sName_);
}
