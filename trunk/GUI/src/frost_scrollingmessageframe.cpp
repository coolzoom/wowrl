/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "frost_scrollingmessageframe.h"

#include "frost_frame.h"
#include "frost_guimanager.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str ScrollingMessageFrame::CLASS_NAME = "GUI::ScrollingMessageFrame";

// TODO : Implement ScrollingMessageFrame

ScrollingMessageFrame::ScrollingMessageFrame(s_ptr<GUIManager> pManager) : Frame(pManager)
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
        s_ptr<Lua::State> pLua = pManager_->GetLua();
        pLua->PushNumber(uiID_);
        lGlueList_.PushBack(pLua->PushNew<LuaVirtualGlue>());
        pLua->SetGlobal(sLuaName_);
    }
    else
    {
        s_ptr<Lua::State> pLua = pManager_->GetLua();
        pLua->PushString(sLuaName_);
        lGlueList_.PushBack(pLua->PushNew<LuaScrollingMessageFrame>());
        pLua->SetGlobal(sLuaName_);
    }
}
