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
    lType_.PushBack("ScrollingMessageFrame");
}

ScrollingMessageFrame::~ScrollingMessageFrame()
{
}

void ScrollingMessageFrame::CopyFrom( s_ptr<UIObject> pObj )
{
    Frame::CopyFrom(pObj);
}

void ScrollingMessageFrame::CreateGlue()
{
    if (bVirtual_)
    {
        s_ptr<Lua::State> pLua = GUIManager::GetSingleton()->GetLua();
        pLua->PushNumber(uiID_);
        lGlueList_.PushBack(
            pLua->Push<LuaVirtualGlue>(new LuaVirtualGlue(pLua->GetState()))
        );
        pLua->SetGlobal(sLuaName_);
    }
    else
    {
        s_ptr<Lua::State> pLua = GUIManager::GetSingleton()->GetLua();
        pLua->PushString(sLuaName_);
        lGlueList_.PushBack(
            pLua->Push<LuaScrollingMessageFrame>(new LuaScrollingMessageFrame(pLua->GetState()))
        );
        pLua->SetGlobal(sLuaName_);
    }
}
