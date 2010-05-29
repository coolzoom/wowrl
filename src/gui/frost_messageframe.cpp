/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_messageframe.h"

#include "gui/frost_frame.h"
#include "gui/frost_guimanager.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str MessageFrame::CLASS_NAME = "GUI::MessageFrame";

// TODO : Implementer MessageFrame

MessageFrame::MessageFrame() : Frame()
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
        s_ptr<Lua::State> pLua = GUIManager::GetSingleton()->GetLua();
        pLua->PushNumber(uiID_);
        lGlueList_.PushBack(pLua->PushNew<LuaVirtualGlue>());
        pLua->SetGlobal(sLuaName_);
    }
    else
    {
        s_ptr<Lua::State> pLua = GUIManager::GetSingleton()->GetLua();
        pLua->PushString(sLuaName_);
        lGlueList_.PushBack(pLua->PushNew<LuaMessageFrame>());
        pLua->SetGlobal(sLuaName_);
    }
}
