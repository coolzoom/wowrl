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

void MessageFrame::CreateGlue()
{
    s_ptr<Lua::State> pLua = GUIManager::GetSingleton()->GetLua();
    pLua->PushString(sName_);
    lGlueList_.PushBack(
        pLua->Push<LuaMessageFrame>(new LuaMessageFrame(pLua->GetState()))
    );
    pLua->SetGlobal(sName_);
}
