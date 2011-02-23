/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "frost_messageframe.h"

#include "frost_frame.h"
#include "frost_guimanager.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str MessageFrame::CLASS_NAME = "GUI::MessageFrame";

// TODO : Implement MessageFrame

MessageFrame::MessageFrame(s_ptr<GUIManager> pManager) : Frame(pManager)
{
    lType_.PushBack("MessageFrame");
}

MessageFrame::~MessageFrame()
{
}

void MessageFrame::CopyFrom( s_ptr<UIObject> pObj )
{
    Frame::CopyFrom(pObj);
}

void MessageFrame::CreateGlue()
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
        lGlueList_.PushBack(pLua->PushNew<LuaMessageFrame>());
        pLua->SetGlobal(sLuaName_);
    }
}
